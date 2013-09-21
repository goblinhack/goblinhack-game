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

    $Id: map.cpp 943 2008-06-08 23:58:42Z Ghack $
*/

#include <ghack.h>
#include <gh_maze.h>
#include <gh_fractal.h>

static int GH_MAP_WIDTH                 = 100;
static int GH_MAP_HEIGHT                = 100;

//
// Generate a maze!
//
void gh_maze::landscape_gen (void)
{
    GH_FUNCTION()

    GH_LOG("Generate new landscape");

redo:
    maze_w = GH_MIN(GH_MAP_WIDTH + level*2, GH_MAX_MAZE_SIZE - 2);
    maze_h = GH_MIN(GH_MAP_HEIGHT + level*2, GH_MAX_MAZE_SIZE - 2);
    maze_w = GH_MAX(maze_w, GH_MIN_MAZE_SIZE);
    maze_h = GH_MAX(maze_h, GH_MIN_MAZE_SIZE);

    gh_fractal * f = new gh_fractal(
                            maze_w,
                            maze_h,
                            100, // max starting height
                            rand(), // seed
                            20, // stdev
                            0.85);

    maze_reset();
    memset(map_save, ' ', sizeof(map_save));

    for (int x = 2; x < maze_w-2; x++) {
        for (int y = 2; y < maze_h-2; y++) {

            float depth = (*f->map)[x][y];

            gh_point3d where(
                         (int)((GH_RES * (float)x) + (GH_RES / 2)),
                         (int)((GH_RES * (float)y) + (GH_RES / 2)),
                         0);

            float gradient = max(fabs((*f->map)[x-2][y] - (*f->map)[x+2][y]),
                                 fabs((*f->map)[x][y-2] - (*f->map)[x][y+2]));

            if (depth > 120) {
                if (gradient < 1) {
                    map[x][y] = GH_MAP_DIRT;
                } else if (gradient < 2) {
                    map[x][y] = GH_MAP_LAVA;
                } else {
                    map[x][y] = GH_MAP_ROCK;
                }
            } else if (depth > 70) {
                if (gradient < 1) {
                    map[x][y] = GH_MAP_WATER1;
                } else if (gradient < 2) {
                    map[x][y] = GH_MAP_GRASS;
                } else if (gradient < 20) {
                    map[x][y] = GH_MAP_DIRT;
                } else if (gradient < 30) {
                    map[x][y] = GH_MAP_ROCK_CRUMBLES;
                } else {
                    map[x][y] = GH_MAP_ROCK;
                }
            } else if (depth > 10) {
                if (gradient < 20) {
                    map[x][y] = GH_MAP_GRASS;
                } else if (gradient < 30) {
                    map[x][y] = GH_MAP_TREE;
                } else if (gradient < 40) {
                    map[x][y] = GH_MAP_DIRT;
                } else {
                    map[x][y] = GH_MAP_ROCK_CRUMBLES;
                }
            } else if (depth > 0) {
                map[x][y] = GH_MAP_WATER1;
            } else if (depth > -5) {
                map[x][y] = GH_MAP_WATER2;
            } else if (depth > -10) {
                map[x][y] = GH_MAP_WATER3;
            }
        }
    }

    delete f;

    for (int y=0; y < maze_h; y++) {
        map[y][0] = map[y][maze_w-1] = GH_MAP_ROCK;
        map[y][0] = map[y][maze_w-2] = GH_MAP_ROCK;
        map[y][0] = map[y][maze_w-3] = GH_MAP_ROCK;
        map[y][0] = map[y][maze_w-4] = GH_MAP_ROCK;
        map[y][0] = map[y][maze_w-5] = GH_MAP_ROCK;
        map[y][0] = map[y][0] = GH_MAP_ROCK;
        map[y][0] = map[y][1] = GH_MAP_ROCK;
        map[y][0] = map[y][2] = GH_MAP_ROCK;
        map[y][0] = map[y][3] = GH_MAP_ROCK;
        map[y][0] = map[y][4] = GH_MAP_ROCK;
    }

    for (int x=0; x < maze_w; x++) {
        map[0][x] = map[maze_h-1][x] = GH_MAP_ROCK;
        map[0][x] = map[maze_h-2][x] = GH_MAP_ROCK;
        map[0][x] = map[maze_h-3][x] = GH_MAP_ROCK;
        map[0][x] = map[maze_h-4][x] = GH_MAP_ROCK;
        map[0][x] = map[maze_h-5][x] = GH_MAP_ROCK;
        map[0][x] = map[0][x] = GH_MAP_ROCK;
        map[0][x] = map[1][x] = GH_MAP_ROCK;
        map[0][x] = map[2][x] = GH_MAP_ROCK;
        map[0][x] = map[3][x] = GH_MAP_ROCK;
        map[0][x] = map[4][x] = GH_MAP_ROCK;
    }

    //
    // More of everything in landscapes.
    //
    place_monst();
    place_monst();
    place_monst();
    place_monst();
    place_monst();
    place_monst();
    place_monst();
    place_monst();
    place_treasure();
    place_potion();

    place_obstacle(GH_MAP_START);
    place_obstacle(GH_MAP_EXIT);

    maze_print();

    if (!maze_test()) {
        /*
         * No solution for the maze!
         */
        goto redo;
    }
}

