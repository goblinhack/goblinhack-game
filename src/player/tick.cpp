/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (look your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: tick.cpp 685 2008-01-26 22:15:33Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_level.h>
#include <gh_game.h>

void gh_thing::hero_tick (void)
{
    GH_FUNCTION()

    static int tick_count;

    //
    // Update the item list every now and again - less than once per
    // second for changes.
    //
    if (++tick_count > 20) {
        tick_count = 0;
        player->make_item_list();
    }

    const gh_point3d saved_dir = direction_wanted;

    if (direction_wanted.x && direction_wanted.y) {

        direction_wanted.x = 0;
        if (!try_to_move_in_chosen_direction(false /* try other */)) {
        }

        if (is_dead) {
            return;
        }

        direction_wanted = saved_dir;
        direction_wanted.y = 0;
        if (!try_to_move_in_chosen_direction(false /* try other */)) {
        }

        if (is_dead) {
            return;
        }

        direction_wanted = saved_dir;
        direction_last = saved_dir;

    } else if (direction_wanted.x || direction_wanted.y) {
        if (!try_to_move_in_chosen_direction(false /* try other */)) {
        }

        if (is_dead) {
            return;
        }

        direction_last = saved_dir;
    }

    if (direction_wanted == gh_point3d(0,0,0)) {
        if (speed) {
            speed--;
        }
    }
}
