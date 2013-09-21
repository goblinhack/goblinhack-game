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

    $Id: player.cpp 279 2006-11-24 21:49:26Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_thing.h>

void gh_player::save (void)
{
    GH_FUNCTION()

    if (!thing->level) {
        return;
    }

    game->save();

    //
    // Don't save debug mode else when we load we will turn it on
    // but only for new things, and will get into a mess.
    //
    bool debug = gh_global::debug;

    debug = false;

    game->new_message("%%fg=redThou Art Saved, Mortal%%fg=green",
                      true /* unique */);

    gh_global::debug = debug;
}
