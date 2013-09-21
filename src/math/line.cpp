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

    $Id: line.cpp 621 2008-01-04 15:25:24Z goblinhack $
*/

#include <ghack.h>
#include <gh_math.h>

static vector<gh_point3d> line_vec_int (gh_point3d a, gh_point3d b, int flag)
{
    GH_FUNCTION()

    int temp, dx, dy, tdy, dydx, p, x, y, i;

    if (a.x > b.x) {
        temp = a.x; a.x = b.x; b.x = temp;
        temp = a.y; a.y = b.y; b.y = temp;
    }

    dx = b.x - a.x;
    dy = b.y - a.y;
    tdy = 2 * dy;
    dydx = tdy - 2 * dx;
    p = tdy - dx;   /* p0 = 2dy - dx */
    x = a.x;
    y = a.y;

    vector<gh_point3d> r;

    switch (flag) {
        case 0: r.push_back(gh_point3d(x, y, a.z)); break;
        case 1: r.push_back(gh_point3d(y, x, a.z)); break;
        case 2: r.push_back(gh_point3d(y, -x, a.z)); break;
        case 3: r.push_back(gh_point3d(x, -y, a.z)); break;
    }

    const int step = 10;
    int s = 0;

    for (i = 1; i <= dx; i++){

        x++;
        if (p < 0) p += tdy;
        else { p += dydx; y++; }

        if (s++ != step) {
            continue;
        }
        s = 0;

        switch (flag) {
            case 0: r.push_back(gh_point3d(x, y, a.z)); break;
            case 1: r.push_back(gh_point3d(y, x, a.z)); break;
            case 2: r.push_back(gh_point3d(y, -x, a.z)); break;
            case 3: r.push_back(gh_point3d(x, -y, a.z)); break;
        }
    }

    return (r);
}

vector<gh_point3d> gh_math::line_vec (gh_point3d a, gh_point3d b)
{
    GH_FUNCTION()

    int x0 = a.x;
    int x1 = b.x;
    int y0 = a.y;
    int y1 = b.y;
    int z0 = a.z;
    int z1 = b.z;

    float slope = 100;

    if (x0 != x1) slope = (y1 - y0) * 1.0 / (x1 - x0);

    /* call helper function depending on the line slope */
    if ((0 <= slope) && (slope <= 1))
        return line_vec_int(
                gh_point3d(x0, y0, z0),
                gh_point3d(x1, y1, z1), 0);
    else if ((-1 <= slope) && (slope <= 0))
        return line_vec_int(
                gh_point3d(x0, -y0, z0),
                gh_point3d(x1, -y1, z1), 3);
    else if (slope > 1)
        return line_vec_int(
                gh_point3d(y0, x0, z0),
                gh_point3d(y1, x1, z1), 1);
    else
        return line_vec_int(
                gh_point3d(-y0, x0, z0),
                gh_point3d(-y1, x1, z1), 2);
}
