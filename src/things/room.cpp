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

    $Id: meet.cpp 276 2006-11-20 21:16:05Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_room.h>
#include <gh_thing.h>
#include <gh_templates.h>

void gh_thing::enter_room (gh_thing *floor_tile)
{
    GH_FUNCTION()

    gh_room *new_room;

    new_room = floor_tile->room;

    //
    // Leaving a room via a corridor or something else?
    //
    if (room) {
        if (new_room != room) {
            leave_room();

            room = new_room;
        }

        return;
    }

    room = new_room;

    //
    // Entering a room?
    //
    if (!new_room) {
        //
        // No
        //
        return;
    }

    if (is_hero) {
        //
        // New room?
        //
        if (contains(&visited, new_room)) {
            //
            // No.
            //
            return;
        }

        //
        // Yes.
        //
        visited.push_back(new_room);

        //
        // Welcome.
        //
        if (new_room->upon_enter != "") {
            if (new_room->is_shop) {
                //
                // Welcome to a shop.
                //
                gh_thing *shopkeeper = new_room->shopkeeper;

                if (!shopkeeper) {
                    level->game->new_message("This shop seems deserted. Odd...");
                    return;
                }

                //
                // Dead keepers don't complain.
                //
                if (shopkeeper->is_dead) {
                    level->game->new_message("This shop seems deserted.");
                }

                level->game->new_message("%%fg=yellow" + new_room->upon_enter +
                                         "%%fg=green");
            } else {
                //
                // Welcome to ... something else...
                //
                level->game->new_message("%%fg=yellow" + new_room->upon_enter +
                                         "%%fg=green");

                play("doorbell2");
            }
        }
    }
}

void gh_thing::leave_room (void)
{
    GH_FUNCTION()

    if (!room) {
        return;
    }

    if (room->is_shop) {
        leave_shop();
    }

    //
    // Make sure all carried items forget where they came from -
    // so we have no problems when loading this game again.
    //
    reset_all_items();

    room = NULL;
}
