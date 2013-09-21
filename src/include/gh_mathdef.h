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

    $Id: gh_mathdef.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_MATHDEF_H_
#define _GH_MATHDEF_H_

#include <math.h>

#undef GH_SQUARED
#define GH_SQUARED(x) ((x) * (x))

#undef GH_DISTANCE
#define GH_DISTANCE(xa,ya,xb,yb) \
    sqrt((float)(GH_SQUARED((xb)-(xa))+GH_SQUARED((yb)-(ya))))

#undef GH_DISTANCE3f
#define GH_DISTANCE3f(xa,ya,za,xb,yb,zb) \
    sqrt((float)(GH_SQUARED((xb)-(xa))+\
                 GH_SQUARED((yb)-(ya))+\
                 GH_SQUARED((zb)-(za))))

#undef GH_DIFFERENCE
#define GH_DIFFERENCE(x,y) (((y)>(x)) ? ((y)-(x)) : ((x)-(y)))

#undef M_360
#define M_360       (2.0*3.14159265358979323846)  /* 2*pi */

#undef M_270
#define M_270       (M_180 + M_90)

#undef M_180
#define M_180       3.14159265358979323846  /* pi */

#undef M_90
#define M_90        1.57079632679489661923  /* pi/2 */

#undef M_45
#define M_45        0.78539816339744830962  /* pi/4 */

#endif /* _GH_MATHDEF_H_ */
