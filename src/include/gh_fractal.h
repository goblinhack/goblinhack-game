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

    $Id: maze.cpp 479 2007-03-12 02:13:38Z goblinhack $
*/

#ifndef _GH_FRACTAL_H_
#define _GH_FRACTAL_H_

#include <gh_array2d.h>

class gh_fractal {
public:
    int width;
    int height;
    int max_vertical_height;
    int seed;
    float stdev;
    float stdev_shrink_factor;

    GH_ARRAY2D(float) *map;

    gh_fractal(int width,
               int height,
               int max_vertical_height,
               int seed,
               float stdev,
               float stdev_shrink_factor);

private:
    vector< vector<int> > boxes;

    void make_boxes(int x1,
                    int y1,
                    int x3,
                    int y3,
                    unsigned int level);

    void make_landscape(int seed,
                        float stdev,
                        float stdev_shrink_factor);

    void make_map(float stdev, float stdev_shrink_factor);
};

#endif
