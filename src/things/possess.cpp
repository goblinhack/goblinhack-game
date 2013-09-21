/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: chasing.cpp 209 2006-10-18 02:58:42Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_game.h>
#include <gh_level.h>

bool gh_thing::possess_monst (gh_thing *monst)
{
    GH_FUNCTION()

    vector<string>::iterator pattern;
    size_t index;

    //
    // Make possessed demons that possess imps, not attack the hero.
    //
    bool monster_summoner_chases_heros = true;

    //
    // Make sure you can't possess angels if you are evil.
    //
    // Be lenient, so you can be slightly nasty or slightly good and
    // a demon will protect you.
    //
    if (abs(karma - monst->karma) > 1200) {
        if (is_hero) {
            level->game->new_message("%%fg=red" + 
                                     monst->the_named() +
                                     " will not be possessed by the "
                                     "likes of you!");
        }

        GH_THING_LOG(this, "failed to possess " + monst->named());

        return (false);
    }
    
    string target_string = tostring(monst->id);

    if (is_monst) {
        //
        // Does the monster summoner chase the hero? If not, we want to make 
        // the new creature do the same. This means imps summoned by demons 
        // will not attack the hero.
        //
        for (index = 0, pattern = attacks.begin(); 
             pattern != attacks.end(); 
             pattern++, index++) {

            //
            // Look for an explicit 'don't chase the hero entry'.
            //
            if (*pattern == "hero") {
                if (attack_chance[index] <= 0) {
                    //
                    // No chance of chasing the hero.
                    //
                    monster_summoner_chases_heros = false;
                }
            }
        } 

        //
        // This is a monster summoning. Does it chase the hero?
        //
        if (!monster_summoner_chases_heros) {
            //
            // No, it's tame. Make the new creature tame too.
            //
            monst->attacks.insert(monst->attacks.begin(), "hero");
            monst->attack_chance.insert(monst->attack_chance.begin(), -220);
        }
    }

    //
    // Make the poor possessed thing follow us
    // 
    monst->follows.insert(monst->follows.begin(), short_name);
    monst->follow_chance.insert(monst->follow_chance.begin(), 200);

    //
    // But not to cluster others also possessed.
    //
    monst->follows.insert(monst->follows.begin(), monst->short_name);
    monst->follow_chance.insert(monst->follow_chance.begin(), -200);

    if (is_hero) {
        //
        // Make our pet really go for the bad guys.
        //
        monst->follows.insert(monst->follows.begin(), "evil");
        monst->follow_chance.insert(monst->follow_chance.begin(), 70);

        monst->follows.insert(monst->follows.begin(), "monst");
        monst->follow_chance.insert(monst->follow_chance.begin(), 70);

        monst->follows.insert(monst->follows.begin(), "generator");
        monst->follow_chance.insert(monst->follow_chance.begin(), 70);

        //
        // Also, make it chase existing monsters so it can protect us.
        //
        monst->attacks.insert(monst->attacks.begin(), "monst");
        monst->attack_chance.insert(monst->attack_chance.begin(), 200);

        //
        // Make the monster help out by destroying generators.
        //
        monst->attacks.insert(monst->attacks.begin(), "generator");
        monst->attack_chance.insert(monst->attack_chance.begin(), 200);
    }

    //
    // Make the id the same so that we don't attack fellow possessed
    // monsters. Or us.
    //
    monst->friend_id = id;

    GH_THING_LOG(this, "possessed " + monst->named());

    return (true);
}

void gh_thing::collect_followers (void)
{
    GH_FUNCTION()

    int res = GH_RES;
    int sx = -3;
    int sy = -3;
    int ex = +3;
    int ey = +3;

    for (int i = sx; i <= ex; i++) {
        for (int j = sy; j <= ey; j++) {
            vector <gh_thing*> **vp =
                level->map.real_isatp(at + gh_point3d(i*res, j*res, 0));

            if (vp == NULL) {
                continue;
            }

            vector <gh_thing*> *v = *vp;

            if (v == NULL) {
                continue;
            }
redo:
            vector<gh_thing*>::iterator i;

            for (i = v->begin(); i != v->end(); i++) {
                gh_thing *t = *i;

                if (t->is_monst) {
                    if (t->friend_id == id) {
                        collect_item(t, true /* force */);
                        goto redo;
                    }
                }
            }
        }
    }
}

void gh_thing::deposit_followers (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i  = carrying.begin();

    while (i != carrying.end()) {
        gh_thing *item = *i++;

        if (!item->is_monst) {
            continue;
        }

        GH_THING_LOG(item, "deposit follower on level start");

        drop_item(item, true /* force */);
    }
}
