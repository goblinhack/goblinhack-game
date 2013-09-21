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

    $Id: gfx.cpp 194 2006-10-08 23:18:11Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_term.h>

//
// Are the 2 things overlapping?
//
bool gh_thing_gfx_is_obstacle_internal (gh_thing *a, gh_thing *b)
{
    GH_FUNCTION()

    const gh_anim_frame *anim_a = a->get_anim();
    const gh_anim_frame *anim_b = b->get_anim();

    bool collision = false;

    if (GH_DISTANCE(a->at.x, a->at.y, b->at.x, b->at.y) >
        max((anim_a->pix_max_w + anim_b->pix_max_w)/2,
            (anim_a->pix_max_h + anim_b->pix_max_h)/2)) {
        return (false);
    }

    //
    // Quick check to see if any corners overlap. If so, we dig deeper.
    //
    if ((a->at.x + anim_a->obs_pix_minv.x >= b->at.x + anim_b->obs_pix_minv.x) &&
        (a->at.x + anim_a->obs_pix_minv.x <= b->at.x + anim_b->obs_pix_maxv.x) &&
        (a->at.y + anim_a->obs_pix_minv.y >= b->at.y + anim_b->obs_pix_minv.y) &&
        (a->at.y + anim_a->obs_pix_minv.y <= b->at.y + anim_b->obs_pix_maxv.y)) {
        collision = true;
    } else
                        
    if ((a->at.x + anim_a->obs_pix_maxv.x >= b->at.x + anim_b->obs_pix_minv.x) &&
        (a->at.x + anim_a->obs_pix_maxv.x <= b->at.x + anim_b->obs_pix_maxv.x) &&
        (a->at.y + anim_a->obs_pix_minv.y >= b->at.y + anim_b->obs_pix_minv.y) &&
        (a->at.y + anim_a->obs_pix_minv.y <= b->at.y + anim_b->obs_pix_maxv.y)) {
        collision = true;
    } else

    if ((a->at.x + anim_a->obs_pix_maxv.x >= b->at.x + anim_b->obs_pix_minv.x) &&
        (a->at.x + anim_a->obs_pix_maxv.x <= b->at.x + anim_b->obs_pix_maxv.x) &&
        (a->at.y + anim_a->obs_pix_maxv.y >= b->at.y + anim_b->obs_pix_minv.y) &&
        (a->at.y + anim_a->obs_pix_maxv.y <= b->at.y + anim_b->obs_pix_maxv.y)) {
        collision = true;
    } else

    if ((a->at.x + anim_a->obs_pix_minv.x >= b->at.x + anim_b->obs_pix_minv.x) &&
        (a->at.x + anim_a->obs_pix_minv.x <= b->at.x + anim_b->obs_pix_maxv.x) &&
        (a->at.y + anim_a->obs_pix_maxv.y >= b->at.y + anim_b->obs_pix_minv.y) &&
        (a->at.y + anim_a->obs_pix_maxv.y <= b->at.y + anim_b->obs_pix_maxv.y)) {
        collision = true;
    } else

    //
    // Opposite direction, is thing a interecting thing b
    //
    if ((b->at.x + anim_b->obs_pix_minv.x >= a->at.x + anim_a->obs_pix_minv.x) &&
        (b->at.x + anim_b->obs_pix_minv.x <= a->at.x + anim_a->obs_pix_maxv.x) &&
        (b->at.y + anim_b->obs_pix_minv.y >= a->at.y + anim_a->obs_pix_minv.y) &&
        (b->at.y + anim_b->obs_pix_minv.y <= a->at.y + anim_a->obs_pix_maxv.y)) {
        collision = true;
    } else

    if ((b->at.x + anim_b->obs_pix_maxv.x >= a->at.x + anim_a->obs_pix_minv.x) &&
        (b->at.x + anim_b->obs_pix_maxv.x <= a->at.x + anim_a->obs_pix_maxv.x) &&
        (b->at.y + anim_b->obs_pix_minv.y >= a->at.y + anim_a->obs_pix_minv.y) &&
        (b->at.y + anim_b->obs_pix_minv.y <= a->at.y + anim_a->obs_pix_maxv.y)) {
        collision = true;
    } else

    if ((b->at.x + anim_b->obs_pix_maxv.x >= a->at.x + anim_a->obs_pix_minv.x) &&
        (b->at.x + anim_b->obs_pix_maxv.x <= a->at.x + anim_a->obs_pix_maxv.x) &&
        (b->at.y + anim_b->obs_pix_maxv.y >= a->at.y + anim_a->obs_pix_minv.y) &&
        (b->at.y + anim_b->obs_pix_maxv.y <= a->at.y + anim_a->obs_pix_maxv.y)) {
        collision = true;
    } else

    if ((b->at.x + anim_b->obs_pix_minv.x >= a->at.x + anim_a->obs_pix_minv.x) &&
        (b->at.x + anim_b->obs_pix_minv.x <= a->at.x + anim_a->obs_pix_maxv.x) &&
        (b->at.y + anim_b->obs_pix_maxv.y >= a->at.y + anim_a->obs_pix_minv.y) &&
        (b->at.y + anim_b->obs_pix_maxv.y <= a->at.y + anim_a->obs_pix_maxv.y)) {
        collision = true;
    }

    if (!collision) {
        return (false);
    }

    //
    // How many pixels are there per character cell? Both things should
    // share this
    //
    const float pix_ch_w = GH_RES_CHAR;
    const float pix_ch_h = GH_RES_CHAR;

    //
    // For each character in the source thing
    //
    for (int w = 0; w < anim_a->width; w++) {
        float x1 = a->at.x - (anim_a->pix_max_w / 2) + (w * pix_ch_w);

        for (int h = 0; h < anim_a->height; h++) {

            //
            // Only consider cells that are "solid"
            //
            if (!anim_a->cell[anim_a->width * h + w].obstacle_mask) {
                continue;
            }

            float y1 = a->at.y - (anim_a->pix_max_h / 2) + (h * pix_ch_h);

            //
            // For each corner of this cell, look for a collision.
            //
            for (float dx = 0; dx <= pix_ch_w; dx += pix_ch_w) {
                for (float dy = 0; dy <= pix_ch_h; dy += pix_ch_h) {
                    float x = x1 + dx;
                    float y = y1 + dy;

                    //
                    // Is this corner of a cell inside the other thing?
                    //
                    if ((x >= b->at.x + anim_b->obs_pix_minv.x) &&
                        (x <= b->at.x + anim_b->obs_pix_maxv.x) &&
                        (y >= b->at.y + anim_b->obs_pix_minv.y) &&
                        (y <= b->at.y + anim_b->obs_pix_maxv.y)) {

                        float x2 = b->at.x - (anim_b->pix_max_w / 2);
                        float y2 = b->at.y - (anim_b->pix_max_h / 2);

                        //
                        // It is. Which cell does it live in? Subtract the co-ord
                        // in thing a from the top left of thing b to find out
                        //
                        int col_x = (int)((x - x2) / pix_ch_w);
                        int col_y = (int)((y - y2) / pix_ch_h);

                        if ((col_x < 0) || (col_x >= anim_b->width)) {
                            continue;
                        }

                        if ((col_y < 0) || (col_y >= anim_b->height)) {
                            continue;
                        }

                        if (anim_b->cell[anim_b->width * col_y + col_x].obstacle_mask) {
                            return (true);
                        }
                    }
                }
            }
        }
    }

    return (false);
}

//
// Are the 2 things overlapping?
//
bool gh_thing_gfx_can_touch_internal (gh_thing *a, gh_thing *b)
{
    GH_FUNCTION()

    const gh_anim_frame *anim_a = a->get_anim();
    const gh_anim_frame *anim_b = b->get_anim();

    if (GH_DISTANCE(a->at.x, a->at.y, b->at.x, b->at.y) >
        max((anim_a->pix_max_w + anim_b->pix_max_w)/2,
            (anim_a->pix_max_h + anim_b->pix_max_h)/2)) {
        return (false);
    }

    bool can_touch = false;

    //
    // Quick check to see if any corners overlap. If so, we dig deeper.
    //
    if ((a->at.x + anim_a->pix_minv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_minv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_minv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_minv.y <= b->at.y + anim_b->pix_maxv.y)) {
        can_touch = true;
    } else
                        
    if ((a->at.x + anim_a->pix_maxv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_maxv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_minv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_minv.y <= b->at.y + anim_b->pix_maxv.y)) {
        can_touch = true;
    } else

    if ((a->at.x + anim_a->pix_maxv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_maxv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_maxv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_maxv.y <= b->at.y + anim_b->pix_maxv.y)) {
        can_touch = true;
    } else

    if ((a->at.x + anim_a->pix_minv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_minv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_maxv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_maxv.y <= b->at.y + anim_b->pix_maxv.y)) {
        can_touch = true;
    } else

    //
    // Opposite direction, is thing a interecting thing b
    //
    if ((b->at.x + anim_b->pix_minv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_minv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_minv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_minv.y <= a->at.y + anim_a->pix_maxv.y)) {
        can_touch = true;
    } else

    if ((b->at.x + anim_b->pix_maxv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_maxv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_minv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_minv.y <= a->at.y + anim_a->pix_maxv.y)) {
        can_touch = true;
    } else

    if ((b->at.x + anim_b->pix_maxv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_maxv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_maxv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_maxv.y <= a->at.y + anim_a->pix_maxv.y)) {
        can_touch = true;
    } else

    if ((b->at.x + anim_b->pix_minv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_minv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_maxv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_maxv.y <= a->at.y + anim_a->pix_maxv.y)) {
        can_touch = true;
    }

    if (!can_touch) {
        return (false);
    }

    //
    // How many pixels are there per character cell? Both things should
    // share this
    //
    const float pix_ch_w = GH_RES_CHAR;
    const float pix_ch_h = GH_RES_CHAR;

    //
    // For each character in the source thing
    //
    for (int w = 0; w < anim_a->width; w++) {
        float x1 = a->at.x - (anim_a->pix_max_w / 2) + (w * pix_ch_w);

        for (int h = 0; h < anim_a->height; h++) {

            //
            // Only consider cells that are "solid"
            //
            if (!anim_a->cell[anim_a->width * h + w].can_touch_mask) {
                continue;
            }

            float y1 = a->at.y - (anim_a->pix_max_h / 2) + (h * pix_ch_h);

            //
            // For each corner of this cell, look for a can_touch.
            //
            for (float dx = 0; dx <= pix_ch_w; dx += pix_ch_w) {
                for (float dy = 0; dy <= pix_ch_h; dy += pix_ch_h) {
                    float x = x1 + dx;
                    float y = y1 + dy;

                    //
                    // Is this corner of a cell inside the other thing?
                    //
                    if ((x >= b->at.x + anim_b->pix_minv.x) &&
                        (x <= b->at.x + anim_b->pix_maxv.x) &&
                        (y >= b->at.y + anim_b->pix_minv.y) &&
                        (y <= b->at.y + anim_b->pix_maxv.y)) {

                        float x2 = b->at.x - (anim_b->pix_max_w / 2);
                        float y2 = b->at.y - (anim_b->pix_max_h / 2);

                        //
                        // It is. Which cell does it live in? Subtract the co-ord
                        // in thing a from the top left of thing b to find out
                        //
                        int col_x = (int)((x - x2) / pix_ch_w);
                        int col_y = (int)((y - y2) / pix_ch_h);

                        if ((col_x < 0) || (col_x >= anim_b->width)) {
                            continue;
                        }

                        if ((col_y < 0) || (col_y >= anim_b->height)) {
                            continue;
                        }

                        if (anim_b->cell[anim_b->width * col_y + col_x].can_touch_mask) {
                            return (true);
                        }
                    }
                }
            }
        }
    }

    return (false);
}

//
// Are the 2 things biting? hit mask overlaps touch mask?
//
bool gh_thing_gfx_can_hit_internal (gh_thing *a, gh_thing *b)
{
    GH_FUNCTION()

    const gh_anim_frame *anim_a = a->get_anim();
    const gh_anim_frame *anim_b = b->get_anim();

    bool collision = false;

    if (GH_DISTANCE(a->at.x, a->at.y, b->at.x, b->at.y) >
        max((anim_a->pix_max_w + anim_b->pix_max_w)/2,
            (anim_a->pix_max_h + anim_b->pix_max_h)/2)) {
        return (false);
    }

    //
    // Quick check to see if any corners overlap. If so, we dig deeper.
    //
    if ((a->at.x + anim_a->pix_minv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_minv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_minv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_minv.y <= b->at.y + anim_b->pix_maxv.y)) {
        collision = true;
    } else
                        
    if ((a->at.x + anim_a->pix_maxv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_maxv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_minv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_minv.y <= b->at.y + anim_b->pix_maxv.y)) {
        collision = true;
    } else

    if ((a->at.x + anim_a->pix_maxv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_maxv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_maxv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_maxv.y <= b->at.y + anim_b->pix_maxv.y)) {
        collision = true;
    } else

    if ((a->at.x + anim_a->pix_minv.x >= b->at.x + anim_b->pix_minv.x) &&
        (a->at.x + anim_a->pix_minv.x <= b->at.x + anim_b->pix_maxv.x) &&
        (a->at.y + anim_a->pix_maxv.y >= b->at.y + anim_b->pix_minv.y) &&
        (a->at.y + anim_a->pix_maxv.y <= b->at.y + anim_b->pix_maxv.y)) {
        collision = true;
    } else

    //
    // Opposite direction, is thing a interecting thing b
    //
    if ((b->at.x + anim_b->pix_minv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_minv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_minv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_minv.y <= a->at.y + anim_a->pix_maxv.y)) {
        collision = true;
    } else

    if ((b->at.x + anim_b->pix_maxv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_maxv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_minv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_minv.y <= a->at.y + anim_a->pix_maxv.y)) {
        collision = true;
    } else

    if ((b->at.x + anim_b->pix_maxv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_maxv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_maxv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_maxv.y <= a->at.y + anim_a->pix_maxv.y)) {
        collision = true;
    } else

    if ((b->at.x + anim_b->pix_minv.x >= a->at.x + anim_a->pix_minv.x) &&
        (b->at.x + anim_b->pix_minv.x <= a->at.x + anim_a->pix_maxv.x) &&
        (b->at.y + anim_b->pix_maxv.y >= a->at.y + anim_a->pix_minv.y) &&
        (b->at.y + anim_b->pix_maxv.y <= a->at.y + anim_a->pix_maxv.y)) {
        collision = true;
    }

    if (!collision) {
        return (false);
    }

    //
    // How many pixels are there per character cell? Both things should
    // share this
    //
    const float pix_ch_w = GH_RES_CHAR;
    const float pix_ch_h = GH_RES_CHAR;

    //
    // For each character in the source thing
    //
    for (int w = 0; w < anim_a->width; w++) {
        float x1 = a->at.x - (anim_a->pix_max_w / 2) + (w * pix_ch_w);

        for (int h = 0; h < anim_a->height; h++) {

            //
            // Only consider cells that are "solid"
            //
            if (!anim_a->cell[anim_a->width * h + w].can_hit_mask) {
                continue;
            }

            float y1 = a->at.y - (anim_a->pix_max_h / 2) + (h * pix_ch_h);

            //
            // For each corner of this cell, look for a collision.
            //
            for (float dx = 0; dx <= pix_ch_w; dx += pix_ch_w) {
                for (float dy = 0; dy <= pix_ch_h; dy += pix_ch_h) {
                    float x = x1 + dx;
                    float y = y1 + dy;

                    //
                    // Is this corner of a cell inside the other thing?
                    //

                    if ((x >= b->at.x + anim_b->pix_minv.x) &&
                        (x <= b->at.x + anim_b->pix_maxv.x) &&
                        (y >= b->at.y + anim_b->pix_minv.y) &&
                        (y <= b->at.y + anim_b->pix_maxv.y)) {

                        float x2 = b->at.x - (anim_b->pix_max_w / 2);
                        float y2 = b->at.y - (anim_b->pix_max_h / 2);

                        //
                        // It is. Which cell does it live in? Subtract the co-ord
                        // in thing a from the top left of thing b to find out
                        //
                        int col_x = (int)((x - x2) / pix_ch_w);
                        int col_y = (int)((y - y2) / pix_ch_h);

                        if ((col_x < 0) || (col_x >= anim_b->width)) {
                            continue;
                        }

                        if ((col_y < 0) || (col_y >= anim_b->height)) {
                            continue;
                        }

                        if (anim_b->cell[anim_b->width * col_y + col_x].can_touch_mask) {
                            return (true);
                        }
                    }
                }
            }
        }
    }

    return (false);
}
