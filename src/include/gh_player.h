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

    $Id: gh_player.h 610 2007-12-16 21:41:22Z goblinhack $
*/

#ifndef _GH_PLAYER_H_
#define _GH_PLAYER_H_

#include "gh_subsys.h"
#include "gh_screen.h"
#include "gh_sdl.h"

//
// Not to be confused with the carrying list, this is a 
// list of unique items being carried. i.e. 10 potions
// of slimeaway would appear as one item here. This is
// the list we display on screen.
//
class gh_item {
public:
    gh_item(string name, int sort, int how_many, int row) :
            name(name), sort(sort), how_many(how_many), row(row)
    {
        is_chargeable = false;
        is_weapon = false;
        is_sword = false;
        is_staff = false;
        is_potion = false;
        is_spell = false;
        is_wand = false;
        is_axe = false;
        is_warhammer = false;
        is_bow = false;
        is_gun = false;
        is_armour = false;
        is_amulet = false;
        is_cursed = false;
        curse_noticed = false;
        is_stolen = false;
        is_unpaid_for = false;
        is_heavy = false;
        is_vheavy = false;
        is_damaged = false;
        shortcut_key = -1;
    }

    string name;
    int sort;
    int how_many;
    int row;
    int item_no;
    int shortcut_key;
    uint32 is_chargeable:1;
    uint32 is_weapon:1;
    uint32 is_sword:1;
    uint32 is_staff:1;
    uint32 is_potion:1;
    uint32 is_spell:1;
    uint32 is_wand:1;
    uint32 is_axe:1;
    uint32 is_warhammer:1;
    uint32 is_bow:1;
    uint32 is_gun:1;
    uint32 is_armour:1;
    uint32 is_amulet:1;
    uint32 is_cursed:1;
    uint32 curse_noticed:1;
    uint32 is_stolen:1;
    uint32 is_unpaid_for:1;
    uint32 is_heavy:1;
    uint32 is_vheavy:1;
    uint32 is_damaged:1;
};

class gh_player : public gh_stream
{   
public:
    gh_screen       myscreen;
    string          level_set_name;
    string          which_level;
    uint32          level_no;
    class gh_thing  *thing;
    string          thing_name;
    class gh_game   *game;

    gh_player(class gh_game *game = NULL);
    ~gh_player(void);
    void loop(void);
    void recording_toggle(void);
    void remove_level(void);
    void new_level(string level_set, 
                   uint32 level_no,
                   string which_level,
                   bool want_empty_level = false);

    void init(void);
    void fini(void);
    void tick(void);
    void level_start(void);
    void level_begin(void);
    void level_end(void);
    void display(void);

    int key_down(const SDL_keysym & key);
    int key_held(const SDL_keysym & key);
    int key_up(const SDL_keysym & key);
    int editing_key_down(const SDL_keysym & key);
    int editing_key_held(const SDL_keysym & key);
    int mouse_up(const size_t button);
    int mouse_down(const size_t button);
    int mouse_held(const size_t button);
    int mouse_movement(const gh_point & p, const gh_point & at);
    void reset(void);
    void display_map(void);
    void level_editor_display_map(void);
    void display_editor(void);
    void display_game(void);

    //
    // help.cpp
    //
    void display_help(void);
    void display_editor_help(void);

    //
    // quit.cpp
    //
    void display_quit(void);

    //
    // info.cpp
    //
    void display_info(void);

    //
    // pause.cpp
    //
    void display_pause(void);

    //
    // score.cpp
    //
    void display_score(void);

    //
    // editor.cpp
    //
    void enter_level_editor(void);
    void exit_level_editor(void);
    gh_level *new_level_editor(void);

    //
    // item.cpp
    //
    void make_item_list(void);
    void display_item_list(void);
    bool drop_item(void);
    void use_item(void);
    bool shortcut_key_use_item(const SDL_keysym & sym);

    //
    // save.cpp
    //
    void save(void);

    //
    // A list of unique items and the row they exist in.
    //
    list<gh_item> items;

    //
    // Which row item the user last selected.
    //
    int item_last_selected;

    //
    // If we grabbed a thing in level edit mode, this is it.
    //
    gh_thing *editing_thing;

    //
    // The name of the level we just left. This is used for setting up
    // return exits on the new level.
    //
    string last_level_set_name;

    //
    // Named things we have met that were unique
    //
    vector<string> met;

    //
    // Cowardly quit?
    // 
    uint32 quit:1;

    friend void operator>> (gh_stream_in& is, gh_player *me);
    friend void operator<< (gh_stream_out& os, const gh_player *me);
};

#endif /* _GH_PLAYER_H_ */
