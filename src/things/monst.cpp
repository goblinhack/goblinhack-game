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

    $Id: monst.cpp 1118 2008-12-20 20:44:56Z Ghack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

void gh_thing::monst_look_around (void)
{
    GH_FUNCTION()

    //
    // If this thing can see, work out what things are visible
    // to it. This is just a list of pointers. It shouldn't
    // be relied on after an event occurs.
    //
    if (!can_see_distance()) {
        //
        // Nothing to see here. Could be blind, so check there's nothing
        // on us.
        //
        meet_all_at_new_position();
        look_last_when = gh_get_time_milli();

        return;
    }

    //
    // What can we see?
    //
    what_can_i_see();

    look_last_when = gh_get_time_milli();
}

//
// This is really 'thing tick. 
//
void gh_thing::monst_tick (void)
{
    GH_FUNCTION()

    if (is_dead) {
        return;
    }

    //
    // Monsters get to look around.
    //
    if (is_monst) {
        //
        // If in a chain, follow the leader.
        //
        if (prev) {
            //
            // But let torsos with multiple limbs drive.
            //
            if (!limb_next && !limb_prev) {
                follow(prev);
                return;
            }
        }

        if (gh_have_x_tenths_passed_since(look_delay_in_tenths,
                                          look_last_when)) {

            monst_look_around();

            if (is_dead) {
                return;
            }

            //
            // If this thing isn't mindless then choose a new direction.
            //
            if (!moves_until_collision && !is_explorer) {
                //
                // In corridors, move in straight lines more.
                //
                if (level->map.is_on_corridor(at)) {
                    if (gh_math::rand1000() < 5) {
                        monst_choose_new_dir();
                    }
                } else {
                    monst_choose_new_dir();
                }
            }

            if (is_dead) {
                return;
            }
        }
    }

    monst_move();
    if (is_dead) {
        return;
    }

    (void) monst_spawn();
    if (is_dead) {
        return;
    }

    //
    // Want to jump around the level?
    //
    if (self_teleport()) {
        //
        // Could continue, but seems unfair to do to much.
        //
        return;
    }

    if (is_dead) {
        return;
    }

    //
    // Will be dead if this works.
    //
    if (monst_transmog()) {
        return;
    }

    if (is_dead) {
        return;
    }
}
