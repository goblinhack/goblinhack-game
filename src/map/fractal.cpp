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

#include <ghack.h>
#include <gh_fractal.h>
#include <gh_math.h>

//
// Break a box into four more smaller boxes and store them at the next
// recurse level.
//
// (x1,y1)-----(x2,y1)-----(x3,y1)
//    |           |           |
//    |           |           |
// (x1,y2)-----(x2,y2)-----(x3,y2)
//    |           |           |
//    |           |           |
// (x1,y3)-----(x2,y3)-----(x3,y3)
//
//
gh_inline void gh_fractal::make_boxes (int x1,
                             int y1,
                             int x3,
                             int y3,
                             unsigned int level)
{
    boxes[level].push_back(x1);
    boxes[level].push_back(y1);
    boxes[level].push_back(x3);
    boxes[level].push_back(y3);

    if (x3 - x1 == 1) {
        return;
    }

    if (y3 - y1 == 1) {
        return;
    }

    level++;

    if (level >= boxes.size()) {
        return;
    }

    int x2 = ((x3 - x1) / 2) + x1;
    int y2 = ((y3 - y1) / 2) + y1;

    make_boxes(x1, y1, x2, y2, level);
    make_boxes(x2, y1, x3, y2, level);
    make_boxes(x1, y2, x2, y3, level);
    make_boxes(x2, y2, x3, y3, level);
}

//
// Walk the current box (and then the next level below) and assign heights
// to the corners. We avoid recursion here and do each level one at a time
// to avoid any boxing of the landscape.
//
// (x1,y1)-----(x2,y1)-----(x3,y1)
//    |           |           |
//    |           |           |
// (x1,y2)-----(x2,y2)-----(x3,y2)
//    |           |           |
//    |           |           |
// (x1,y3)-----(x2,y3)-----(x3,y3)
//
//

gh_inline void gh_fractal::make_map (float stdev, float stdev_shrink_factor)
{
    vector<int>::iterator s;

    for (unsigned int l = 0; l < boxes.size(); l++) {
        for (s = boxes[l].begin(); s != boxes[l].end(); ) {
            int x1 = *s++;
            int y1 = *s++;
            int x3 = *s++;
            int y3 = *s++;

            int x2 = ((x3 - x1) / 2) + x1;
            int y2 = ((y3 - y1) / 2) + y1;

            (*map)[x2][y1] =
                gh_math::gauss((((*map)[x1][y1] + (*map)[x3][y1]) / 2.0), stdev);
            (*map)[x1][y2] =
                gh_math::gauss((((*map)[x1][y1] + (*map)[x1][y3]) / 2.0), stdev);
            (*map)[x2][y3] =
                gh_math::gauss((((*map)[x1][y3] + (*map)[x3][y3]) / 2.0), stdev);
            (*map)[x3][y2] =
                gh_math::gauss((((*map)[x3][y1] + (*map)[x3][y3]) / 2.0), stdev);
            (*map)[x2][y2] =
                gh_math::gauss((((*map)[x1][y1] + (*map)[x3][y1] +
                        (*map)[x1][y3] + (*map)[x3][y3]) / 4.0), stdev);
        }

        stdev *= stdev_shrink_factor;

        if (!(int)stdev) {
            stdev = 1.0;
        }
    }
}

gh_inline void gh_fractal::make_landscape (int seed,
                                           float stdev,
                                           float stdev_shrink_factor)
{
    srand(seed);

    (*map)[0][0]                        = rand() % max_vertical_height;
    (*map)[width - 1][0]                = rand() % max_vertical_height;
    (*map)[0][height - 1]               = rand() % max_vertical_height;
    (*map)[width - 1][height - 1]       = rand() % max_vertical_height;

    make_map(stdev, stdev_shrink_factor);

#ifdef DEBUG
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if ((*map)[x][y] > 120) {
                cout << "^";
            } else if ((*map)[x][y] > 100) {
                cout << "&";
            } else if ((*map)[x][y] > 90) {
                cout << "=";
            } else if ((*map)[x][y] > 80) {
                cout << ":";
            } else if ((*map)[x][y] > 50) {
                cout << ".";
            } else if ((*map)[x][y] < 0) {
                cout << "w";
            } else if ((*map)[x][y] < -10) {
                cout << "W";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
#endif
}

gh_fractal::gh_fractal (int width,
                        int height,
                        int max_vertical_height,
                        int seed,
                        float stdev,
                        float stdev_shrink_factor)
{
    boxes.resize(20);

    this->width = width;
    this->height = height;
    this->max_vertical_height = max_vertical_height;
    this->seed = seed;
    this->stdev = stdev;
    this->stdev_shrink_factor = stdev_shrink_factor;

    float tmp = 0.0;

    map = gh_array2d(width, height, tmp);

    make_boxes(0, 0, /* top left */
               width - 1, height - 1, /* bottom right */
               0 /* level */);

    make_landscape(seed, stdev, stdev_shrink_factor);
}
