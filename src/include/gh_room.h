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

    $Id: gh_room.h 226 2006-10-22 21:01:19Z goblinhack $
*/

#ifndef _GH_ROOM_H_
#define _GH_ROOM_H_

typedef bool (*gh_room_evaluator_callback)(class gh_room *room, void *arg);

#include <gh_map.h>

typedef struct gh_room_contents_cell_ {
    char            c;
} gh_room_contents_cell;

typedef struct {
    //
    // The dimensions of the character array; we only really
    // need the width.
    //
    int width;
    int height;

    //
    // Characters in this frame
    //
    vector<gh_room_contents_cell> cell;

} gh_room_contents;

class gh_room : public gh_stream
{   
public:
    void *self;

    gh_room(const string gh_cfg_file = "");
    gh_room(class gh_room *);
    ~gh_room(void);

    string              full_name;  // ../resources/rooms/treasure/1
    string              short_name; // treasure/1
    string              name;       // treasure room

    //
    // What are the odds of this room appearing on this level?
    //
    vector<int>         appearing_chance;
    string              upon_enter;
    string              floor_name;
    string              wall_name;
    string              door_name;
    uint8               is_shop:1;
    uint8               is_random:1;
    uint8               is_lit:1;

    vector<gh_room_contents> contents;

    list<gh_map_char> map_chars;

    class gh_thing *shopkeeper;

    class gh_room_registration *reg;

    //
    // Thing bank operations
    //
    typedef map<string, gh_room*> hash_room;
    static hash_room rooms;

    //
    // Find this room in the room bank
    //
    gh_inline static gh_room* find (const string &s1,
                                    const bool err = true)
    {
        gh_room::hash_room::iterator m = rooms.find(s1);

        if (m == rooms.end()) {
            if (err) {
                GH_WARN("couldn't find " + string(s1) + " in loaded list");
            }
            return (NULL);
        }

        gh_room *room = m->second;

        if (room == NULL) {
            if (err) {
                GH_WARN("null room " + string(s1) + " in loaded list");
                return (NULL);
            }
        }

        return (room);
    }

    static gh_room *nth(const int n);
    static size_t how_many(void);
    static void load_all(void);
    static void unload_all(void);
    static gh_room *choose_for_level(int level);
    static gh_room *choose_shop_for_level(int level);

    //
    // gfx.cpp
    //
    void parse(const string file);

    bool parse_block(const int room_no,
                     const char **atp,
                     const char*const end);

    friend void operator>> (gh_stream_in& is, gh_room *me);
    friend void operator<< (gh_stream_out& os, const gh_room *me);
};

#endif /* _GH_ROOM_H_ */
