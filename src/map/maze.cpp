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

    $Id: maze.cpp 1240 2009-07-10 02:20:51Z Ghack $
*/

#include <ghack.h>
#include <gh_maze.h>
#include <gh_math.h>
#include <gh_room.h>
#include <SDL/SDL.h>
#include <hacklib.h>

#define nDEBUG_MAZE

static int GH_MAP_ROOM_AMOUNT           = 13;
static int GH_MAP_ROOM_W_AMOUNT         = 17;
static int GH_MAP_ROOM_H_AMOUNT         = 17;
static int GH_MAP_CORRIDOR_LEN_AMOUNT   = 40;
static int GH_MAP_TREASURE_AMOUNT       = 10;
static int GH_MAP_POTION_AMOUNT         = 5;
static int GH_MAP_FOOD_AMOUNT           = 5;
static int GH_MAP_DOORS_AMOUNT          = 12;
static int GH_MAP_TRAPS_AMOUNT          = 3;
static int GH_MAP_MONST_AMOUNT          = 6;
static int GH_MAP_WIDTH                 = 70;
static int GH_MAP_HEIGHT                = 70;
static int GH_MAP_BORDER_PADDING        = 1;

//
// Prototypes
//
static int myrand(int r);
static double myfrand(double r);

//
// Generate a random number in the range 0 .. r - 1
//
static int myrand (int r)
{
    if (!r) {
        return (0);
    }

    return (rand() % r);
}

//
// Generate a floating point random in the range 0 .. r
//
static double myfrand (double r)
{
    GH_FUNCTION()

    return ((r / 10000.0) * (double)(rand() % 10000));
}

void gh_maze::do_room_line (int x0_in, int y0_in, int x1_in, int y1_in, int flag_in)
{
    GH_FUNCTION()

    double temp;
    double dx;
    double dy;
    double tdy;
    double dydx;
    double p;
    double x;
    double y;
    double i;

    double x0 = x0_in;
    double y0 = y0_in;
    double x1 = x1_in;
    double y1 = y1_in;
    double flag = flag_in;

    if (x0 > x1) {
        temp = x0;
        x0 = x1;
        x1 = temp;

        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    tdy = 2.0 * dy;
    dydx = tdy - (2.0 * dx);

    p = tdy - dx;
    x = x0;
    y = y0;

    if (flag == 0) {
        room[(int)x][(int)y] = GH_MAP_ROOM_POSSIBLE;
    } else if (flag == 1) {
        room[(int)y][(int)x] = GH_MAP_ROOM_POSSIBLE;
    } else if (flag == 2) {
        room[(int)y][(int)-x] = GH_MAP_ROOM_POSSIBLE;
    } else if (flag == 3) {
        room[(int)x][(int)-y] = GH_MAP_ROOM_POSSIBLE;
    }

    for (i = 1; i <= dx; i++){
        x++;

        if (p < 0) {
            p += tdy;
        } else {
            p += dydx;
            y++;
        }

        if (flag == 0) {
            room[(int)x][(int)y] = GH_MAP_ROOM_POSSIBLE;
        } else if (flag == 1) {
            room[(int)y][(int)x] = GH_MAP_ROOM_POSSIBLE;
        } else if (flag == 2) {
            room[(int)y][(int)-x] = GH_MAP_ROOM_POSSIBLE;
        } else if (flag == 3) {
            room[(int)x][(int)-y] = GH_MAP_ROOM_POSSIBLE;
        }
    }
}

void gh_maze::room_line (int x0, int y0, int x1, int y1)
{
    GH_FUNCTION()

    double slope = 100.0;

    if (x0 != x1) {
        slope = (y1 - y0) * (1.0 / (x1 - x0));
    }

    if ((0 <= slope) && (slope <= 1)) {
        do_room_line(x0, y0, x1, y1, 0);
    } else if ((-1 <= slope) && (slope <= 0)) {
        do_room_line(x0, -y0, x1, -y1, 3);
    } else if (slope > 1) {
        do_room_line(y0, x0, y1, x1, 1);
    } else {
        do_room_line(-y0, x0, -y1, x1, 2);
    }
}

//
// initialize the maze.
//
void gh_maze::maze_clean (void)
{
    GH_FUNCTION()

    memset(map, ' ', sizeof(map));
}

//
// initialize the maze.
//
void gh_maze::maze_reset (void)
{
    GH_FUNCTION()

    maze_clean();

    memset(map_searched, 0, sizeof(map_searched));
    memset(map_obstacle, 0, sizeof(map_obstacle));
    memset(map_obj, 0, sizeof(map_obj));
    memset(maze_room_info, 0, sizeof(maze_room_info));
    memset(room_number_map, 0, sizeof(room_number_map));

    memset(rooms, 0, sizeof(rooms));
}

//
// Add a maze border that the player should never be able to get through.
//
void gh_maze::maze_border (void)
{
    GH_FUNCTION()

    for (int y = 0; y < maze_h; y++) {
        map[0][y] = GH_MAP_BORDER;
        map[1][y] = GH_MAP_BORDER;
        map[2][y] = GH_MAP_BORDER;
        map[3][y] = GH_MAP_BORDER;
        map[maze_w - 1][y] = GH_MAP_BORDER;
        map[maze_w - 2][y] = GH_MAP_BORDER;
        map[maze_w - 3][y] = GH_MAP_BORDER;
        map[maze_w - 4][y] = GH_MAP_BORDER;
    }

    for (int x = 0; x < maze_w; x++) {
        map[x][0] = GH_MAP_BORDER;
        map[x][1] = GH_MAP_BORDER;
        map[x][2] = GH_MAP_BORDER;
        map[x][3] = GH_MAP_BORDER;
        map[x][maze_h - 1] = GH_MAP_BORDER;
        map[x][maze_h - 2] = GH_MAP_BORDER;
        map[x][maze_h - 3] = GH_MAP_BORDER;
        map[x][maze_h - 4] = GH_MAP_BORDER;
    }
}

//
// Add a padding area around the maze we've made so that walls of
// a room are never on the map boundaries.
//
void gh_maze::maze_pad (void)
{
    GH_FUNCTION()

    int border = GH_MAP_BORDER_PADDING;

    maze_backup();
    maze_clean();

    for (int y = 0; y < maze_h; y++) {
        for (int x = 0; x < maze_w; x++) {
            map[x + border][y + border] = map_save[x][y];
        }
    }

    maze_w += border * 4;
    maze_h += border * 4;
}

//
// Draw the corridors
//
void gh_maze::maze_corridors (int cx, int cy,
                              int tot_number_of_corridors,
                              int scale,
                              int split_chance)
{
    GH_FUNCTION()

    int c1x;
    int c1y;
    int dir;
    int corridor_length;
    int corridor_this_strip_len;
    int corridor_cant_move;
    int number_of_corridors;
    int corridor_start_x;
    int corridor_start_y;
    int tries = 0;

    number_of_corridors = 0;

    if (!tot_number_of_corridors) {
        tot_number_of_corridors = 13 + level / 8;
    }

    //
    // Choose a random start place somewhere in the middle.
    //
    if (!cx && !cy) {
        cx = myrand(maze_w - (maze_w/2)) + (maze_w/4);
        cy = myrand(maze_h - (maze_h/2)) + (maze_h/4);
    }

    corridor_start_x = cx;
    corridor_start_y = cy;

    while (number_of_corridors < tot_number_of_corridors) {
        c1x = cx;
        c1y = cx;
        dir = myrand(4);
        corridor_length = 0;
        corridor_this_strip_len = 0;
        corridor_cant_move = 0;

        int thick_x = 0;
        int thick_y = 0;
        int thick2_x = 0;
        int thick2_y = 0;

        if (myrand(100) < 10) {
            thick_x = -1;
            if (myrand(100) < 10) {
                thick2_x = -2;
            }
        }

        if (myrand(100) < 10) {
            thick_x = 1;
            if (myrand(100) < 10) {
                thick2_x = 2;
            }
        }

        if (myrand(100) < 10) {
            thick_y = 1;
            if (myrand(100) < 10) {
                thick2_y = 2;
            }
        }

        if (myrand(100) < 10) {
            thick_y = -1;
            if (myrand(100) < 10) {
                thick2_y = -2;
            }
        }

        //
        // Don't make them too long.
        //
        while (corridor_length < (GH_MAP_CORRIDOR_LEN_AMOUNT*scale)) {
            //
            // Save the old position for backtracking if we can't move
            //
            int old_cx = cx;
            int old_cy = cy;
            int old_c1x = c1x;
            int old_c1y = c1y;
            int ok_move = 1;

            //
            // Map the direction to a x,y delta
            //
            int dx = 0;
            int dy = 0;

            if (dir == 0) {
                dx = -2;
            }
            if (dir == 1) {
                dx = 2;
            }
            if (dir == 2) {
                dy = -2;
            }
            if (dir == 3) {
                dy = 2;
            }

            //
            // Move to the new position.
            //
            c1x = old_cx + (dx / 2);
            c1y = old_cy + (dy / 2);
            cx  = old_cx + dx;
            cy  = old_cy + dy;

            //
            // Check for hitting the edge of the maze.
            //
            if ((cx <= 3) || (cx >= maze_w - 4)) {
                ok_move = 0;
            }

            if ((cy <= 3) || (cy >= maze_h - 4)) {
                ok_move = 0;
            }

            if (myrand(100) < 20) {
                //
                // Check that our new corridor position isn't next to another
                // corridor.
                //
                if ((map[cx+1][cy-1] == GH_MAP_CORR) ||
                    (map[cx+0][cy-1] == GH_MAP_CORR) ||
                    (map[cx-1][cy-1] == GH_MAP_CORR) ||
                    (map[cx+1][cy+0] == GH_MAP_CORR) ||
                    (map[cx+0][cy+0] == GH_MAP_CORR) ||
                    (map[cx-1][cy+0] == GH_MAP_CORR) ||
                    (map[cx+1][cy+1] == GH_MAP_CORR) ||
                    (map[cx+0][cy+1] == GH_MAP_CORR) ||
                    (map[cx-1][cy+1] == GH_MAP_CORR)) {

                    ok_move = 0;
                }

                if ((map[cx+1][cy-1] == GH_MAP_DUSTY_CORR) ||
                    (map[cx+0][cy-1] == GH_MAP_DUSTY_CORR) ||
                    (map[cx-1][cy-1] == GH_MAP_DUSTY_CORR) ||
                    (map[cx+1][cy+0] == GH_MAP_DUSTY_CORR) ||
                    (map[cx+0][cy+0] == GH_MAP_DUSTY_CORR) ||
                    (map[cx-1][cy+0] == GH_MAP_DUSTY_CORR) ||
                    (map[cx+1][cy+1] == GH_MAP_DUSTY_CORR) ||
                    (map[cx+0][cy+1] == GH_MAP_DUSTY_CORR) ||
                    (map[cx-1][cy+1] == GH_MAP_DUSTY_CORR)) {

                    ok_move = 0;
                }
            }

            if (map[cx][cy] != ' ') {
                ok_move = 0;
            }

            if (ok_move) {
                corridor_length++;
                corridor_cant_move = 0;
                corridor_this_strip_len++;
            } else {
                //
                // Try another direction
                //
                corridor_cant_move++;
                corridor_this_strip_len = 0;

                //
                // Tried all directions?
                //
                if (corridor_cant_move > 5) {
                    break;
                }

                cx = old_cx;
                cy = old_cy;
                c1x = old_c1x;
                c1y = old_c1y;

                dir++;
                dir = dir % 4;
                continue;
            }

            map[cx][cy] = GH_MAP_DUSTY_CORR;
            map[c1x][c1y] = GH_MAP_DUSTY_CORR;

            if (map[cx-thick_x][cy-thick_y] == ' ') {
                map[cx-thick_x][cy-thick_y] = GH_MAP_DUSTY_CORR;
                if (map[c1x-thick_x][c1y-thick_y] == ' ') {
                    map[c1x-thick_x][c1y-thick_y] = GH_MAP_DUSTY_CORR;
                }
            }

            if (map[cx-thick2_x][cy-thick2_y] == ' ') {
                map[cx-thick2_x][cy-thick2_y] = GH_MAP_DUSTY_CORR;
                if (map[c1x-thick2_x][c1y-thick2_y] == ' ') {
                    map[c1x-thick2_x][c1y-thick2_y] = GH_MAP_DUSTY_CORR;
                }
            }


            //
            // We're still able to move then occasionaly change dir
            // when the corridor is long enough
            //
            corridor_this_strip_len++;
            if (corridor_this_strip_len > 5) {
                if (myrand(100) < 80) {
                    dir = myrand(4);
                    corridor_this_strip_len = 0;
                }
            }

            corridor_length++;
        }

        if (corridor_length > 0) {
            number_of_corridors++;

            map[cx][cy] = GH_MAP_DUSTY_CORR;
            map[c1x][c1y] = GH_MAP_DUSTY_CORR;
        }

        //
        // Start a new corridor. Sometimes we start from an existing
        // one and others, totally new - which we then have to decide
        // later if it can join up at all.
        //
        if (number_of_corridors > (tot_number_of_corridors / 3) * 2) {
            cx = (myrand(maze_w - 10) + 5);
            cy = (myrand(maze_h - 10) + 5);
        } else {
            int found_new_corridor_start = 0;

            for (int attempt = 0; attempt < 10 * maze_w * maze_h; attempt++) {
                cx = (myrand(maze_w - 10) + 5);
                cy = (myrand(maze_h - 10) + 5);

                if (map[cx][cy] == GH_MAP_DUSTY_CORR) {
                    found_new_corridor_start = 1;
                    break;
                }
            }

            if (!found_new_corridor_start) {
                cx = (myrand(maze_w - 10) + 5);
                cy = (myrand(maze_h - 10) + 5);
            }
        }

        tries++;

        if (tries > 100) {
            break;
        }
    }

    //
    // Flood fill the corridors made. Any left behind unconnected are
    // marked as "dusty"
    //
    maze_find_hidden_corridors(corridor_start_x, corridor_start_y);
}

//
// print the maze.
//
void gh_maze::maze_print (void)
{
    GH_FUNCTION()

#ifdef DEBUG_MAZE
    fprintf(stdout, "Maze map with obstacles only:\n");
    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");

    for (int y = 0; y < maze_h; y++) {
        fprintf(stdout, "#");
        for (int x = 0; x < maze_w; x++) {
            if (map_obstacle[x][y]) {
                fprintf(stdout, "%c", map_obstacle[x][y]);
            } else {
                fprintf(stdout, "%c", ' ');
            }
        }
        fprintf(stdout, "#\n");
    }

    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "Maze map with objects only:\n");
    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }

    fprintf(stdout, "\n");
    for (int y = 0; y < maze_h; y++) {
        fprintf(stdout, "#");
        for (int x = 0; x < maze_w; x++) {
            if (map_obj[x][y]) {
                fprintf(stdout, "%c", map_obj[x][y]);
            } else {
                fprintf(stdout, "%c", ' ');
            }
        }
        fprintf(stdout, "#\n");
    }

    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");

#endif
    fprintf(stdout, "Maze map:\n");
    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");

    for (int y = 0; y < maze_h; y++) {
        fprintf(stdout, "#");
        for (int x = 0; x < maze_w; x++) {
            fprintf(stdout, "%c", map[x][y]);
        }
        fprintf(stdout, "#\n");
    }

    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");

    fprintf(stdout, "Maze map with objects:\n");
    for (int x = 0; x < maze_w + 2; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");

    for (int y = 0; y < maze_h; y++) {
        fprintf(stdout, "#");
        for (int x = 0; x < maze_w; x++) {
            if (map_obstacle[x][y]) {
                fprintf(stdout, "%c", map_obstacle[x][y]);
            } else if (map_obj[x][y]) {
                fprintf(stdout, "%c", map_obj[x][y]);
            } else {
                fprintf(stdout, "%c", map[x][y]);
            }
        }
        fprintf(stdout, "#\n");
    }
}

//
// Trim all boring corridor deadends. Leave dusty corridors as-is for
// some interest.
//
int gh_maze::maze_remove_deadends (void)
{
    GH_FUNCTION()

    int got;

    got = 0;
    for (int y = 1; y < maze_h - 1; y++) {
        for (int x = 1; x < maze_w - 1; x++) {
            // ...
            // ..#
            // ...
            if ((map[x][y] == GH_MAP_CORR) &&
                (map[x-1][y] == ' ') &&
                (map[x][y-1] == ' ') &&
                (map[x][y+1] == ' ')) {
                map[x][y] = ' ';
                got++;
            }

            // ...
            // #..
            // ...
            if ((map[x][y] == GH_MAP_CORR) &&
                (map[x+1][y] == ' ') &&
                (map[x][y-1] == ' ') &&
                (map[x][y+1] == ' ')) {
                map[x][y] = ' ';
                got++;
            }

            // .#.
            // ...
            // ...
            if ((map[x][y] == GH_MAP_CORR) &&
                (map[x-1][y] == ' ') &&
                (map[x+1][y] == ' ') &&
                (map[x][y+1] == ' ')) {
                map[x][y] = ' ';
                got++;
            }

            // ...
            // ...
            // .#.
            if ((map[x][y] == GH_MAP_CORR) &&
                (map[x-1][y] == ' ') &&
                (map[x+1][y] == ' ') &&
                (map[x][y-1] == ' ')) {
                map[x][y] = ' ';
                got++;
            }
        }
    }

    return (got);
}

//
// Trim all corridor fragments that wholly live in a room
//
int gh_maze::maze_remove_corridor_fragments (void)
{
    GH_FUNCTION()

    int got;

    got = 0;
    for (int y = 1; y < maze_h - 1; y++) {
        for (int x = 1; x < maze_w - 1; x++) {
            if ((map[x][y] == GH_MAP_CORR) || (map[x][y] == GH_MAP_DUSTY_CORR)) {
                int neb;

                neb =  ((map[x-1][y] == GH_MAP_ROOM) || 
                        (map[x-1][y] == GH_MAP_ROOM_WALL));
                neb += ((map[x+1][y] == GH_MAP_ROOM) || 
                        (map[x+1][y] == GH_MAP_ROOM_WALL));
                neb += ((map[x][y-1] == GH_MAP_ROOM) || 
                        (map[x][y-1] == GH_MAP_ROOM_WALL));
                neb += ((map[x][y+1] == GH_MAP_ROOM) || 
                        (map[x][y+1] == GH_MAP_ROOM_WALL));

                if (neb >= 3) {
                    map[x][y] = GH_MAP_ROOM;
                    got++;
                }
            }
        }
    }

    return (got);
}

//
// Occasionally leave a deadend and stick a teleport on it. This is
// one way to get to dusty corridors.
//
int gh_maze::maze_add_deadend_teleports (void)
{
    GH_FUNCTION()

    int teleports = 0;

    for (int r=0; r<4; r++) {
        maze_rotate();

        for (int y = 1; y < maze_h - 1; y++) {
            for (int x = 1; x < maze_w - 1; x++) {
                // ...
                // ..#
                // ...
                if ((map[x][y-1] == ' ')               &&
                    (map[x][y]   == GH_MAP_CORR)       &&
                    (map[x][y+1] == ' ')               &&
                    (map[x+1][y-1] == ' ')             &&
                    (map[x+1][y]   == GH_MAP_CORR)     &&
                    (map[x+1][y+1] == ' ')             &&
                    (map[x-1][y-1] == ' ')             &&
                    (map[x-1][y]   == ' ')             &&
                    (map[x-1][y+1] == ' ')) {

                    if (myrand(100) < 10) {
                        map[x+1][y] = GH_MAP_TELEPORT;
                        teleports++;
                    }
                }

                // ...
                // ..#
                // ...
                if ((map[x][y-1] == ' ')                 &&
                    (map[x][y]   == GH_MAP_DUSTY_CORR)   &&
                    (map[x][y+1] == ' ')                 &&
                    (map[x+1][y-1] == ' ')               &&
                    (map[x+1][y]   == GH_MAP_DUSTY_CORR) &&
                    (map[x+1][y+1] == ' ')               &&
                    (map[x-1][y-1] == ' ')               &&
                    (map[x-1][y]   == ' ')               &&
                    (map[x-1][y+1] == ' ')) {

                    if (myrand(200) < 10) {
                        map[x+1][y] = GH_MAP_TELEPORT;
                        teleports++;
                    }
                }
            }
        }
    }

    return (teleports);
}

//
// Flood fill the corridors made. Any left behind unconnected are
// marked as "dusty"
//
void gh_maze::maze_find_hidden_corridors (int x, int y)
{
    GH_FUNCTION()

    map[x][y] = GH_MAP_CORR;

    if (map[x-1][y] == GH_MAP_DUSTY_CORR) {
        maze_find_hidden_corridors(x-1, y);
    }
    if (map[x+1][y] == GH_MAP_DUSTY_CORR) {
        maze_find_hidden_corridors(x+1, y);
    }
    if (map[x][y-1] == GH_MAP_DUSTY_CORR) {
        maze_find_hidden_corridors(x, y-1);
    }
    if (map[x][y+1] == GH_MAP_DUSTY_CORR) {
        maze_find_hidden_corridors(x, y+1);
    }
}

//
// We've made some corridors. Now stick them in the middle so we
// can place rooms around the outside easier.
//
void gh_maze::maze_recenter (void)
{
    GH_FUNCTION()

    int min_x = maze_w;
    int min_y = maze_h;
    int max_x = 0;
    int max_y = 0;

    for (int y = 1; y < maze_h-1; y++) {
        for (int x = 1; x < maze_w-1; x++) {
            if (map[x][y] != ' ') {
                if (x < min_x) {
                    min_x = x;
                }

                if (x > max_x) {
                    max_x = x;
                }

                if (y < min_y) {
                    min_y = y;
                }

                if (y > max_y) {
                    max_y = y;
                }
            }
        }
    }

    maze_backup();
    maze_clean();

    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {

            map[(x - min_x) + (min_x + (maze_w - max_x) / 2)]
               [(y - min_y) + (min_y + (maze_h - max_y) / 2)] =
                    map_save[x][y];
        }
    }
}

//
// save the maze to a temporary, for debugging
//
void gh_maze::maze_backup (void)
{
    GH_FUNCTION()

    memcpy(map_save, map, sizeof(map));
}

//
// save the maze to a temporary, for debugging
//
void gh_maze::maze_restore (void)
{
    GH_FUNCTION()

    memcpy(map, map_save, sizeof(map));
}

//
// Rotate 90 degrees
//
void gh_maze::maze_rotate (void)
{
    GH_FUNCTION()

    maze_backup();

    int tmp_maze_w = maze_w;
    int tmp_maze_h = maze_h;

    for (int y = 0; y <= maze_h; y++) {
        for (int x = 0; x <= maze_w; x++) {
            map[y][x] = map_save[maze_w - x - 1][y];
        }
    }

    maze_w = tmp_maze_h;
    maze_h = tmp_maze_w;
}

//
// Evaluate the characters that a room borders
//
int gh_maze::maze_room_eval_border_char (char l, char c, char r)
{
    GH_FUNCTION()

    int score = 0;

    if ((l == GH_MAP_ROOM) || (c == GH_MAP_ROOM) || (r == GH_MAP_ROOM)) {
        //
        // This is bad. We don't want rooms adjoining.
        //
        score += 1000;
    }

    if ((l == GH_MAP_ROOM_WALL) || (c == GH_MAP_ROOM_WALL) || 
        (r == GH_MAP_ROOM_WALL)) {
        //
        // This is bad. We don't want rooms adjoining.
        //
        score += 1000;
    }

    if (c == GH_MAP_CORR) {
        //
        // This is ok. A corridor runs into the room.
        // Don't allow too many though.
        //
        score += 1;
    }

    if (c == GH_MAP_DUSTY_CORR) {
        //
        // This is ok. A corridor runs into the room.
        // Don't allow too many though.
        //
        score += 2;
    }

    return (score);
}

//
// Evaluate the characters that a room overlaps
//
int gh_maze::maze_room_eval_char (char c)
{
    GH_FUNCTION()

    if (c == GH_MAP_ROOM) {
        return (1000);
    }

    if (c == GH_MAP_ROOM_WALL) {
        return (1000);
    }

    if (c == GH_MAP_CORR) {
        return (1);
    }

    if (c == GH_MAP_DUSTY_CORR) {
        return (2);
    }

    return (0);
}

//
// Iterate through the maze trying to find the best place for the
// randomly generated room in our array. It can cross corridors
// which is fine - these will just become doors; but we prefer as
// few of these as possible.
//
bool gh_maze::room_place_in_maze (int *sx, int *sy, int *ex, int *ey)
{
    GH_FUNCTION()

    int best_x = -1;
    int best_y = -1;
    int best_score = 100000;

    //
    // Use 2 here to allow us to add thick walls for the rooms
    //
    // Make it more to allow corridors to go around the doors
    //
    for (int y = 12; y < maze_h - room_h - 12; y+=4) {
            for (int x = 12; x < maze_w - room_w - 12; x+=4) {

            int score = 0;

            //
            // See if this room overlaps another room.
            //
            for (int ry = 0; (ry < room_h) && (score < 1000); ry++) {
                for (int rx = 0; (rx < room_w) && (score < 1000); rx++) {
                    //
                    // Or is close to another room
                    //
                    for (int ix = -10; ix <= 10; ix ++) {
                        for (int iy = -10; iy <= 10; iy ++) {
                            if (map[x + rx + ix][y + ry + iy] == GH_MAP_ROOM) {
                                score += 1000;
                            }

                            if (score >= 1000) {
                                break;
                            }
                        }

                        if (score >= 1000) {
                            break;
                        }
                    }
                }
            }

            //
            // Evaluate all the characters *under* this room
            //
            for (int ry = 0; (ry < room_h) && (score < 1000); ry++) {
                for (int rx = 0; (rx < room_w) && (score < 1000); rx++) {
                    if (room[rx][ry] != ' ') {
                        score += maze_room_eval_char(
                            map[x + rx][y + ry]);
                    }
                }
            }

            //
            // Evaluate all the characters bordering this room
            //
            for (int ry = 0; (ry < room_h) && (score < 1000); ry++) {
                for (int rx = 0; (rx < room_w) && (score < 1000); rx++) {

                    if (room[rx][ry] == ' ') {
                        continue;
                    }

                    // R ?
                    // RR?
                    // R ?
                    if ((rx == room_w - 1) || (room[rx+1][ry] == ' ')) {
                        score += maze_room_eval_border_char(
                            map[x + rx + 1][y + ry - 1],
                            map[x + rx + 1][y + ry],
                            map[x + rx + 1][y + ry + 1]);
                    }

                    // ? R
                    // ?RR
                    // ? R
                    if ((rx == 0) || (room[rx-1][ry] == ' ')) {
                        score += maze_room_eval_border_char(
                            map[x + rx - 1][y + ry - 1],
                            map[x + rx - 1][y + ry],
                            map[x + rx - 1][y + ry + 1]);
                    }

                    // ???
                    //  R
                    // RRR
                    if ((ry == room_h - 1) || (room[rx][ry+1] == ' ')) {
                        score += maze_room_eval_border_char(
                            map[x + rx - 1][y + ry + 1],
                            map[x + rx]    [y + ry + 1],
                            map[x + rx + 1][y + ry + 1]);
                    }

                    // RRR
                    //  R
                    // ???
                    if ((ry == 0) || (room[rx][ry-1] == ' ')) {
                        score += maze_room_eval_border_char(
                            map[x + rx - 1][y + ry - 1],
                            map[x + rx]    [y + ry - 1],
                            map[x + rx + 1][y + ry - 1]);
                    }
                }
            }

            //
            // Over empty space?
            //
            if (score == 0) {
                //
                // Occasionally allow this; *very* secret rooms.
                //
                if (myrand(maze_w * maze_h) < 2) {
                    best_score = 0;
                } else {
                    continue;
                }
            }

            //
            // Over too much?
            //
            if (score >= 1000) {
                continue;
            }

            score += myrand(20);

            if (score < best_score) {
                best_x = x;
                best_y = y;
                best_score = score;
            } else if (score == best_score) {
                if (myrand(100) > 50) {
                    best_x = x;
                    best_y = y;
                    best_score = score;
                }
            }
        }
    }

    if (best_score >= 1000) {
        return (false);
    }

    int x = best_x;
    int y = best_y;

    if ((x == -1) && (y == -1)) {
        return (false);
    }

    gh_room *r = new gh_room("");
    r->is_random = true;

    for (int ry = 0; ry < room_h; ry++) {
        for (int rx = 0; rx < room_w; rx++) {
            if (room[rx][ry] != ' ') {
                map[x + rx][y + ry] = GH_MAP_ROOM;

                for (int ix = -1; ix <= 1; ix ++) {
                    for (int iy = -1; iy <= 1; iy ++) {
                        rooms[x + rx + ix][y + ry + iy] = r;
                    }
                }
            }
        }
    }

    *sx = x;
    *sy = y;
    *ex = x + room_w;
    *ey = y + room_h;

    return (true);
}

//
// initialize the maze.
//
void gh_maze::maze_wallify (int sx, int sy, int ex, int ey)
{
    GH_FUNCTION()

    for (int y = sy; y < ey; y++) {
        for (int x = sx; x < ex; x++) {
            if (map[x][y] == GH_MAP_ROOM) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {

                        if (map[x+dx][y+dy] == ' ') {
                            map[x+dx][y+dy] = GH_MAP_ROOM_WALL;

                            //
                            // Make some of the room walls secret doors.
                            //
                            char c = map[x+dx-1][y+dy];

                            if ((c == GH_MAP_CORR) || 
                                (c == GH_MAP_DUSTY_CORR)) {
                                if (myrand(100) < 2) {
                                    map[x+dx-1][y+dy] = GH_MAP_SECRET_WALL;
                                }
                            }

                            c = map[x+dx][y+dy-1];

                            if ((c == GH_MAP_CORR) || 
                                (c == GH_MAP_DUSTY_CORR)) {
                                if (myrand(100) < 2) {
                                    map[x+dx][y+dy-1] = GH_MAP_SECRET_WALL;
                                }
                            }

                            c = map[x+dx+1][y+dy];

                            if ((c == GH_MAP_CORR) || 
                                (c == GH_MAP_DUSTY_CORR)) {
                                if (myrand(100) < 2) {
                                    map[x+dx+1][y+dy] = GH_MAP_SECRET_WALL;
                                }
                            }

                            c = map[x+dx][y+dy+1];

                            if ((c == GH_MAP_CORR) || 
                                (c == GH_MAP_DUSTY_CORR)) {
                                if (myrand(100) < 2) {
                                    map[x+dx][y+dy+1] = GH_MAP_SECRET_WALL;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

//
// initialize the maze.
//
void gh_maze::maze_wallify2 (int sx, int sy, int ex, int ey)
{
    GH_FUNCTION()

    for (int y = sy; y < ey; y++) {
        for (int x = sx; x < ex; x++) {
            if (map[x][y] == GH_MAP_ROOM) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int wdx = -1; wdx <= 1; wdx ++) {
                            for (int wdy = -1; wdy <= 1; wdy ++) {
                                if (myrand(100) < 10) {
                                    if (map[x+dx+wdx][y+dy+wdy] == ' ') {
                                        map[x+dx+wdx][y+dy+wdy] = 
                                                        GH_MAP_ROOM_WALL;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //
    // Remove stub walls
    //
    for (int y = sy; y < ey; y++) {
        for (int x = sx; x < ex; x++) {
            if (map[x][y] == GH_MAP_ROOM_WALL) {
                int neb;

                neb =  (map[x-1][y] == ' ');
                neb += (map[x+1][y] == ' ');
                neb += (map[x][y-1] == ' ');
                neb += (map[x][y+1] == ' ');

                if (neb >= 3) {
                    map[x][y] = ' ';
                }
            }
        }
    }
}

//
// initialize the maze.
//
void gh_maze::maze_wallify_corr (void)
{
    GH_FUNCTION()

    for (int y = 1; y < maze_h; y++) {
        for (int x = 1; x < maze_w; x++) {
            if ((map[x][y] == GH_MAP_CORR) || (map[x][y] == GH_MAP_DUSTY_CORR)) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (map[x+dx][y+dy] == ' ') {
                            map[x+dx][y+dy] = GH_MAP_CORR_WALL;
                        }
                    }
                }
            }
        }
    }
}

//
// Fill a room with an id
//
void gh_maze::maze_flood_room (int x, int y, int room_no)
{
    GH_FUNCTION()

    room_number_map[x][y] = room_no;

    if (map_searched[x][y]) {
        return;
    }

    map_searched[x][y] = 1;

    if ((map[x-1][y] == GH_MAP_CORR) ||
        (map[x+1][y] == GH_MAP_CORR) ||
        (map[x][y-1] == GH_MAP_CORR) ||
        (map[x][y+1] == GH_MAP_CORR)) {
        maze_room_info[room_no].corridors++;
    }

    if ((map[x-1][y] == GH_MAP_DUSTY_CORR) ||
        (map[x+1][y] == GH_MAP_DUSTY_CORR) ||
        (map[x][y-1] == GH_MAP_DUSTY_CORR) ||
        (map[x][y+1] == GH_MAP_DUSTY_CORR)) {
        maze_room_info[room_no].dusty_corridors++;
    }

    if (map[x-1][y] == GH_MAP_ROOM) {
        maze_flood_room(x-1, y, room_no);
    }

    if (map[x+1][y] == GH_MAP_ROOM) {
        maze_flood_room(x+1, y, room_no);
    }

    if (map[x][y-1] == GH_MAP_ROOM) {
        maze_flood_room(x, y-1, room_no);
    }

    if (map[x][y+1] == GH_MAP_ROOM) {
        maze_flood_room(x, y+1, room_no);
    }
}

//
// Find all secret corridors bordering this room and mark them as
// hidden walls
//
void gh_maze::maze_mark_secret_doors (int x, int y, int room_no)
{
    GH_FUNCTION()

    if (map_searched[x][y]) {
        return;
    }

    map_searched[x][y] = 1;

    if ((map[x-1][y] == GH_MAP_DUSTY_CORR) && !map_obj[x-1][y]) {
        map_obj[x-1][y] = GH_MAP_SECRET_WALL;
    }

    if ((map[x+1][y] == GH_MAP_DUSTY_CORR) && !map_obj[x+1][y]) {
        map_obj[x+1][y] = GH_MAP_SECRET_WALL;
    }

    if ((map[x][y+1] == GH_MAP_DUSTY_CORR) && !map_obj[x][y+1]) {
        map_obj[x][y+1] = GH_MAP_SECRET_WALL;
    }

    if ((map[x][y-1] == GH_MAP_DUSTY_CORR) && !map_obj[x][y-1]) {
        map_obj[x][y-1] = GH_MAP_SECRET_WALL;
    }

    if (map[x-1][y] == GH_MAP_ROOM) {
        maze_mark_secret_doors(x-1, y, room_no);
    }

    if (map[x+1][y] == GH_MAP_ROOM) {
        maze_mark_secret_doors(x+1, y, room_no);
    }

    if (map[x][y-1] == GH_MAP_ROOM) {
        maze_mark_secret_doors(x, y-1, room_no);
    }

    if (map[x][y+1] == GH_MAP_ROOM) {
        maze_mark_secret_doors(x, y+1, room_no);
    }
}

//
// Mark each room in the maze with a room number.
//
int gh_maze::maze_count_rooms (void)
{
    GH_FUNCTION()

    int r = 1;

    memset(map_searched, 0, sizeof(map_searched));

    for (int y = 1; y < maze_h - 1; y++) {
        for (int x = 1; x < maze_w - 1; x++) {
            if (room_number_map[x][y] > 0) {
                continue;
            }

            if (map[x][y] == GH_MAP_ROOM) {
                maze_flood_room(x,y,r++);
            }
        }
    }

    return (r);
}

//
// Mark each room in the maze with a room number.
//
void gh_maze::maze_hide_doors (void)
{
    GH_FUNCTION()

    memset(map_searched, 0, sizeof(map_searched));

    for (int y = 1; y < maze_h - 1; y++) {
        for (int x = 1; x < maze_w - 1; x++) {
            if (map[x][y] != GH_MAP_ROOM) {
                continue;
            }

            if (room_number_map[x][y] == 0) {
                continue;
            }

            int r = room_number_map[x][y];

            //
            // If this room has dusty and normal corridors then hide the
            // entrance to the dusty ones.
            //
            if (maze_room_info[r].corridors &&
                maze_room_info[r].dusty_corridors) {

                maze_mark_secret_doors(x,y,r);
            }
        }
    }
}

//
// print the room.
//
void gh_maze::room_print (void)
{
    GH_FUNCTION()

    int x;
    int y;

    for (x = 0; x <= room_w + 1; x++) {
        fprintf(stdout, "#");
    }

    fprintf(stdout, "\n");
    for (y = 0; y < room_h; y++) {
        fprintf(stdout, "#");
        for (x = 0; x < room_w; x++) {
            fprintf(stdout, "%c", room[x][y]);
        }
        fprintf(stdout, "#\n");
    }

    for (x = 0; x <= room_w + 1; x++) {
        fprintf(stdout, "#");
    }
    fprintf(stdout, "\n");
}

//
// initialize the room.
//
void gh_maze::room_reset (void)
{
    GH_FUNCTION()

    int i;

    memset(room, ' ', sizeof(room));
    memset(room_save, ' ', sizeof(room_save));

    //
    // Stick in some occasional square rooms.
    //
    if (myrand(100) < 40) {
redo:
        int w = myrand(20) + 4;
        int h = myrand(20) + 4;
        int x = myrand(room_w);
        int y = myrand(room_h);

        if (x < 0) {
            goto redo;
        }

        if (y < 0) {
            goto redo;
        }

        if (x + w >= room_w) {
            goto redo;
        }

        if (y + h >= room_h) {
            goto redo;
        }

        for (int x1 = x; x1 < x + w; x1++) {
            for (int y1 = y; y1 < y + h; y1++) {
                room[x1][y1] = GH_MAP_ROOM_POSSIBLE;
            }
        }


        //
        // Occasionally, allow us to overlay some randomness on
        // the square.
        //
        if (myrand(100) < 10) {
            return;
        }
    }

    for (i = 0; i < room_w; i++) {
        double r;
        double rad_start = myfrand(3.14 * 2.0);
        double rad_end = rad_start + myfrand(3.14 * 2.0);
        double rad_delta = (rad_end - rad_start) / myrand(90+5);
        int x = myrand(room_w);
        int y = myrand(room_h);

        //
        // Create random lines. This will form our room.
        //
        for (r = rad_start; r < rad_end; r += rad_delta) {

            int x2 = (int)(x + sin(r) * myrand(room_w / 4));
            int y2 = (int)(y + cos(r) * myrand(room_h / 8));
            int x3 = (int)(x - sin(r) * myrand(room_w / 4));
            int y3 = (int)(y - cos(r) * myrand(room_h / 8));
            int x4 = (int)(x + sin(r) * myrand(room_w / 4));
            int y4 = (int)(y - cos(r) * myrand(room_h / 8));
            int x5 = (int)(x - sin(r) * myrand(room_w / 4));
            int y5 = (int)(y + cos(r) * myrand(room_h / 8));

            if (x2 < 0) {
                continue;
            }

            if (y2 < 0) {
                continue;
            }

            if (x2 >= room_w) {
                continue;
            }


            if (y2 >= room_h) {
                continue;
            }

            room_line(x, y, x2, y2);

            if (myrand(100) < 50) {
                room_line(x, y, x3, y3);
            }

            if (myrand(100) < 50) {
                room_line(x, y, x4, y4);
            }

            if (myrand(100) < 50) {
                room_line(x, y, x5, y5);
            }
        }
    }
}

//
// Make sure we have a one space border around the room.
//
void gh_maze::room_ensure_border_around_room (void)
{
    GH_FUNCTION()

    for (int y = 0; y < room_h; y++) {
        room[0][y] = ' ';
        room[room_w - 1][y] = ' ';
    }

    for (int x = 0; x < room_w; x++) {
        room[x][0] = ' ';
        room[x][room_h - 1] = ' ';
    }
}

//
// Get rid of lone squares
//
void gh_maze::room_trim (void)
{
    GH_FUNCTION()

    int got = 1;

    while (got) {
        got = 0;
        for (int y = 1; y < room_h - 1; y++) {
            for (int x = 1; x < room_w - 1; x++) {
                if (room[x][y] != ' ') {
                    if (room[x+1][y] != ' ') {
                        continue;
                    }
                    if (room[x-1][y] != ' ') {
                        continue;
                    }
                    if (room[x][y+1] != ' ') {
                        continue;
                    }
                    if (room[x][y-1] != ' ') {
                        continue;
                    }
                    room[x][y] = ' ';
                    got = 1;
                }
            }
        }
    }
}

//
// Smooth out the room
//
void gh_maze::room_trim_stubs (void)
{
    GH_FUNCTION()

    int got = 1;

    while (got) {
        got = 0;
        for (int y = 1; y < room_h - 1; y++) {
            for (int x = 1; x < room_w - 1; x++) {
                if (room[x][y] != ' ') {
                    int neb = 0;

                    if (room[x+1][y] != ' ') {
                        neb++;
                    }
                    if (room[x-1][y] != ' ') {
                        neb++;
                    }
                    if (room[x][y+1] != ' ') {
                        neb++;
                    }
                    if (room[x][y-1] != ' ') {
                        neb++;
                    }
                    if (neb == 1) {
                        room[x][y] = ' ';
                        got = 1;
                    }
                }
            }
        }
    }
}

//
// Get rid of all the bits that aren't a room
//
void gh_maze::room_scrub (void)
{
    GH_FUNCTION()

    for (int y = 0; y < room_h; y++) {
        for (int x = 0; x < room_w; x++) {
            if (room[x][y] == GH_MAP_ROOM_POSSIBLE) {
                room[x][y] = ' ';
            }
        }
    }
}

//
// How big is our room?
//
int gh_maze::room_count (int x, int y, int c)
{
    GH_FUNCTION()

    c++;
    room[x][y] = ' ';

    if (x < room_w) {
        if (room[x + 1][y] != ' ') {
            c = room_count(x + 1, y, c);
        }
    }

    if (x) {
        if (room[x - 1][y] != ' ') {
            c = room_count(x - 1, y, c);
        }
    }

    if (y < room_h) {
        if (room[x][y + 1] != ' ') {
            c = room_count(x, y + 1, c);
        }
    }

    if (y) {
        if (room[x][y - 1] != ' ') {
            c = room_count(x, y - 1, c);
        }
    }
    return (c);
}

void gh_maze::room_mark (int x, int y)
{
    GH_FUNCTION()

    if (room[x][y] != GH_MAP_ROOM_POSSIBLE) {
        return;
    }

    room[x][y] = GH_MAP_ROOM;

    if (x < room_w) {
        if (room[x + 1][y] == GH_MAP_ROOM_POSSIBLE) {
            room_mark(x + 1, y);
        }
    }

    if (x > 0) {
        if (room[x - 1][y] == GH_MAP_ROOM_POSSIBLE) {
            room_mark(x - 1, y);
        }
    }

    if (y < room_h) {
        if (room[x][y + 1] == GH_MAP_ROOM_POSSIBLE) {
            room_mark(x, y + 1);
        }
    }

    if (y > 0) {
        if (room[x][y - 1] == GH_MAP_ROOM_POSSIBLE) {
            room_mark(x, y - 1);
        }
    }
}

//
// Attempt to find a feasible room out of the random room characters
// we've generate
//
int gh_maze::room_find_one (void)
{
    GH_FUNCTION()

    int attempt;

    for (attempt = 0; attempt < 100; attempt++) {
        int x = myrand(room_w);
        int y = myrand(room_h);

        if (room[x][y] == GH_MAP_ROOM_POSSIBLE) {
            room_backup();

            if (room_count(x, y, 0) > 4) {
                room_restore();
                room_mark(x, y);
                return (1);
            }
        }
    }
    return (0);
}

//
// Given a room, work out its bounding x1 and shrink it to it.
//
void gh_maze::room_minimize (void)
{
    GH_FUNCTION()

    int min_x = room_w;
    int min_y = room_h;
    int max_x = 0;
    int max_y = 0;

    for (int y = 0; y < room_h; y++) {
        for (int x = 0; x < room_w; x++) {
            if (room[x][y] != ' ') {
                if (x < min_x) {
                    min_x = x;
                }

                if (x > max_x) {
                    max_x = x;
                }

                if (y < min_y) {
                    min_y = y;
                }

                if (y > max_y) {
                    max_y = y;
                }
            }
        }
    }

    room_backup();

    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {

            room[(x - min_x)][(y - min_y)] = room_save[x][y];
        }
    }

    room_w = max_x - min_x + 1;
    room_h = max_y - min_y + 1;
}

//
// save the room to a temporary, for debugging
//
void gh_maze::room_backup (void)
{
    GH_FUNCTION()

    for (int y = 0; y < room_h; y++) {
        for (int x = 0; x < room_w; x++) {
            room_save[x][y] = room[x][y];
        }
    }
}

//
// save the room to a temporary, for debugging
//
void gh_maze::room_restore (void)
{
    GH_FUNCTION()

    for (int y = 0; y < room_h; y++) {
        for (int x = 0; x < room_w; x++) {
            room[x][y] = room_save[x][y];
        }
    }
}

//
// Generate a randomly shaped room
//
void gh_maze::room_gen (void)
{
    GH_FUNCTION()

    int r = 0;

    if (myrand(100) < 20) {
        r += min(10, (level / 10));
    }

    if (myrand(100) == 1) {
        r += min(20, (level / 5));
    }

    room_w = GH_MIN(GH_MAP_ROOM_W_AMOUNT + r, GH_MAX_MAZE_SIZE - 2);
    room_h = GH_MIN(GH_MAP_ROOM_H_AMOUNT + r, GH_MAX_MAZE_SIZE - 2);

    do {
        room_reset();
        room_trim();
        room_ensure_border_around_room();

        if (myrand(10) < 4) {
            room_trim_stubs();
        }
    } while (room_find_one() == 0);

    room_scrub();
    room_minimize();

#ifdef DEBUG_MAZE
    room_print();
#endif
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
int gh_maze::room_place_obstacle (int x, int y, char c, int not_in_this_room)
{
    GH_FUNCTION()

    int attempt = 0;

    while (attempt < 1000) {
       attempt++;

        int old_x = x;
        int old_y = y;

        if (!gh_maze_can_obstacle(map[x-1][y]) &&
            !gh_maze_can_obstacle(map[x+1][y]) &&
            !gh_maze_can_obstacle(map[x][y-1]) &&
            !gh_maze_can_obstacle(map[x][y+1])) {
            //
            // We're stuck.
            //
            return (0);
        }

        //
        // Try to move randomly around the room.
        //
        bool moved = false;
        while (!moved) {
            switch (myrand(4)) {
            case 0:
                if (gh_maze_can_obstacle(map[x-1][y])) {
                    x--;
                    moved = true;
                }
                break;
            case 1:
                if (gh_maze_can_obstacle(map[x+1][y])) {
                    x++;
                    moved = true;
                }
                break;
            case 2:
                if (gh_maze_can_obstacle(map[x][y-1])) {
                    y--;
                    moved = true;
                }
                break;
            case 3:
                if (gh_maze_can_obstacle(map[x][y+1])) {
                    y++;
                    moved = true;
                }
                break;
            }
        }

        //
        // Don't place on top of existing objects, like electro doors.
        //
        if (map_obj[x][y]) {
            x = old_x;
            y = old_y;
            goto redo;
        }

        //
        // Failed to move? Backtrack.
        //
        if (!gh_maze_can_obstacle(map[x][y])) {
            x = old_x;
            y = old_y;
            goto redo;
        }

        if (not_in_this_room != -1) {
            if (not_in_this_room == room_number_map[x][y]) {
                x = old_x;
                y = old_y;
                goto redo;
            }
        }

        //
        // Don't plaze an object next to another one
        //
        for (int dx = -2; dx < 2; dx++) {
            for (int dy = -2; dy < 2; dy++) {
                if (map_obstacle[x+dx]  [y+dy]) {
                    goto redo;
                }
            }
        }

        //
        // Don't plaze an object next to a corridor
        //
        if ((map[x-1][y-1] == GH_MAP_CORR) ||
            (map[x]  [y-1] == GH_MAP_CORR) ||
            (map[x+1][y-1] == GH_MAP_CORR) ||
            (map[x-1][y]   == GH_MAP_CORR) ||
            (map[x]  [y]   == GH_MAP_CORR) ||
            (map[x+1][y]   == GH_MAP_CORR) ||
            (map[x-1][y+1] == GH_MAP_CORR) ||
            (map[x]  [y+1] == GH_MAP_CORR) ||
            (map[x+1][y+1] == GH_MAP_CORR)) {
            goto redo;
        }

        //
        // Don't plaze an object next to a corridor
        //
        if ((map[x-1][y-1] == GH_MAP_DUSTY_CORR) ||
            (map[x]  [y-1] == GH_MAP_DUSTY_CORR) ||
            (map[x+1][y-1] == GH_MAP_DUSTY_CORR) ||
            (map[x-1][y]   == GH_MAP_DUSTY_CORR) ||
            (map[x]  [y]   == GH_MAP_DUSTY_CORR) ||
            (map[x+1][y]   == GH_MAP_DUSTY_CORR) ||
            (map[x-1][y+1] == GH_MAP_DUSTY_CORR) ||
            (map[x]  [y+1] == GH_MAP_DUSTY_CORR) ||
            (map[x+1][y+1] == GH_MAP_DUSTY_CORR)) {
            goto redo;
        }

        //
        // Don't plaze an exit next to 2 walls as you wont
        // be able to get past it
        //
        int walls;
        walls =  map[x-1][y-1] == GH_MAP_ROOM_WALL;
        walls += map[x]  [y-1] == GH_MAP_ROOM_WALL;
        walls += map[x+1][y-1] == GH_MAP_ROOM_WALL;
        walls += map[x-1][y]   == GH_MAP_ROOM_WALL;
        walls += map[x]  [y]   == GH_MAP_ROOM_WALL;
        walls += map[x+1][y]   == GH_MAP_ROOM_WALL;
        walls += map[x-1][y+1] == GH_MAP_ROOM_WALL;
        walls += map[x]  [y+1] == GH_MAP_ROOM_WALL;
        walls += map[x+1][y+1] == GH_MAP_ROOM_WALL;

        //
        // Don't plaze an exit next to 2 walls as you wont
        // be able to get past it
        //
        int doors;
        doors =  map_obj[x-1][y-1] == GH_MAP_DOOR;
        doors += map_obj[x]  [y-1] == GH_MAP_DOOR;
        doors += map_obj[x+1][y-1] == GH_MAP_DOOR;
        doors += map_obj[x-1][y]   == GH_MAP_DOOR;
        doors += map_obj[x]  [y]   == GH_MAP_DOOR;
        doors += map_obj[x+1][y]   == GH_MAP_DOOR;
        doors += map_obj[x-1][y+1] == GH_MAP_DOOR;
        doors += map_obj[x]  [y+1] == GH_MAP_DOOR;
        doors += map_obj[x+1][y+1] == GH_MAP_DOOR;

        //
        // Don't place a hero next to an obstacle
        //
        if (c == GH_MAP_START) {
            if (walls) {
                goto redo;
            }

            if (doors) {
                goto redo;
            }

            if (rooms[x][y]) {
                if (rooms[x][y]->is_random) {
                    //
                    // Random rooms are ok.
                    //
                } else {
                    //
                    // Not in monster or treasure rooms!
                    //
                    continue;
                }
            }
        }

        if (walls > 1) {
            goto redo;
        }

        //
        // No exits in shops; too easy 8)
        //
        if ((c == GH_MAP_EXIT) ||
            (c == GH_MAP_TELEPORT) ||
            (c == GH_MAP_RETURN_EXIT)) {
            if (rooms[x][y]) {
                if (rooms[x][y]->is_shop) {
                    continue;
                }
            }
        }

        //
        // Do we want to try here?
        //
        if (myrand(100) < 30) {
            map_obstacle[x][y] = c;

            //
            // If placing the map start, can we place the return exit too?
            //
            // If we can't it's not a big deal. It just makes it impossible
            // to return.
            //
            // Don't do this for the first level.
            //
            if ((c == GH_MAP_START) && (level > 1)) {
                while (attempt < 1000) {
                    attempt++;

                    int radius = 2;

                    for (int dx = -radius; dx <= radius; dx++) {
                        for (int dy = -radius; dy <= radius; dy++) {

                            //
                            // Not under the hero!
                            //
                            if (!dx && !dy) {
                                continue;
                            }

                            if (map_obj[x + dx][y + dy]) {
                                continue;
                            }

                            if (!gh_maze_can_obstacle(map[x + dx][y + dy])) {
                                continue;
                            }

                            //
                            // Make sure the return exit is not on the
                            // edge of a room where it can replace a door.
                            //
                            if (!gh_maze_can_obstacle(
                                          map[x + dx + 1][y + dy + 1])) {
                                continue;
                            }

                            if (!gh_maze_can_obstacle(
                                          map[x + dx - 1][y + dy + 1])) {
                                continue;
                            }

                            if (!gh_maze_can_obstacle(
                                          map[x + dx - 1][y + dy - 1])) {
                                continue;
                            }

                            if (!gh_maze_can_obstacle(
                                          map[x + dx - 1][y + dy + 1])) {
                                continue;
                            }

                            if (myrand(100) < 5) {
                                map_obstacle[x+dx][y+dy] = GH_MAP_RETURN_EXIT;
                                return (1);
                            }
                        }
                    }
                }
            }

            return (1);
        }
redo:
        continue;
    }

    return (0);
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
int gh_maze::room_place_obstacle (char c, int not_in_this_room)
{
    GH_FUNCTION()

    int attempt = 0;

    while (attempt < 100000) {
        int x = myrand(maze_w - 4) + 2;
        int y = myrand(maze_h - 4) + 2;

        attempt++;

        //
        // Want to find a room next to a corridor.
        //
        if (map[x][y] != GH_MAP_CORR) {
            continue;
        }

        if (c == GH_MAP_START) {
            if (rooms[x][y]) {
                if (rooms[x][y]->is_random) {
                    //
                    // Random rooms are ok.
                    //
                } else {
                    //
                    // Not in monster or treasure rooms!
                    //
                    continue;
                }
            }
        }

        int rx = x - 1;
        int ry = y;

        if (gh_maze_can_obstacle(map[rx][ry])) {
            if (room_place_obstacle(rx, ry, c, not_in_this_room)) {
                return (room_number_map[rx][ry]);
            }
        }

        rx = x - 1;
        ry = y;
        if (gh_maze_can_obstacle(map[rx][ry])) {
            if (room_place_obstacle(rx, ry, c, not_in_this_room)) {
                return (room_number_map[rx][ry]);
            }
        }

        rx = x;
        ry = y - 1;
        if (gh_maze_can_obstacle(map[rx][ry])) {
            if (room_place_obstacle(rx, ry, c, not_in_this_room)) {
                return (room_number_map[rx][ry]);
            }
        }

        rx = x;
        ry = y + 1;
        if (gh_maze_can_obstacle(map[rx][ry])) {
            if (room_place_obstacle(rx, ry, c, not_in_this_room)) {
                return (room_number_map[rx][ry]);
            }
        }
    }
    return (0);
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
void gh_maze::place_treasure (void)
{
    GH_FUNCTION()

    int total_treasure = 2;

    if (level < 5) {
        total_treasure = myrand(GH_MAP_TREASURE_AMOUNT) + 1;
    } else if (level < 10) {
        total_treasure = myrand(GH_MAP_TREASURE_AMOUNT) + 4;
    } else if (level < 50) {
        total_treasure = myrand(GH_MAP_TREASURE_AMOUNT) + 8;
    } else {
        total_treasure = myrand(GH_MAP_TREASURE_AMOUNT) + 10;
    }

    if (december()) {
        total_treasure *= 2;
    }

    //
    // I know...
    //
    if (xmas()) {
        total_treasure *= 4;
    }

    while (total_treasure) {
        int x = myrand(maze_w - 2) + 1;
        int y = myrand(maze_h - 2) + 1;
        bool ok;

        ok = false;

        if (rooms[x][y]) {
            if (rooms[x][y]->is_shop) {
                continue;
            }
        }

        if (map[x][y] == ' ') {
            //
            // Don't plaze treasure next to a corridor
            //
            if ((map[x-1][y-1] == GH_MAP_CORR) ||
                (map[x]  [y-1] == GH_MAP_CORR) ||
                (map[x+1][y-1] == GH_MAP_CORR) ||
                (map[x-1][y]   == GH_MAP_CORR) ||
                (map[x]  [y]   == GH_MAP_CORR) ||
                (map[x+1][y]   == GH_MAP_CORR) ||
                (map[x-1][y+1] == GH_MAP_CORR) ||
                (map[x]  [y+1] == GH_MAP_CORR) ||
                (map[x+1][y+1] == GH_MAP_CORR)) {
                continue;
            }

            //
            // Don't plaze treasure next to a corridor
            //
            if ((map[x-1][y-1] == GH_MAP_DUSTY_CORR) ||
                (map[x]  [y-1] == GH_MAP_DUSTY_CORR) ||
                (map[x+1][y-1] == GH_MAP_DUSTY_CORR) ||
                (map[x-1][y]   == GH_MAP_DUSTY_CORR) ||
                (map[x]  [y]   == GH_MAP_DUSTY_CORR) ||
                (map[x+1][y]   == GH_MAP_DUSTY_CORR) ||
                (map[x-1][y+1] == GH_MAP_DUSTY_CORR) ||
                (map[x]  [y+1] == GH_MAP_DUSTY_CORR) ||
                (map[x+1][y+1] == GH_MAP_DUSTY_CORR)) {
                continue;
            }

            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_CORR) {
            ok = (myrand(100) < 10);
        } else if (map[x][y] == GH_MAP_DUSTY_CORR) {
            ok = (myrand(100) < 40);
        } else if (map[x][y] == GH_MAP_CAVE_DIRT) {
            ok = (myrand(100) < 10);
        } else if (map[x][y] == GH_MAP_DIRT) {
            ok = (myrand(100) < 10);
        } else if (map[x][y] == GH_MAP_GRASS) {
            ok = (myrand(100) < 3);
        } else if (map[x][y] == GH_MAP_VOID_FLOOR) {
            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_ROOM) {
            maze_room_info_t *room;

            room = &maze_room_info[(int)room_number_map[x][y]];

            if (room->dusty_corridors) {
                ok = true;
            } else if (room->corridors) {
                ok = (myrand(100) < 80);
            } else if (!room->corridors) {
                ok = true;
            }
        } else {
            continue;
        }

        if (!ok) {
            continue;
        }

        if (map_obj[x][y]) {
            continue;
        }

        if (map_obstacle[x][y]) {
            //
            // Don't place on top of an obstacle, like the player
            // or maze exit
            //
            continue;
        }

        map_obj[x][y] = GH_MAP_TREASURE;

        total_treasure--;
    }
}

void gh_maze::place_potion (void)
{
    GH_FUNCTION()

    int total_potions = 2;

    if (level < 5) {
        total_potions = myrand(GH_MAP_POTION_AMOUNT) + 2;
    } else if (level < 10) {
        total_potions = myrand(GH_MAP_POTION_AMOUNT) + 3;
    } else if (level < 50) {
        total_potions = myrand(GH_MAP_POTION_AMOUNT) + 4;
    } else {
        total_potions = myrand(GH_MAP_POTION_AMOUNT) + 5;
    }

    while (total_potions) {
        int x = myrand(maze_w - 2) + 1;
        int y = myrand(maze_h - 2) + 1;
        bool ok;

        ok = false;

        if (rooms[x][y]) {
            if (rooms[x][y]->is_shop) {
                continue;
            }
        }

        if (map[x][y] == ' ') {
            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_CORR) {
            ok = (myrand(100) < 10);
        } else if (map[x][y] == GH_MAP_DUSTY_CORR) {
            ok = (myrand(100) < 40);
        } else if (map[x][y] == GH_MAP_CAVE_DIRT) {
            ok = (myrand(100) < 10);
        } else if (map[x][y] == GH_MAP_DIRT) {
            ok = (myrand(100) < 10);
        } else if (map[x][y] == GH_MAP_GRASS) {
            ok = (myrand(100) < 3);
        } else if (map[x][y] == GH_MAP_VOID_FLOOR) {
            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_ROOM) {
            maze_room_info_t *room;

            room = &maze_room_info[(int)room_number_map[x][y]];

            if (room->dusty_corridors) {
                ok = true;
            } else if (room->corridors) {
                ok = (myrand(100) < 80);
            } else if (!room->corridors) {
                ok = true;
            }
        } else {
            continue;
        }

        if (!ok) {
            continue;
        }

        if (map_obj[x][y]) {
            continue;
        }

        if (map_obstacle[x][y]) {
            //
            // Don't place on top of an obstacle, like the player
            // or maze exit
            //
            continue;
        }

        map_obj[x][y] = GH_MAP_POTION;

        total_potions--;
    }
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
void gh_maze::place_food (void)
{
    GH_FUNCTION()

    int total_food = 2;

    if (level < 5) {
        total_food = myrand(GH_MAP_FOOD_AMOUNT) + 2;
    } else if (level < 10) {
        total_food = myrand(GH_MAP_FOOD_AMOUNT) + 4;
    } else if (level < 50) {
        total_food = myrand(GH_MAP_FOOD_AMOUNT) + 4;
    } else {
        total_food = myrand(GH_MAP_FOOD_AMOUNT) + 3;
    }

    while (total_food) {
        int x = myrand(maze_w - 2) + 1;
        int y = myrand(maze_h - 2) + 1;
        bool ok;

        ok = false;

        if (rooms[x][y]) {
            if (rooms[x][y]->is_shop) {
                continue;
            }
        }

        if (map[x][y] == GH_MAP_CORR) {
            ok = (myrand(100) < 5);
        } else if (map[x][y] == GH_MAP_DUSTY_CORR) {
            ok = (myrand(100) < 2);
        } else if (map[x][y] == GH_MAP_CAVE_DIRT) {
            ok = (myrand(100) < 2);
        } else if (map[x][y] == GH_MAP_DIRT) {
            ok = (myrand(100) < 3);
        } else if (map[x][y] == GH_MAP_GRASS) {
            ok = (myrand(100) < 30);
        } else if (map[x][y] == GH_MAP_VOID_FLOOR) {
            ok = (myrand(100) < 3);
        } else if (map[x][y] == GH_MAP_ROOM) {
            maze_room_info_t *room;

            room = &maze_room_info[(int)room_number_map[x][y]];

            if (room->dusty_corridors) {
                ok = true;
            } else if (room->corridors) {
                ok = (myrand(100) < 80);
            } else if (!room->corridors) {
                ok = true;
            }
        } else {
            continue;
        }

        if (!ok) {
            continue;
        }

        if (map_obj[x][y]) {
            continue;
        }

        if (map_obstacle[x][y]) {
            //
            // Don't place on top of an obstacle, like the player
            // or maze exit
            //
            continue;
        }

        map_obj[x][y] = GH_MAP_FOOD;

        total_food--;
    }
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
void gh_maze::place_obstacle (int obj)
{
    GH_FUNCTION()

    int tries = 10000;

    while (tries--) {
        int x = myrand(maze_w - 2) + 1;
        int y = myrand(maze_h - 2) + 1;

        if (map_obj[x][y]) {
            continue;
        }

        if (map_obstacle[x][y]) {
            //
            // Don't place on top of an obstacle, like the player
            // or maze exit
            //
            continue;
        }

        if (rooms[x][y]) {
            if (rooms[x][y]->is_shop) {
                continue;
            }
        }

        if (!gh_maze_can_obstacle(map[x][y])) {
            continue;
        }

        if (!gh_maze_can_obstacle(map[x-1][y])) {
            continue;
        }

        if (!gh_maze_can_obstacle(map[x+1][y])) {
            continue;
        }

        if (!gh_maze_can_obstacle(map[x][y-1])) {
            continue;
        }

        if (!gh_maze_can_obstacle(map[x][y+1])) {
            continue;
        }

        map_obstacle[x][y] = obj;

        return;
    }
}

bool gh_maze::room_fixed_place_in_maze (gh_room *r)
{
    GH_FUNCTION()

    int tries = 0;
    int room_no = rand() % r->contents.size();

#ifdef DEBUG_MAZE
    cout << "FIXED ROOM -------- " << endl;
#endif

    for (;;) {
redo:
        tries++;

        int w = r->contents[room_no].width;
        int h = r->contents[room_no].height;

        //
        // Make a border to allow corridors to go around the doors
        //
        int x = (rand() % ((maze_w - w) - 10)) + 5;
        int y = (rand() % ((maze_h - h) - 10)) + 5;

        //
        // See if we can place this fixed room here
        //
#ifdef DEBUG_MAZE
        for (int ih = 0; ih < h; ih++) {
            for (int iw = 0; iw < w; iw++) {
                cout << r->contents[room_no].cell[(ih * w) + iw].c;
            }
            cout << endl;
        }

        for (int ih = 0; ih < h; ih++) {
            for (int iw = 0; iw < w; iw++) {
                cout << map[x + iw][y + ih] ;
            }
            cout << endl;
        }
#endif

        for (int iw = 0; iw < w; iw++) {
            for (int ih = 0; ih < h; ih++) {
                int cx = x + iw;
                int cy = y + ih;

                char a,b,c,d,e;

                c = r->contents[room_no].cell[(ih * w) + iw].c;

                //
                // Check that the door is next to a corridor
                //
                if ((c != GH_MAP_DOOR) && (c != GH_MAP_ELECTRO_DOOR)) {
                    continue;
                }

                //
                //   a
                // b c d
                //   e
                //
                if (ih == 0) {
                    a = ' ';
                } else {
                    a = r->contents[room_no].cell[((ih-1) * w) + iw].c;
                }

                if (iw == 0) {
                    b = ' ';
                } else {
                    b = r->contents[room_no].cell[(ih * w) + (iw-1)].c;
                }

                if (ih == h - 1) {
                    e = ' ';
                } else {
                    e = r->contents[room_no].cell[((ih+1) * w) + iw].c;
                }

                if (iw == w - 1) {
                    d = ' ';
                } else {
                    d = r->contents[room_no].cell[(ih * w) + (iw+1)].c;
                }

                //
                // Allow inner doors
                //
                if ((a != ' ') && (b != ' ') && (e != ' ') && (d != ' ')) {
                    continue;
                }

                //
                // Ensure we don't look under rooms characters i.e. don't
                // be fooled by the chars we're about to replace.
                //
                if (a == ' ') {
                    a = map[cx][cy-1];
                }
                if (b == ' ') {
                    b = map[cx-1][cy];
                }
                if (d == ' ') {
                    d = map[cx+1][cy];
                }
                if (e == ' ') {
                    e = map[cx][cy+1];
                }

                bool ok = false;

                if ((a == GH_MAP_CORR) ||
                    (a == GH_MAP_DUSTY_CORR) ||
                    (b == GH_MAP_CORR) ||
                    (b == GH_MAP_DUSTY_CORR) ||
                    (d == GH_MAP_CORR) ||
                    (d == GH_MAP_DUSTY_CORR) ||
                    (e == GH_MAP_CORR) ||
                    (e == GH_MAP_DUSTY_CORR)) {
                    ok = true;
                }

                //
                // No rooms on rooms!
                //
                //
                for (int dx = 1; dx < 3; dx++) {
                    for (int dy = 1; dy < 3; dy++) {
                        if (rooms[cx-dx][cy-dy]) {
                            ok = false;
                        }
                    }
                }

                if (!ok) {
                    if (tries > 1000) {
                        return (false);
                    }

                    goto redo;
                }
            }
        }

        //
        // Place the room here
        //
        for (int iw = 0; iw < w; iw++) {
            for (int ih = 0; ih < h; ih++) {
                char c = r->contents[room_no].cell[(ih * w) + iw].c;
                char old_c = map[x + iw][y + ih];

                if (c != ' ') {
                    if ((c != GH_MAP_ROOM_WALL) && (c != GH_MAP_ROOM)) {
                        map_obj[x + iw][y + ih] = c;
                        map[x + iw][y + ih] = GH_MAP_ROOM;
                    } else {
                        map[x + iw][y + ih] = c;
                    }

                    //
                    // Make some of the room walls secret doors.
                    //
                    if ((old_c == GH_MAP_CORR) || 
                        (old_c == GH_MAP_DUSTY_CORR)) {
                        if (c == GH_MAP_ROOM_WALL) {
                            if (myrand(100) < 2) {
                                map[x + iw][y + ih] = GH_MAP_SECRET_WALL;
                            }
                        }
                    }

                    rooms[x + iw][y + ih] = r;
                }
            }
        }

        return (true);
    }
}

//
// Place a number of non random rooms
//
void gh_maze::place_fixed_rooms (void)
{
    GH_FUNCTION()

    int tries = 0;

    while (++tries < 4) {
        gh_room *r;
        
        r = gh_room::choose_for_level(level);
        if (!r) {
            continue;
        }

        room_fixed_place_in_maze(r);
    }
}

//
// Place a shop
//
void gh_maze::place_shop (void)
{
    GH_FUNCTION()

    int tries = 0;

    while (++tries < 5) {
        gh_room *r;
        
        r = gh_room::choose_shop_for_level(level);
        if (!r) {
            continue;
        }

        room_fixed_place_in_maze(r);

        return;
    }
}

//
// We've found a corridor that borders a room. Add doors alongside all
// other room blocks that adjoin this corridor.
//
void gh_maze::place_doors_along_corridor (int x, int y)
{
    GH_FUNCTION()

    if (map_searched[x][y]) {
        return;
    }

    if ((map[x][y] != GH_MAP_CORR) &&
        (map[x][y] != GH_MAP_DUSTY_CORR) &&
        (map[x][y] != GH_MAP_DOOR)) {
        return;
    }

    map_searched[x][y] = 1;

    if ((map[x]  [y-1] == GH_MAP_ROOM)) {
        if (map_obj[x][y-1] != GH_MAP_DOOR) {
            if (myrand(100) < 70) {
                map[x][y-1] = GH_MAP_ROOM_WALL;
            } else {
                map_obj[x][y-1] = GH_MAP_DOOR;
            }
        }
    }

    if ((map[x]  [y+1] == GH_MAP_ROOM)) {
        if (map_obj[x][y+1] != GH_MAP_DOOR) {
            if (myrand(100) < 70) {
                map[x][y+1] = GH_MAP_ROOM_WALL;
            } else {
                map_obj[x][y+1] = GH_MAP_DOOR;
            }
        }
    }

    if ((map[x+1][y] == GH_MAP_ROOM)) {
        if (map_obj[x+1][y] != GH_MAP_DOOR) {
            if (myrand(100) < 70) {
                map[x+1][y] = GH_MAP_ROOM_WALL;
            } else {
                map_obj[x+1][y] = GH_MAP_DOOR;
            }
        }
    }

    if ((map[x-1][y] == GH_MAP_ROOM)) {
        if (map_obj[x-1][y] != GH_MAP_DOOR) {
            if (myrand(100) < 70) {
                map[x-1][y] = GH_MAP_ROOM_WALL;
            } else {
                map_obj[x-1][y] = GH_MAP_DOOR;
            }
        }
    }

    place_doors_along_corridor(x-1,y);
    place_doors_along_corridor(x+1,y);
    place_doors_along_corridor(x,y-1);
    place_doors_along_corridor(x,y+1);
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
void gh_maze::place_doors (void)
{
    GH_FUNCTION()

    int total_doors = 2;

    if (level < 5) {
        total_doors = myrand(GH_MAP_DOORS_AMOUNT) + 2;
    } else if (level < 10) {
        total_doors = myrand(GH_MAP_DOORS_AMOUNT) + 4;
    } else if (level < 50) {
        total_doors = myrand(GH_MAP_DOORS_AMOUNT) + 6;
    } else {
        total_doors = myrand(GH_MAP_DOORS_AMOUNT) + 8;
    }

    while (total_doors) {
        int tries = 0;

        while (tries++ < 1000) {
            int x = myrand(maze_w - 2) + 1;
            int y = myrand(maze_h - 2) + 1;
            bool ok;

            ok = false;

            //
            // No doors in shops.
            //
            if (rooms[x][y]) {
                if (rooms[x][y]->is_shop) {
                    continue;
                }
            }

            //
            // No doors on top of obstacles, like return exits.
            //
            if (map_obstacle[x][y]) {
                continue;
            }

            if (map[x][y] == ' ') {
                //
                // Don't plaze doors next to a corridor
                //
                if ((map[x-1][y-1] == GH_MAP_CORR) ||
                    (map[x]  [y-1] == GH_MAP_CORR) ||
                    (map[x+1][y-1] == GH_MAP_CORR) ||
                    (map[x-1][y]   == GH_MAP_CORR) ||
                    (map[x]  [y]   == GH_MAP_CORR) ||
                    (map[x+1][y]   == GH_MAP_CORR) ||
                    (map[x-1][y+1] == GH_MAP_CORR) ||
                    (map[x]  [y+1] == GH_MAP_CORR) ||
                    (map[x+1][y+1] == GH_MAP_CORR)) {
                    continue;
                }

                //
                // Don't plaze doors next to a corridor
                //
                if ((map[x-1][y-1] == GH_MAP_DUSTY_CORR) ||
                    (map[x]  [y-1] == GH_MAP_DUSTY_CORR) ||
                    (map[x+1][y-1] == GH_MAP_DUSTY_CORR) ||
                    (map[x-1][y]   == GH_MAP_DUSTY_CORR) ||
                    (map[x]  [y]   == GH_MAP_DUSTY_CORR) ||
                    (map[x+1][y]   == GH_MAP_DUSTY_CORR) ||
                    (map[x-1][y+1] == GH_MAP_DUSTY_CORR) ||
                    (map[x]  [y+1] == GH_MAP_DUSTY_CORR) ||
                    (map[x+1][y+1] == GH_MAP_DUSTY_CORR)) {
                    continue;
                }

                ok = (myrand(1000) < 2);
            } else if (map[x][y] == GH_MAP_CORR) {
                continue;
            } else if (map[x][y] == GH_MAP_DUSTY_CORR) {
                continue;
            } else if (map[x][y] == GH_MAP_ROOM) {
                maze_room_info_t *room;

                room = &maze_room_info[(int)room_number_map[x][y]];

                //
                // Only place a door in a room when it's next to a
                // corridor!
                //
                if ((map[x]  [y-1] == GH_MAP_DUSTY_CORR) ||
                    (map[x-1][y]   == GH_MAP_DUSTY_CORR) ||
                    (map[x+1][y]   == GH_MAP_DUSTY_CORR) ||
                    (map[x]  [y+1] == GH_MAP_DUSTY_CORR)) {
                    ok = true;
                }

                if ((map[x]  [y-1] == GH_MAP_CORR) ||
                    (map[x-1][y]   == GH_MAP_CORR) ||
                    (map[x+1][y]   == GH_MAP_CORR) ||
                    (map[x]  [y+1] == GH_MAP_CORR)) {
                    ok = true;
                }
            
                if (!ok) {
                    continue;
                }

                ok = false;

                if (room->dusty_corridors) {
                    ok = true;
                } else if (room->corridors) {
                    ok = (myrand(100) < 80);
                } else if (!room->corridors) {
                    ok = true;
                }
            } else {
                continue;
            }

            if (!ok) {
                continue;
            }

            if (map_obj[x][y]) {
                continue;
            }

            if (map_obstacle[x][y]) {
                //
                // Don't place on top of an obstacle, like the player
                // or maze exit
                //
                continue;
            }

            if (map[x][y] == GH_MAP_ROOM) {
                map_obj[x][y] = GH_MAP_DOOR;
            } else {
                if (myrand(1000) < 10) {
                    //
                    // No hidden exits in shops; too easy 8)
                    //
                    if (rooms[x][y]) {
                        if (rooms[x][y]->is_shop) {
                            continue;
                        }
                    }

                    map[x][y] = GH_MAP_HIDDEN_EXIT;
                }
            }

            memset(map_searched, 0, sizeof(map_searched));

            if ((map[x]  [y-1] == GH_MAP_DUSTY_CORR)) {
                place_doors_along_corridor(x, y - 1);
            }
            if ((map[x]  [y+1] == GH_MAP_DUSTY_CORR)) {
                place_doors_along_corridor(x, y + 1);
            }
            if ((map[x-1][y] == GH_MAP_DUSTY_CORR)) {
                place_doors_along_corridor(x - 1, y);
            }
            if ((map[x+1][y] == GH_MAP_DUSTY_CORR)) {
                place_doors_along_corridor(x + 1, y);
            }
            if ((map[x]  [y-1] == GH_MAP_CORR)) {
                place_doors_along_corridor(x, y - 1);
            }
            if ((map[x]  [y+1] == GH_MAP_CORR)) {
                place_doors_along_corridor(x, y + 1);
            }
            if ((map[x-1][y] == GH_MAP_CORR)) {
                place_doors_along_corridor(x - 1, y);
            }
            if ((map[x+1][y] == GH_MAP_CORR)) {
                place_doors_along_corridor(x + 1, y);
            }

            break;
        }

        total_doors--;
    }
}

//
// Trim all corridor fragments that wholly live in a room
//
void gh_maze::fixup_doors (void)
{
    GH_FUNCTION()

    for (int y = 1; y < maze_h - 1; y++) {
        for (int x = 1; x < maze_w - 1; x++) {

            //
            //  X...
            //  XD..
            // ###D.
            //    X.
            //
            //
            //
            //  X...
            //  XDD.
            // ###D.
            //    X.
            //
            if (map[x][y] != GH_MAP_ROOM) {
                continue;
            }

            if ((map_obj[x-1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y+1] == GH_MAP_DOOR) &&
                (map[x-1][y+1] == GH_MAP_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            if ((map_obj[x+1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y+1] == GH_MAP_DOOR) &&
                (map[x+1][y+1] == GH_MAP_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            if ((map_obj[x-1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y-1] == GH_MAP_DOOR) &&
                (map[x-1][y-1] == GH_MAP_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            if ((map_obj[x+1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y-1] == GH_MAP_DOOR) &&
                (map[x+1][y-1] == GH_MAP_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            //
            //  X...
            //  XD..
            // ###D.
            //    X.
            //
            //
            //
            //  X...
            //  XDD.
            // ###D.
            //    X.
            //
            if ((map_obj[x-1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y+1] == GH_MAP_DOOR) &&
                (map[x-1][y+1] == GH_MAP_DUSTY_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            if ((map_obj[x+1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y+1] == GH_MAP_DOOR) &&
                (map[x+1][y+1] == GH_MAP_DUSTY_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            if ((map_obj[x-1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y-1] == GH_MAP_DOOR) &&
                (map[x-1][y-1] == GH_MAP_DUSTY_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }

            if ((map_obj[x+1][y] == GH_MAP_DOOR) &&
                (map_obj[x][y-1] == GH_MAP_DOOR) &&
                (map[x+1][y-1] == GH_MAP_DUSTY_CORR)) {
                if (myrand(100) < 50) {
                    map[x][y] = GH_MAP_ROOM_WALL;
                } else {
                    map_obj[x][y] = GH_MAP_DOOR;
                }
            }
        }
    }
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
void gh_maze::place_traps (void)
{
    GH_FUNCTION()

    int total_traps = 2;

    if (level < 5) {
        total_traps = myrand(GH_MAP_TRAPS_AMOUNT) + 2;
    } else if (level < 10) {
        total_traps = myrand(GH_MAP_TRAPS_AMOUNT) + 4;
    } else if (level < 50) {
        total_traps = myrand(GH_MAP_TRAPS_AMOUNT) + 6;
    } else {
        total_traps = myrand(GH_MAP_TRAPS_AMOUNT) + 8;
    }

    while (total_traps) {
        int x = myrand(maze_w - 2) + 1;
        int y = myrand(maze_h - 2) + 1;
        bool ok;

        ok = false;

        if (rooms[x][y]) {
            if (rooms[x][y]->is_shop) {
                continue;
            }
        }

        if (map[x][y] == ' ') {
            //
            // No traps outwith the dungeon
            //
            continue;
        } else if (map[x][y] == GH_MAP_CORR) {
            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_DUSTY_CORR) {
            ok = (myrand(100) < 40);
        } else if (map[x][y] == GH_MAP_CAVE_DIRT) {
            ok = (myrand(100) < 50);
        } else if (map[x][y] == GH_MAP_DIRT) {
            ok = (myrand(100) < 5);
        } else if (map[x][y] == GH_MAP_GRASS) {
            ok = (myrand(100) < 1);
        } else if (map[x][y] == GH_MAP_VOID_FLOOR) {
            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_ROOM) {
            maze_room_info_t *room;

            room = &maze_room_info[(int)room_number_map[x][y]];

            if (room->dusty_corridors) {
                ok = true;
            } else if (room->corridors) {
                ok = (myrand(100) < 80);
            } else if (!room->corridors) {
                ok = true;
            }
        } else {
            continue;
        }

        if (!ok) {
            continue;
        }

        if (map_obj[x][y]) {
            continue;
        }

        if (map_obstacle[x][y]) {
            //
            // Don't place on top of an obstacle, like the player
            // or maze exit
            //
            continue;
        }

        map_obj[x][y] = GH_MAP_TRAP;

        total_traps--;
    }
}

//
// Place an object that you can't walk over without something happening;
// an important object
//
void gh_maze::place_monst (void)
{
    GH_FUNCTION()

    int total_monst = 2;

    if (level < 5) {
        total_monst = myrand(GH_MAP_MONST_AMOUNT) + 2;
    } else if (level < 10) {
        total_monst = myrand(GH_MAP_MONST_AMOUNT) + 10;
    } else if (level < 50) {
        total_monst = myrand(GH_MAP_MONST_AMOUNT) + 20;
    } else if (level < 90) {
        total_monst = myrand(GH_MAP_MONST_AMOUNT) + 30;
    } else {
        total_monst = myrand(GH_MAP_MONST_AMOUNT) + 40;
    }

    while (total_monst) {
        int x = myrand(maze_w - 2) + 1;
        int y = myrand(maze_h - 2) + 1;
        bool ok;

        ok = false;

        if (rooms[x][y]) {
            if (rooms[x][y]->is_shop) {
                continue;
            }
        }

        if (map[x][y] == ' ') {
            //
            // No traps outwith the dungeon
            //
            continue;
        } else if (map[x][y] == GH_MAP_CORR) {
            ok = (myrand(100) < 20);
        } else if (map[x][y] == GH_MAP_DUSTY_CORR) {
            ok = (myrand(100) < 30);
        } else if (map[x][y] == GH_MAP_CAVE_DIRT) {
            ok = (myrand(100) < 30);
        } else if (map[x][y] == GH_MAP_DIRT) {
            ok = (myrand(100) < 30);
        } else if (map[x][y] == GH_MAP_GRASS) {
            ok = (myrand(100) < 30);
        } else if (map[x][y] == GH_MAP_VOID_FLOOR) {
            ok = (myrand(100) < 40);
        } else if (map[x][y] == GH_MAP_ROOM) {
            maze_room_info_t *room;

            room = &maze_room_info[(int)room_number_map[x][y]];

            if (room->dusty_corridors) {
                ok = true;
            } else if (room->corridors) {
                ok = (myrand(100) < 80);
            } else if (!room->corridors) {
                ok = true;
            }
        } else {
            continue;
        }

        if (!ok) {
            continue;
        }

        if (map_obj[x][y]) {
            continue;
        }

        //
        // Don't place on top of an obstacle, like the player
        // or maze exit
        //
        for (int dx = -2; dx < 2; dx++) {
            for (int dy = -2; dy < 2; dy++) {
                if (map_obstacle[x+dx]  [y+dy]) {
                    goto redo;
                }
            }
        }

        map_obj[x][y] = GH_MAP_MONST;

        total_monst--;
redo:
        continue;
    }
}

//
// Ensure that the maze is solvable
//
// 0 on ok, 1 on err
//
bool gh_maze::maze_test_from (int x, int y)
{
    if (map_searched[x][y]) {
        return (false);
    }

    map_searched[x][y] = 1;

    if (map_obstacle[x][y] == GH_MAP_EXIT) {
        return (true);
    }

    /*
     * Doors are obstacles, but we can get through them.
     */
    if (map_obstacle[x][y] != GH_MAP_START) {
        if (map_obstacle[x][y]) {
            return (true);
        }
    }

    if (gh_maze_can_search(map[x-1][y])) {
        if (maze_test_from(x-1, y)) {
            return (true);
        }
    }

    if (gh_maze_can_search(map[x+1][y])) {
        if (maze_test_from(x+1, y)) {
            return (true);
        }
    }

    if (gh_maze_can_search(map[x][y-1])) {
        if (maze_test_from(x, y-1)) {
            return (true);
        }
    }

    if (gh_maze_can_search(map[x][y+1])) {
        if (maze_test_from(x, y+1)) {
            return (true);
        }
    }

    return (false);
}

//
// Ensure that the maze is solvable
//
bool gh_maze::maze_test (void)
{
    GH_FUNCTION()

    for (int y = 0; y < maze_h; y++) {
        for (int x = 0; x < maze_w; x++) {
            if (map_obstacle[x][y] == GH_MAP_START) {
                if (!maze_test_from(x,y)) {
                    return (false);
                }

                return (true);
            }
        }
    }

    return (false);
}

//
// Smooth out useless lumps in the maze.
//
int gh_maze::maze_smooth_bumps (void)
{
    GH_FUNCTION()

    int got;

    got = 0;
redo:
    for (int y = 1; y < maze_h-1; y++) {
        for (int x = 1; x < maze_w-1; x++) {

            //
            // ........
            // .++++++.
            // ++....++
            //
            if (!(
                   (map[x-1][y]   == ' ') &&
                   (map[x]  [y-1] == ' ') &&
                   (map[x]  [y]   == GH_MAP_CORR) &&
                   (map[x]  [y+1] == GH_MAP_CORR) &&
                   (map[x+1][y-1] == ' ') &&
                   (map[x+1][y]   == GH_MAP_CORR) &&
                   (map[x+1][y+1] == ' '))
                ) {
                continue;
            }

            int bx;

            for (bx = x + 1; bx < maze_w-1; bx++) {
               if (!(
                    (map[bx][y-1] == ' ') &&
                    (map[bx][y]   == GH_MAP_CORR) &&
                    (map[bx][y+1] == ' ') &&
                    (map[bx][y+2] == ' ')
                    )
                  ) {
                   break;
               }
            }

            if (!(
                   (map[bx-1][y-1] == ' ') &&
                   (map[bx-1][y]   == GH_MAP_CORR) &&
                   (map[bx-1][y+1] == ' ') &&
                   (map[bx]  [y-1] == ' ') &&
                   (map[bx]  [y]   == GH_MAP_CORR) &&
                   (map[bx]  [y+1] == GH_MAP_CORR) &&
                   (map[bx+1][y]   == ' '))
               ) {
                continue;
            }

            while (x <= bx) {
               map[x][y] = ' ';
               map[x][y+1] = GH_MAP_CORR;
               x++;
            }

            got++;

            goto redo;
        }
    }

    return (got);
}

//
// Attempt to remove loops in corridors
//
int gh_maze::maze_remove_stubs (void)
{
    GH_FUNCTION()

    int got;

    got = 0;

    for (int y = 1; y < maze_h-2; y++) {
        for (int x = 1; x < maze_w-2; x++) {

            //
            // ....
            // .++.
            //
            if (
                   (map[x-1][y-1]   == ' ') &&
                   (map[x]  [y-1]   == ' ') &&
                   (map[x+1][y-1]   == ' ') &&
                   (map[x+2][y-1]   == ' ') &&

                   (map[x-1][y]     == ' ') &&
                   (map[x]  [y]     == GH_MAP_CORR) &&
                   (map[x+1][y]     == GH_MAP_CORR) &&
                   (map[x+2][y]     == ' ')
                ) {

               map[x][y] = ' ';
               map[x+1][y] = ' ';

               got++;
            }
        }
    }

    return (got);
}

//
// Smooth out bumps and useless paths that would bore the dungeoneer
//
void gh_maze::maze_smooth (void)
{
    GH_FUNCTION()

    int got;

    got = 1;

    while (got) {
        got = 0;

        for (int r=0; r<4; r++) {
            maze_rotate();
            got += maze_smooth_bumps();
            got += maze_remove_stubs();

            //
            // Too annoying to have
            //
            got += maze_remove_deadends();

            got += maze_remove_corridor_fragments();
        }
    }
}

//
// Generate a maze!
//
void gh_maze::maze_gen (void)
{
    GH_FUNCTION()

    int teleports;
    int start_room;
    int end_room;

redo:
    teleports = 0;
    number_of_rooms = 0;

    maze_w = GH_MIN(GH_MAP_WIDTH + (rand() % 
                                        ((level/5)+1)), GH_MAX_MAZE_SIZE - 2);
    maze_h = GH_MIN(GH_MAP_HEIGHT + (rand() % 
                                        ((level/5)+1)), GH_MAX_MAZE_SIZE - 2);
    maze_w = GH_MAX(maze_w, GH_MIN_MAZE_SIZE);
    maze_h = GH_MAX(maze_h, GH_MIN_MAZE_SIZE);

    maze_reset();

    //
    // Create some corridors and center them in the maze.
    //
    maze_corridors();

    maze_recenter();

    //
    // Wallify the rooms and pad it out to make it safe to explore.
    //
    maze_pad();
    maze_border();

//    GH_LOG("Corridors:");
//    maze_print();

    place_fixed_rooms();
    place_shop();

//    GH_LOG("Fixed rooms:");
//    maze_print();

    int total_rooms = 2;

    if (level < 5) {
        total_rooms = myrand(GH_MAP_ROOM_AMOUNT) + 2;
    } else if (level < 10) {
        total_rooms = myrand(GH_MAP_ROOM_AMOUNT) + 4;
    } else if (level < 50) {
        total_rooms = myrand(GH_MAP_ROOM_AMOUNT) + 10;
    } else if (level < 90) {
        total_rooms = myrand(GH_MAP_ROOM_AMOUNT) + 15;
    } else {
        total_rooms = myrand(GH_MAP_ROOM_AMOUNT) + 20;
    }

    //
    // Create some rooms over these corridors.
    //
    int r = 0;
    int tries = 0;

    while (r < total_rooms) {
        int sx;
        int sy;
        int ex;
        int ey;

        //
        // Create a randomly shaped room.
        //
        room_gen();

        //
        // Try to add this room to the maze.
        //
        if (!room_place_in_maze(&sx, &sy, &ex, &ey)) {
            if (tries++ > 50) {
//              maze_print();
//              GH_LOG("failed to place in maze");
                break;
            }

            continue;
        }

        //
        // Use walls to seperate the rooms.
        //
        maze_wallify(sx, sy, ex, ey);

        r++;
    }

    //
    // Get rid of needless loops in the maze.
    //
    maze_smooth();

    //
    // Removing dead end corridors can leave holes in the walls.
    // Make sure they're patched up.
    //
    maze_wallify(1, 1, maze_w - 1, maze_h - 1);

    while (maze_remove_corridor_fragments()) {
    }

    maze_wallify2(1, 1, maze_w - 1, maze_h - 1);

    number_of_rooms = maze_count_rooms();
    if (number_of_rooms < 2) {
        /*
         * Not enough rooms. Redo.
         */
#ifdef DEBUG
        fprintf(stdout, "\nNot enough rooms:\n");
#endif
        goto redo;
    }

    maze_hide_doors();

    teleports = maze_add_deadend_teleports();

    int teleport_room;

    if (teleports || (myrand(100) < 10)) {
        int max = 2 + myrand(level / 10);
        int tries = 0;

        while (teleports < max) {
            tries++;
            if (tries > 20) {
                /*
                 * No place to place matching telepors. Redo.
                 */
                if (teleports > 2) {
                    break;
                }

#ifdef DEBUG
                fprintf(stdout, "\nNo place for teleport:\n");
#endif
                goto redo;
            }

            if ((teleport_room = room_place_obstacle(GH_MAP_TELEPORT, -1))) {
                teleports++;
                if (!room_place_obstacle(GH_MAP_TELEPORT, teleport_room)) {
                    /*
                     * No place to place matching telepors. Redo.
                     */
#ifdef DEBUG
                    fprintf(stdout, "\nNo place for matching teleport:\n");
#endif
                    goto redo;
                }
            }
        }
    }

//    GH_LOG("Wallified corridors:");
//    maze_print();

    maze_wallify_corr();
    place_traps();
    place_monst();
    place_doors();
    fixup_doors();
    place_treasure();
    place_potion();
    place_food();

    start_room = room_place_obstacle(GH_MAP_START, -1);
    if (!start_room) {
        /*
         * No place to place the maze start. Redo.
         */
#ifdef DEBUG
        fprintf(stdout, "\nNo maze start placed:\n");
#endif
        goto redo;
    }

    end_room = room_place_obstacle(GH_MAP_EXIT, start_room);
    if (!end_room) {
        /*
         * No place to place the maze end. Redo.
         */
#ifdef DEBUG
        fprintf(stdout, "\nNo maze end placed:\n");
#endif
        goto redo;
    }

    if (start_room == end_room) {
        /*
         * No start and end in the same room.
         */
        if (myrand(100) > 5) {
            /*
             * Most of the time.
             */
#ifdef DEBUG
            fprintf(stdout, "\nNo start and end in same room:\n");
#endif
            goto redo;
        }
    }

    if (!maze_test()) {
        /*
         * No solution for the maze!
         */
#ifdef DEBUG
        fprintf(stdout, "\nNo solution:\n");
#endif
        goto redo;
    }

#ifdef DEBUG
    fprintf(stdout, "\nAll done:\n");
#endif
    maze_print();
}
