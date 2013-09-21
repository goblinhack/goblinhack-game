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

    $Id: game.cpp 307 2006-12-03 15:20:21Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_player.h>
#include <gh_ptr.h>
#include <gh_templates.h>
#include <gh_ptr.h>
#include <gh_room.h>
#include <gh_thing_registration.h>

//
// Well, we've actually done the level load already, but this is where
// we complete the loading by fixing up pointers.
//
void gh_game::fixup (void)
{
    GH_FUNCTION()

    GH_LOG("");
    GH_LOG("*** Load fixup *** ");
    GH_LOG("");

    //
    // Fixup the player
    //
    player->game = this;

    //
    // Fixup the level
    //
    gh_level *level = player->thing->level;
    if (!level) {
        GH_THROW("no level to fixup");
    }

    level->game = this;

    level->fixup();

    new_message("%%fg=redWelcome back, Mortal%%fg=green", true /* unique */);
}
