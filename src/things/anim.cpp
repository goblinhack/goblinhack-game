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

    $Id: gfx.cpp 212 2006-10-21 15:32:50Z goblinhack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_thing.h>
#include <gh_console.h>
#include <gh_math.h>
#include <gh_level.h>

bool gh_thing::update_anim (void)
{
    GH_FUNCTION()

    //
    // Keep the weapon in sync with the player.
    //
    if (weapon) {
        weapon->direction = direction;
        weapon->direction_last = direction_last;
        weapon->direction_wanted = direction_wanted;

        (void) weapon->get_dir_and_mode(&weapon->dir, &weapon->mode);
    }

    return (get_dir_and_mode(&dir, &mode));
}

//
// Calculate the animation frame to use for this thing. Reflect what
// we decide on, on the weapon being carried too.
//
bool gh_thing::get_dir_and_mode (gh_thing_dir *dir, gh_thing_mode *mode)
{
    GH_FUNCTION()

    //
    // Map the things direction, or direction it wants,
    // into an anim key.
    //
    gh_point3d d(0,0,0);

    //
    // The hero's dir is a combo of cursor key presses and we only
    // get one at a time, so the direction ping pongs between straight
    // and diagonal.
    //
    if (is_hero) {
        d = direction_wanted;
    } else if (weapon_wielder) {
        //
        // If we're a weapon being wielded and the wielder is a hero, then
        // copy the direction, so we get diagonals, as mentioned above.
        //
        d = weapon_wielder->direction_wanted;
    }

    if (d == gh_point3d(0,0,0)) {
        d = direction_last;
    }

    if (d == gh_point3d(0,0,0)) {
        d = direction_wanted;
    }

    //
    // If this thing joins onto other things, its direction is hardwired.
    //
    if (join != GH_THING_DIR_NONE) {
        *dir = join;
    } else {
        //
        // Account for rounding of missiles in movements. e.g. moving
        // x 1 and y 100 really is more down than bottom right.
        //
        int dx = d.x;
        int dy = d.y;

        if (is_missile) {
            if ((d.x > 0) && (d.x > d.y)) {
                dy = 0;
            }

            if ((d.x < 0) && (d.x < d.y)) {
                dy = 0;
            }

            if ((d.y > 0) && (d.y > d.x)) {
                dx = 0;
            }

            if ((d.y < 0) && (d.y < d.x)) {
                dx = 0;
            }
        }

        //
        // Map the things direction, or direction it wants,
        // into an anim key
        //
        if (dx < 0) {
            if (dy < 0) {
                *dir = GH_THING_DIR_TL;
            } else if (dy == 0) {
                *dir = GH_THING_DIR_L;
            } else {
                *dir = GH_THING_DIR_BL;
            }
        } else if (dx == 0) {
            if (dy < 0) {
                *dir = GH_THING_DIR_U;
            } else if (dy == 0) {
                *dir = GH_THING_DIR_ALL;
            } else {
                *dir = GH_THING_DIR_D;
            }
        } else {
            if (dy < 0) {
                *dir = GH_THING_DIR_TR;
            } else if (dy == 0) {
                *dir = GH_THING_DIR_R;
            } else {
                *dir = GH_THING_DIR_BR;
            }
        }
    }

    //
    // Make swords spin when being used.
    //
    if (is_sword || is_staff || is_axe || is_warhammer) {
        if (weapon_spin) {
            int d = (int)*dir;

            //
            // Make the direction loop through the 8 stages of the compass.
            //
            d += weapon_spin;
            d = d % 8;

            *dir = (gh_thing_dir)d;
        }
    }

    //
    // Map the things state into an anim key.
    //
    if (scared()) {
        *mode = GH_THING_MODE_SCARED;
    } else if (sleeping()) {
        *mode = GH_THING_MODE_SLEEPING;
    } else if (is_open) {
        *mode = GH_THING_MODE_OPEN;
    } else if (is_closed) {
        *mode = GH_THING_MODE_CLOSED;
    } else {
        *mode = GH_THING_MODE_NORMAL;
    }

    //
    // Don't want sleeping exits on level editor mode.
    //
    if (level && level->is_editing) {
        *mode = GH_THING_MODE_NORMAL;
    }

    //
    // If nothing is available for that direction, try a more
    // generic direction. Else, return.
    //
    if (!anim_frames) {
        //
        // Limb bodys are not drawn - only the first body.
        //
        if (limb_prev != NULL) {
            return (true);
        }

        if (!template_thing) {
            GH_THROW("no template or anim frame for " + name);
        }

        anim_frames = template_thing->anim_frames;

        if (!anim_frames) {
            GH_THROW("no anim frame for " + name);
        }
    }

    if (anim_frames->anim_frame[*dir][*mode][0].size()) {
        //
        // If we've changed mode/dir etc... then make sure the new mode
        // has sufficent anim frames.
        //
        if (frame >= anim_frames->anim_frame[*dir][*mode][0].size()) {
            frame = 0;
        }

        return (true);
    }

    //
    // If this thing joins onto other things, its direction is hardwired.
    //
    if (join == GH_THING_DIR_NONE) {
        if (d.x < 0) {
            *dir = GH_THING_DIR_L;
        } else {
            *dir = GH_THING_DIR_R;
        }

        if (anim_frames->anim_frame[*dir][*mode][0].size()) {
            //
            // If we've changed mode/dir etc... then make sure the new mode
            // has sufficent anim frames.
            //
            if (frame >= anim_frames->anim_frame[*dir][*mode][0].size()) {
                frame = 0;
            }

            return (true);
        }

        if (d.y < 0) {
            *dir = GH_THING_DIR_U;
        } else {
            *dir = GH_THING_DIR_D;
        }

        if (anim_frames->anim_frame[*dir][*mode][0].size()) {
            //
            // If we've changed mode/dir etc... then make sure the new mode
            // has sufficent anim frames.
            //
            if (frame >= anim_frames->anim_frame[*dir][*mode][0].size()) {
                frame = 0;
            }

            return (true);
        }
    }

    //
    // If still nothing has been set, try a more generic mode.
    //
    *dir = GH_THING_DIR_ALL;
    if (anim_frames->anim_frame[*dir][*mode][0].size()) {
        //
        // If we've changed mode/dir etc... then make sure the new mode
        // has sufficent anim frames.
        //
        if (frame >= anim_frames->anim_frame[*dir][*mode][0].size()) {
            frame = 0;
        }

        return (true);
    }

    //
    // If still nothing, we're stuck.
    //
    *mode = GH_THING_MODE_NORMAL;
    if (anim_frames->anim_frame[*dir][*mode][0].size()) {
        //
        // If we've changed mode/dir etc... then make sure the new mode
        // has sufficent anim frames.
        //
        if (frame >= anim_frames->anim_frame[*dir][*mode][0].size()) {
            frame = 0;
        }

        return (true);
    }

    GH_WARN("no gfx for thing " + name);

    return (false);
}
