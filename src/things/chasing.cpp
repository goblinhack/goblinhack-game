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

    $Id: chasing.cpp 1163 2009-02-07 18:06:44Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

void gh_thing::found_something_to_chase (gh_thing *t)
{
    GH_FUNCTION()

    //
    // Wake up and pounce!
    //
    if (sleeping()) {
        if (!wake_reason(t)) {
            return;
        }

        _sleeping = 0;

        //
        // So that waking mines can harm anyone, even the thrower.
        //
        if (missile_thrower) {
            missile_thrower = NULL;
            friend_id = 0;
        }

        //
        // Update the graphics
        //
        update_anim();

        speed = metabolism() / 10;
    }

    is_chasing = true;

    target = t->at;

    direction = gh_point3d(0,0,0);

    if (target.x < at.x) {
        direction.x = -speed;
    }

    if (target.x > at.x) {
        direction.x = speed;
    }

    if (target.y < at.y) {
        direction.y = -speed;
    }

    if (target.y > at.y) {
        direction.y = speed;
    }

    target_last_set_where = at;
    target_last_thing = t;

    update_anim();
}

void gh_thing::found_something_to_run_from (gh_thing *t)
{
    GH_FUNCTION()

    is_chasing = false;

    direction = gh_point3d(0,0,0);

    if (target.x > at.x) {
        direction.x = -speed;
    }

    if (target.x < at.x) {
        direction.x = speed;
    }

    if (target.y > at.y) {
        direction.y = -speed;
    }

    if (target.y < at.y) {
        direction.y = speed;
    }

    update_anim();
}

int gh_thing::want_to_follow (gh_thing *target)
{
    GH_FUNCTION()

    string name = target->short_name;
    vector<string>::iterator pattern;
    int result = 0;
    size_t index = 0;

    if (target == this) {
        return (0);
    }

    if (target->id == id) {
        return (0);
    }

    //
    // No chasing corpses in this game!
    //
    if (target->is_dead) {
        return (0);
    }

    //
    // Hit your own weapon? Madness!
    //
    if (target == weapon) {
        return (0);
    }

    //
    // Chase a missile. Not wise.
    //
    if (target->is_missile) {
        return (0);
    }

    //
    // Seems unfair to follow (see dimensional rifts) when invisible
    // e.g. they will throw lava balls when unseen - which is really mean.
    //
    if (is_invisible_anim()) {
        return (0);
    }

    //
    // Swords always like to follow!
    //
    if (is_sword || is_staff || is_axe || is_warhammer) {
        //
        // But only if we are swinging.
        //
        if (weapon_spin) {
            return (100);
        }

        return (0);
    }

    if (is_monst) {
        //
        // No point having monsters chase traps.
        //
        if (target->is_trap) {
            return (0);
        }
    }

    //
    // Now, see if this is one of the things we like to chase?
    //
    for (pattern=follows.begin(); pattern != follows.end(); pattern++, index++) {
        if (!(*pattern).size()) {
            //
            // Too general. You end up chasing treasure. Rather be specific.
            //
            continue;
        }

        //
        // And what are the chances of us chasing it?
        //
        if (index >= follow_chance.size()) {
            //
            // We never chase it.
            //
            GH_THROW("overflow on follow table for " + name + 
                     " wanted index " + tostring(index) + 
                     " real size is " + tostring(follow_chance.size()));

            gh_err(this);
            break;
        }

        bool chase = target->matches(*pattern);

        if (chase) {
            if (!follow_chance[index]) {
                continue;
            }

            if (follow_chance[index] < 0) {
                result -= (gh_math::rand100() % -follow_chance[index]);
            } else {
                result += (gh_math::rand100() % follow_chance[index]);
            }
        }
    }

    //
    // A friend of ours.
    //
    if (target->id && (target->id == friend_id)) {
        if (target->is_hero) {
            result += 50;
        }
    }

    //
    // In the same possessed group?
    //
    if (target->friend_id && (target->friend_id == friend_id)) {
        if (target->is_hero) {
            result += 50;
        }
    }

    return (result);
}
