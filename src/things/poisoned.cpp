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

    $Id: poisoned.cpp 621 2008-01-04 15:25:24Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>

//
// Give the thing a boost of "poisoned", that fades away
//
void gh_thing::poisoned_boost_start (int boost)
{
    GH_FUNCTION()

    if (boost == 0) {
        return;
    }

    poisoned_boost += boost;

    //
    // Update the graphics
    //
    update_anim();
}

//
// If a boost is in operation, gradually ease off of its effects.
//
void gh_thing::poisoned_boost_tick (void)
{
    GH_FUNCTION()

    if (poisoned_boost > 0) {
        poisoned_boost--;
    } else if (poisoned_boost < 0) {
        poisoned_boost++;
    }
    if (poisoned_boost == 0) {
        poisoned_boost_stop();
    }

    if (poisoned()) {
        stamina_tick();
        stamina_tick();
        stamina_tick();
        stamina_tick();
        stamina_tick();
        stamina_tick();
        stamina_tick();
        stamina_tick();
        stamina_tick();

        hit(this,
            NULL, /* killed */
            0, /* bite_damage */
            0, /* bite_armour */
            0, /* bite_treasure */
            0, /* bite_karma */
            0, /* bite_karma_chance */
            0, /* bite_intelligence */
            0, /* bite_intelligence_chance */
            poisoned(),
            100, /* poison_bite_change */
            0, /* bite_stone */
            0, /* bite_stone_chance */
            0, /* bite_fate */
            0, /* bite_fate_chance */
            NULL /* weapon */,
            true /* mand attack */);
    }
}

//
// The boost is done. Undo any settings.
//
void gh_thing::poisoned_boost_stop (void)
{
    GH_FUNCTION()

    poisoned_boost = 0;

    //
    // Update the graphics
    //
    update_anim();
}
