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

//
// Make all things like this one dislike the hero more!
//
void gh_thing::anger_all (gh_thing *target) const
{
    GH_FUNCTION()

    //
    // No point auto angering the hero. That's what the user is for!
    //
    if (target->is_hero) {
        return;
    }

    if (target->intelligence < 30) {
        //
        // Silly things like worms, walls etc... cannot be angered.
        //
        return;
    }

    //
    // Hitting the body, annoy the head!
    //
    if (target->is_body_part) {
        if (target->head() != target) {
            anger_all(target->head());
            return;
        }
    }

    bool first_time_angered = false;
    bool first_time_irritated = false;

    //
    // Annoy the thing.
    //
    anger(target, &first_time_angered, &first_time_irritated);

    if (is_hero) {
        if (first_time_angered) {
            level->game->new_message("%%fg=redYou anger " +
                                     target->the_named());
        } else if (first_time_irritated) {
            level->game->new_message("%%fg=greenYou annoy " +
                                     target->the_named());
        }
    }

    //
    // Any others on the level to annoy?
    //
    list <class gh_thing*>::iterator l = level->does_stuff_things.begin();

    while (l != level->does_stuff_things.end()) {
        gh_thing *t;

        t = *l;

        if (t->name == target->name) {
            anger(t,
                  &first_time_angered,
                  &first_time_irritated);
        }

        l++;
    }
}

void gh_thing::anger (gh_thing *target, 
                      bool *first_time_angered,
                      bool *first_time_irritated) const
{
    GH_FUNCTION()

    vector<string>::iterator pattern;
    size_t index;
    bool found = false;

    //
    // No point auto angering the hero. That's what the user is for!
    //
    if (target->is_hero) {
        return;
    }

    //
    // Hitting the body, annoy the head!
    //
    if (is_body_part) {
        gh_thing *h = target->head();

        if (h != target) {
            return (target->anger(h));
        }
    }

    string target_string = tostring(id);

    //
    // Look for an exact match in the existing attack list and make it
    // more likely to hit the hero if found.
    //
    for (index = 0, pattern = target->attacks.begin();
         pattern != target->attacks.end(); 
         pattern++, index++) {
        
        if (*pattern == target_string) {
            if (!target->attack_chance[index]) {
                if (first_time_irritated) {
                    *first_time_irritated = true;
                }
            }

            target->attack_chance[index] += 10;

            if (target->attack_chance[index] >= GH_THING_ANGER_LIMIT) {
                if (first_time_angered) {
                    *first_time_angered = true;
                }
            }

            found = true;
            break;
        }
    }

    //
    // Make the thing less likely to follow us if it is following alreaady.
    //
    for (index = 0, pattern = target->follows.begin();
         pattern != target->follows.end(); 
         pattern++, index++) {

        if (*pattern == target_string) {
            target->follow_chance[index] -= 5;
            break;
        }
    }

    //
    // We didn't fid an exact match, add one now.
    //
    if (!found) {
        target->attacks.insert(target->attacks.begin(), target_string);
        target->attack_chance.insert(target->attack_chance.begin(), 10);

        if (first_time_irritated) {
            *first_time_irritated = true;
        }
    }
}
