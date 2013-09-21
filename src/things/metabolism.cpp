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

    $Id: metabolism.cpp 1298 2009-08-24 19:52:20Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_game.h>

//
// Give the thing a boost of "metabolism", that fades away
//
void gh_thing::metabolism_boost_start (int boost)
{
    GH_FUNCTION()

    if (!boost) {
        return;
    }

    metabolism_boost += boost;
}

//
// If a boost is in operation, gradually ease off of its effects.
//
void gh_thing::metabolism_boost_tick (void)
{
    GH_FUNCTION()

    if (metabolism_boost > 0) {
        metabolism_boost--;
    } else if (metabolism_boost < 0) {
        metabolism_boost++;
    }
    if (metabolism_boost == 0) {
        metabolism_boost_stop();
    }
}

//
// The boost is done. Undo any settings.
//
void gh_thing::metabolism_boost_stop (void)
{
    GH_FUNCTION()
}
