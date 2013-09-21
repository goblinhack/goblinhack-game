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

    $Id: room.cpp 228 2006-10-22 21:34:30Z goblinhack $
*/

#include <ghack.h>
#include <gh_room.h>
#include <gh_surface.h>
#include <gh_global.h>
#include <gh_collage.h>
#include <gh_math.h>
#include <gh_sdl.h>

gh_room::hash_room gh_room::rooms;

gh_room::gh_room (const string gh_cfg_file)
{
    GH_FUNCTION()

    self                        = this;
    full_name                   = gh_cfg_file;
    short_name                  = gh_cfg_file;

    string_replace(short_name, ".gz", "");
    string_rremove_to_start(short_name, "rooms/");

    name                        = "";
    is_shop                     = false;
    is_random                   = false;
    is_lit                      = false;
    upon_enter                  = "";
    floor_name                  = "";
    door_name                   = "";
    shopkeeper                  = NULL;

    if (gh_cfg_file != "") {
        gh_read_file(this, gh_cfg_file);
    }
}

gh_room::gh_room (class gh_room *r)
{
    GH_FUNCTION()

    this->self                  = this;
    this->full_name             = r->full_name;
    this->short_name            = r->short_name;
    this->name                  = r->name;
    this->appearing_chance      = r->appearing_chance;
    this->upon_enter            = r->upon_enter;
    this->floor_name            = r->floor_name;
    this->door_name             = r->door_name;
    this->shopkeeper            = NULL;
    this->is_shop               = r->is_shop;
    this->is_random             = r->is_random;
    this->is_lit                = r->is_lit;
    this->contents              = r->contents;
    this->map_chars             = r->map_chars;
    this->reg                   = r->reg;
}

gh_room::~gh_room (void)
{
    GH_FUNCTION()

    if (shopkeeper) {
        shopkeeper->decref(GH_THING_REF_SHOPKEEPER);
    }
}

//
// Step through the hash until the nth element
//
gh_room *gh_room::nth (int n)
{
    GH_FUNCTION()

    hash_room::iterator f = rooms.begin();

    while (n-- > 0) {
        if (f == rooms.end()) {
            return (NULL);
        }
        f++;
    }

    if (f == rooms.end()) {
        return (NULL);
    }

    return (f->second);
}

//
// How many rooms are in the hash?
//
size_t gh_room::how_many (void)
{
    GH_FUNCTION()

    return (rooms.size());
}

//
// Nuke all rooms
//
void gh_room::unload_all (void)
{
    GH_FUNCTION()

    GH_LOG("Unloading rooms");
redo:
    hash_room::iterator s = rooms.begin();

    while (s != rooms.end()) {
        gh_room *r = s->second;

        if (!r) {
            GH_THROW("no room on list?");
            continue;
        }

        rooms.erase(s);
        delete r;

        goto redo;
    }
}

void gh_room::load_all (void)
{
    GH_FUNCTION()

    string dir = gh_global::locatedir("rooms");

    if (dir == "") {
        GH_THROW("no rooms dir?");
        return;
    }

    vector<string> files = gh_global::listdir_recursive(dir, "");

    vector<string>::iterator i;

    for (i = files.begin(); i != files.end(); i++) {
        string full_name = *i;
        string short_name = *i;

//      GH_LOG("Load room: " + full_name);

        if (string_contains(short_name, ".map")) {
            continue;
        }

        if (string_contains(short_name, ".tsp")) {
            continue;
        }

        if (string_contains(short_name, ".bmp")) {
            continue;
        }

        if (string_contains(short_name, ".png")) {
            continue;
        }

        string_replace(short_name, ".gz", "");
        string_rremove_to_start(short_name, "rooms/");

//      GH_LOG("          : " + short_name);

        if (rooms[short_name]) {
            GH_THROW(("duplicate named object " + full_name));
        }

        rooms[short_name] = new gh_room(full_name);
    }
}

gh_room *gh_room::choose_for_level (int level_no)
{
    GH_FUNCTION()

    vector <gh_room *> possible;
    string room_name;

    hash_room::iterator ri = rooms.begin();

    while (ri != rooms.end()) {
        gh_room *r = ri->second;

        if (!r) {
            GH_THROW("no room on list?");
            ri++;
            continue;
        }

        if (r->is_shop) {
            ri++;
            continue;
        }

        int appearing_chance = 0;

        //
        // Doth this room want to exist on this level?
        //
        if ((int) r->appearing_chance.size() < level_no) {
            if (!r->appearing_chance.size()) {
                //
                // Some rooms just always appear.
                //
                appearing_chance = 1000;
            } else {
                //
                // Keep using the last value specified if not set.
                //
                appearing_chance =
                        r->appearing_chance[r->appearing_chance.size()-1];
            }
        } else {
            appearing_chance = r->appearing_chance[level_no - 1];
        }

        int rn = gh_math::rand1000();

        if (rn < appearing_chance) {
            possible.push_back(r);

            GH_ROOM_LOG(r, "possible random room");
        }

        ri++;
    }

    if (!possible.size()) {
        return (NULL);
    }

    int i = (rand() % possible.size());

    GH_ROOM_LOG(possible[i], "chose random room");

    return (new gh_room(possible[i]));
}

gh_room *gh_room::choose_shop_for_level (int level_no)
{
    GH_FUNCTION()

    vector <gh_room *> possible;
    string room_name;

    hash_room::iterator ri = rooms.begin();

    while (ri != rooms.end()) {
        gh_room *r = ri->second;

        if (!r) {
            GH_THROW("no room on list?");
            ri++;
            continue;
        }

        if (!r->is_shop) {
            ri++;
            continue;
        }

        int appearing_chance = 0;

        //
        // Doth this room want to exist on this level?
        //
        if ((int) r->appearing_chance.size() < level_no) {
            if (!r->appearing_chance.size()) {
                //
                // Some rooms just always appear.
                //
                appearing_chance = 1000;
            } else {
                //
                // Keep using the last value specified if not set.
                //
                appearing_chance =
                        r->appearing_chance[r->appearing_chance.size()-1];
            }
        } else {
            appearing_chance = r->appearing_chance[level_no - 1];
        }

        int rn = gh_math::rand1000();

        if (rn < appearing_chance) {
            possible.push_back(r);

            GH_ROOM_LOG(r, "possible random room");
        }

        ri++;
    }

    if (!possible.size()) {
        return (NULL);
    }

    int i = (rand() % possible.size());

    GH_ROOM_LOG(possible[i], "chose random room");

    return (new gh_room(possible[i]));
}

void operator>> (gh_stream_in& is, gh_room *me)
{
    GH_FUNCTION()

    string tmp;

    me->get_unnamed(is, &me->name);
    if (me->short_name == "") {
        me->get_unnamed(is, &me->short_name);
        me->get_unnamed(is, &me->full_name);
    }

    //
    // Load the map file that tells us which tiles to use in anim
    // frames
    //
    string map_file =
                gh_global::locate("rooms/" + me->short_name + ".map");

    if (map_file != "") {
        me->parse(map_file);
    }

    bool tmp_bool;
    bool got_one                        = true;
    bool self_set                       = false;
    bool appearing_chance_set           = false;
    bool is_shop_set                    = false;
    bool is_random_set                  = false;
    bool is_lit_set                     = false;
    bool upon_enter_set                 = false;
    bool floor_name_set                 = false;
    bool door_name_set                  = false;
    bool shopkeeper_set                 = false;

    while (got_one) {
        got_one = false;

        //
        // self
        //
        if (!self_set) {
            self_set =
                me->get_opt(is, &me->self, "self", (void*)NULL);

            got_one |= self_set;
        }

        //
        // appearing_chance
        //
        if (!appearing_chance_set) {
            appearing_chance_set =
                me->get_opt_container(is, &me->appearing_chance,
                                      "appearing_chance");

            got_one |= appearing_chance_set;
        }

        //
        // is_shop
        //
        if (!is_shop_set) {
            is_shop_set =
                me->get_opt_named_flag(is, &tmp_bool, "is_shop", false);

            got_one |= is_shop_set;
            me->is_shop = tmp_bool;
        }

        //
        // is_random
        //
        if (!is_random_set) {
            is_random_set =
                me->get_opt_named_flag(is, &tmp_bool, "is_random", false);

            got_one |= is_random_set;
            me->is_random = tmp_bool;
        }

        //
        // is_lit
        //
        if (!is_lit_set) {
            is_lit_set =
                me->get_opt_named_flag(is, &tmp_bool, "is_lit", false);

            got_one |= is_lit_set;
            me->is_lit = tmp_bool;
        }

        //
        // upon_enter
        //
        if (!upon_enter_set) {
            upon_enter_set = me->get_opt(is, &me->upon_enter, "upon_enter",
                                         string(""));

            got_one |= upon_enter_set;
        }

        //
        // floor_name
        //
        if (!floor_name_set) {
            floor_name_set = me->get_opt(is, &me->floor_name, "floor_name",
                                         string(""));

            got_one |= floor_name_set;
        }

        //
        // door_name
        //
        if (!door_name_set) {
            door_name_set = me->get_opt(is, &me->door_name, "door_name",
                                         string(""));

            got_one |= door_name_set;
        }

        //
        // shopkeeper
        //
        if (!shopkeeper_set) {
            shopkeeper_set = me->get_opt_classpp(is, &me->shopkeeper, "shopkeeper");
            got_one |= shopkeeper_set;
        }

        if (!got_one) {
            if (is.node->type == gh_node::GH_NODE_KET) {
                break;
            }
        
            if (is.node->type == gh_node::GH_NODE_EOF) {
                break;
            }

            me->get(is, &tmp_bool, "error");
        }
    }
}

void operator<< (gh_stream_out& os, const gh_room *me)
{
    GH_FUNCTION()

    bool tmp_bool;

    me->put_unnamed(os, me->name.c_str());
    me->put_unnamed(os, me->short_name.c_str());
    me->put_unnamed(os, me->full_name.c_str());

    //
    // self
    //
    me->put(os, &me->self, "self");

    //
    // appearing_chance
    //
    me->put_container(os, &me->appearing_chance, "appearing_chance");

    //
    // is_shop
    //
    tmp_bool = me->is_shop;
    me->put_opt_named_flag(os, &tmp_bool, "is_shop", false);

    //
    // is_random
    //
    tmp_bool = me->is_random;
    me->put_opt_named_flag(os, &tmp_bool, "is_random", false);

    //
    // is_lit
    //
    tmp_bool = me->is_lit;
    me->put_opt_named_flag(os, &tmp_bool, "is_lit", false);

    //
    // owner
    //
    me->put_opt_classpp(os, &me->shopkeeper, "shopkeeper");

    //
    // upon_enter
    //
    me->put_opt(os, &me->upon_enter, "upon_enter", string(""));

    //
    // floor_name
    //
    me->put_opt(os, &me->floor_name, "floor_name", string(""));

    //
    // door_name
    //
    me->put_opt(os, &me->door_name, "door_name", string(""));
}
