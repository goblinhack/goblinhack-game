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

    $Id: gh_level.h 514 2007-05-27 23:15:53Z goblinhack $
*/

#ifndef _GH_LEVEL_H_
#define _GH_LEVEL_H_

#include <gh_map.h>

class gh_bonus
{   
public:
    gh_bonus (const string name, const int score)
    {
        this->name = name;
        this->score = score;
        this->score_orig = score;
    }

    ~gh_bonus (void) { }

    string  name;
    int     score;
    int     score_orig;
};

class gh_level : public gh_stream
{   
public:
    void *self;

    gh_level(class gh_game *game = NULL,
             const int level_no = 1,
             const string which_level = "dummy",
             const string level_set = "random",
             const string gh_cfg_file = "",
             const bool want_empty_level = false);

    ~gh_level(void);

    void load(string name);
    void load_pregenerated_level(string name);
    void fixup(void);
    void load_random_level(void);
    void place_water(gh_maze *);
    void load_thing_from_map(class gh_maze *,
                             class gh_room *, 
                             gh_point3d p, 
                             string thing_name);
    void load_maps(void);
    void load_map(const string map_name);
    char map_char_thing_to_char(const string thing_name);
    string map_char_polymorph(const string thing_name);
    string map_char_to_thing(const int level_no,
                             const char map_char,
                             const int line, const int column);
    string map_char_to_random_thing(class gh_room *,
                                    const int level_no,
                                    const char map_char);

    //
    // Find the nth thing of the given character filter name.
    //
    string map_char_nth(const char map_char, int index);

    //
    // How many characters of the given filter exist?
    //
    int map_char_how_many(const char map_char);

    void init(void);
    void tick_hero(void);
    void tick_things(void);
    void tick_things_all(void);
    void scare_monst_all(void);
    void tick(void);
    void reset_all(void);
    void reset(void);
    void add_dead(gh_thing *monst);
    void handle_dead_all(void);
    void level_completed(void);
    void fixup_dynamic_room_for_shops(void);

    bool                    valid;
    uint32                  tick_at;
    string                  desc;
    int                     level_no;
    string                  which_level;
    string                  level_set_name;
    class gh_map            map;
    list <gh_map_char>      map_chars;

    //
    // All things bright and beautiful.
    //
    list<class gh_thing*>   all_things;

    //
    // All things on the map, not necessarily all things though
    //
    list<class gh_thing*>   map_things;

    //
    // All things that we run on each tick.
    //
    // So this filters boring stuff like rocks that don't do anything.
    //
    list<class gh_thing*>   does_stuff_things;

    //
    // Things not on the map, but in the afterlife
    //
    list<class gh_thing*>   dead_things;
    list<class gh_room*>    rooms;

    gh_point3d              draw_start;
    gh_point3d              draw_end;

    //
    // On edited levels where does yonder player begin?
    //
    gh_point3d              hero_start_where;
    gh_point3d              hero_end_where;

    class gh_game           *game;

    //
    // All active explosions.
    //
    vector<class gh_explosion *> explosions;

    //
    // All bonuses collected.
    //
    vector<gh_bonus>        bonuses;

    //
    // We use this level creation time to allow the player a bit of
    // time before the monsters move.
    //
    uint32                  created_when;
    int                     time_left;

    //
    // Bonus upon the thing being finally killed 
    //
    int                     level_completed_bonus;

    //
    // All things except explosions, can watch but can't move
    //
    uint32                  freeze_things:1;
    uint32                  is_level_completed:1;
    uint32                  is_last_level:1;
    uint32                  is_cave:1;
    uint32                  is_forest:1;
    uint32                  is_landscape:1;
    uint32                  is_dungeon:1;
    uint32                  is_void:1;
    uint32                  is_hell:1;
    uint32                  is_bonus:1;

    //
    // First time we've entered this level?
    //
    uint32                  first_enter:1;
    uint32                  scare_monsts:1;
    uint32                  has_shop:1;
    uint32                  being_deleted:1;
    uint32                  is_editing:1;

    //
    // Level editor stuff
    //
    int                     is_editing_which_thing[255];
    int                     is_editing_grid_res;
    uint8                   is_editing_map_char;
    gh_point3d              cursor_at;
    gh_thing                *cursor;

    //
    // Tiles we use for an entire level
    //
    string                  corridor_name;
    string                  corridor_wall_name;

    friend void operator>> (gh_stream_in& is, gh_level *me);
    friend void operator<< (gh_stream_out& os, const gh_level *me);
};

#endif /* _GH_LEVEL_H_ */
