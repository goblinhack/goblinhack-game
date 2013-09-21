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

    $Id: gh_maze.h 610 2007-12-16 21:41:22Z goblinhack $
*/

#ifndef _GH_MAZE_H_
#define _GH_MAZE_H_

#include <gh_room.h>

//
// Temporary characters used during the creation of the maze.
//
#define  GH_MAP_ROOM_POSSIBLE   'R'

//
// Final characters we output.
//
#define  GH_MAP_SPACE           ' '
#define  GH_MAP_DUSTY_CORR      '?'
#define  GH_MAP_CORR            '#'
#define  GH_MAP_CORR_WALL       '\"'
#define  GH_MAP_BORDER          '@'
#define  GH_MAP_ROOM_WALL       'X'
#define  GH_MAP_ROOM            '.'
#define  GH_MAP_CAVE_DIRT       ','
#define  GH_MAP_DIRT            '%'
#define  GH_MAP_GRASS           '`'
#define  GH_MAP_START           'S'
#define  GH_MAP_RETURN_EXIT     'R'
#define  GH_MAP_EXIT            'E'
#define  GH_MAP_HIDDEN_EXIT     'e'
#define  GH_MAP_TELEPORT        'T'
#define  GH_MAP_TREASURE        '$'
#define  GH_MAP_POTION          'p'
#define  GH_MAP_FOOD            'f'
#define  GH_MAP_TRAP            '_'
#define  GH_MAP_MONST           'm'
#define  GH_MAP_DOOR            'D'
#define  GH_MAP_ROCK            'r'
#define  GH_MAP_ROCK_CRUMBLES   '&'
#define  GH_MAP_LAVA            'L'
#define  GH_MAP_VOID            ':'
#define  GH_MAP_VOID_FLOOR      ';'
#define  GH_MAP_ELECTRO_DOOR    '='
#define  GH_MAP_SECRET_WALL     'x'
#define  GH_MAP_TERRAIN         'F'
#define  GH_MAP_TREE            't'
#define  GH_MAP_WATER1          '/'
#define  GH_MAP_WATER2          '\\'
#define  GH_MAP_WATER3          '|'

enum {
    GH_MAP_LOC_MAP_ROOM,
    GH_MAP_LOC_MAP_CORR,
    GH_MAP_LOC_BURIED,
    GH_MAP_LOC_MAP_DUSTY_CORR,
    GH_MAP_LOC_MAP_DUSTY_ROOM,
    GH_MAP_LOC_HIDDEN_MAP_ROOM,
    GH_MAP_LOC_MAX,
};

typedef struct maze_room_info_t_ {
    int corridors;
    int dusty_corridors;
} maze_room_info_t;

class gh_maze {
public:
    gh_maze (int level, int seed) {
        this->level = level;
        this->seed = seed;

        srand(seed);
    }

    int seed;
    int level;

    int maze_w;
    int maze_h;

    char map[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];
    char map_obstacle[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];
    char map_obj[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];
    char map_searched[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];
    gh_room *rooms[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];

    int number_of_rooms;
    char room_number_map[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];
    char map_save[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];

    int room_w;
    int room_h;
    char room[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];
    char room_save[GH_MAX_MAZE_SIZE][GH_MAX_MAZE_SIZE];

    maze_room_info_t maze_room_info[100];

    void do_room_line(int x0_in, int y0_in, int x1_in, int y1_in, int flag_in);
    void room_line(int x0, int y0, int x1, int y1);
    void maze_reset(void);
    void maze_clean(void);
    void maze_border(void);
    void maze_pad(void);
    void maze_corridors(int cx=0, int cy=0,
                        int tot_number_of_corridors=0,
                        int scale=1,
                        int split_chance=5);
    void maze_print(void);
    int maze_remove_deadends(void);
    int maze_remove_corridor_fragments(void);
    int maze_add_deadend_teleports(void);
    void maze_find_hidden_corridors(int x, int y);
    void maze_recenter(void);
    void maze_backup(void);
    void maze_restore(void);
    void maze_rotate(void);
    int maze_room_eval_border_char(char l, char c, char r);
    int maze_room_eval_char(char c);
    bool room_place_in_maze(int *sx, int *sy, int *ex, int *ey);
    void maze_wallify(int sx, int sy, int ex, int ey);
    void maze_wallify2(int sx, int sy, int ex, int ey);
    void maze_wallify_corr(void);
    void maze_flood_room(int x, int y, int room_no);
    void maze_mark_secret_doors(int x, int y, int room_no);
    int maze_count_rooms(void);
    void maze_hide_doors(void);
    void maze_gen(void);
    void room_print(void);
    void room_reset(void);
    void room_ensure_border_around_room(void);
    void room_trim(void);
    void room_trim_stubs(void);
    void room_scrub(void);
    int room_count(int x, int y, int c);
    void room_mark(int x, int y);
    int room_find_one(void);
    void room_minimize(void);
    void room_backup(void);
    void room_restore(void);
    void room_gen(void);
    int room_place_obstacle(int x, int y, char c, int not_in_this_room);
    int room_place_obstacle(char c, int not_in_this_room);
    void place_treasure(void);
    void place_potion(void);
    void place_obstacle(int obj);
    void place_food(void);
    void place_doors(void);
    void fixup_doors(void);
    void place_doors_along_corridor(int x, int y);
    void place_traps(void);
    void place_monst(void);
    bool maze_test_from(int x, int y);
    bool maze_test(void);
    int maze_smooth_bumps(void);
    int maze_remove_stubs(void);
    void maze_smooth(void);
    bool room_fixed_place_in_maze(gh_room *r);
    void place_fixed_rooms(void);
    void place_shop(void);

    //
    // cave.cpp
    //
    void cave_generation(void);
    void make_cave(void);
    void cave_gen(void);

    //
    // forest.cpp
    //
    void forest_generation(void);
    void make_forest(void);
    void forest_gen(void);

    //
    // landscape.cpp
    //
    void landscape_generation(void);
    void make_landscape(void);
    void landscape_gen(void);

    //
    // hell.cpp
    //
    void hell_generation(void);
    void make_hell(void);
    void hell_gen(void);

    //
    // void.cpp
    //
    void void_generation(void);
    void make_void(void);
    void void_gen(void);

    //
    // bonus.cpp
    //
    void bonus_generation(void);
    void make_bonus(void);
    void bonus_gen(void);

    //
    // Is this somewhere we can maze search?
    //
    gh_inline bool gh_maze_can_search (int type)
    {
        return ((type == GH_MAP_ROOM) ||
                (type == GH_MAP_CORR) ||
                (type == GH_MAP_CAVE_DIRT) ||
                (type == GH_MAP_DIRT) ||
                (type == GH_MAP_GRASS) ||
                (type == GH_MAP_VOID_FLOOR));
    }

    //
    // Is this somewhere we can place obstacles?
    // Like maze start.
    //
    gh_inline bool gh_maze_can_obstacle (int type)
    {
        return ((type == GH_MAP_ROOM) ||
                (type == GH_MAP_CAVE_DIRT) ||
                (type == GH_MAP_DIRT) ||
                (type == GH_MAP_GRASS) ||
                (type == GH_MAP_VOID_FLOOR));
    }

};

#endif
