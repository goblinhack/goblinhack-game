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

    $Id: rand.cpp 750 2008-03-22 20:48:27Z goblinhack $
*/

#include <ghack.h>
#include <gh_math.h>

float GH_FSIN[GH_RAD_MAX];
float GH_FCOS[GH_RAD_MAX];

void gh_math::fsin_init (void)
{
    for (int16 i = 0; i < GH_RAD_MAX; i++) {
        GH_FSIN[i] = (float)sin(GH_RAD_STEP * (float) i);
    }
}

void gh_math::fcos_init (void)
{
    for (int16 i = 0; i < GH_RAD_MAX; i++) {
        GH_FCOS[i] = (float)cos(GH_RAD_STEP * (float) i);
    }
}
