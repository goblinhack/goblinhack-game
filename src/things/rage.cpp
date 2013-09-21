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

    $Id: hit.cpp 419 2007-01-27 23:58:24Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_subsys.h>
#include <gh_math.h>

//
// Go mad!
//
void gh_thing::rage (void)
{                       
    GH_FUNCTION()

    raging_boost_start(30); // 15 secs
}

//
// Find out the current "raging", taking the boost into account.
//
int gh_thing::raging (void)
{
    GH_FUNCTION()

    return (_raging + raging_boost);
}

//
// Give the thing a boost of "raging", that fades away
//
void gh_thing::raging_boost_start (int boost)
{
    GH_FUNCTION()

    if (boost == 0) {
        return;
    }

    if (!raging_boost) {
        if (is_hero) {
            level->game->new_message("%%fg=redGrr!!! "
                                     "%%fg=green You're beserk!");
        }

        play("dinosaur");
    }

    raging_boost += boost;

    metabolism_boost_start(metabolism() / 4);
    stamina_boost_start(stamina() / 2);

    //
    // Update the graphics
    //
    update_anim();
}

//
// If a boost is in operation, gradually ease off of its effects.
//
void gh_thing::raging_boost_tick (void)
{
    GH_FUNCTION()

    if (raging_boost > 0) {
        raging_boost--;

        flash(GH_ORANGE);
    } else if (raging_boost < 0) {
        raging_boost++;

        flash(GH_ORANGE);
    }

    if (raging_boost == 0) {
        raging_boost_stop();
    }
}

//
// The boost is done. Undo any settings.
//
void gh_thing::raging_boost_stop (void)
{
    GH_FUNCTION()

    raging_boost = 0;

    //
    // Update the graphics
    //
    update_anim();
}
