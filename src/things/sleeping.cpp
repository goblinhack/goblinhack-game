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

    $Id: sleeping.cpp 1123 2008-12-21 20:18:19Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>

//
// Give the thing a boost of "sleeping", that fades away
//
void gh_thing::sleeping_boost_start (int boost)
{
    GH_FUNCTION()

    if (boost == 0) {
        return;
    }

    sleeping_boost += boost;

    //
    // Update the graphics
    //
    update_anim();
}

//
// If a boost is in operation, gradually ease off of its effects.
//
void gh_thing::sleeping_boost_tick (void)
{
    GH_FUNCTION()

    if (sleeping_boost > 0) {
        sleeping_boost--;
    } else if (sleeping_boost < 0) {
        sleeping_boost++;
    }
    if (sleeping_boost == 0) {
        sleeping_boost_stop();
    }
}

//
// The boost is done. Undo any settings.
//
void gh_thing::sleeping_boost_stop (void)
{
    GH_FUNCTION()

    sleeping_boost = 0;

    //
    // Update the graphics
    //
    update_anim();
}

bool gh_thing::wake_reason (gh_thing *target)
{
    GH_FUNCTION()

    vector<string>::iterator pattern;

    if (target == this) {
        return (false);
    }

    //
    // My own body?
    //
    if (target->id == id) {
        return (false);
    }

    //
    // My own possessions?
    //
    if (target->get_owner() == this) {
        return (false);
    }

    //
    // Now, see if this is one of the things we like to attack?
    //
    for (pattern=wake_reasons.begin(); 
         pattern != wake_reasons.end(); pattern++) {
        if (!(*pattern).size()) {
            //
            // To general. You end up chasing treasure. Rather be specific.
            //
            continue;
        }

        if (target->matches(*pattern)) {
            return (true);
        }
    }

    return (false);
}
