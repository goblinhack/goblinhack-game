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

    $Id: gh_game.h 499 2007-04-08 13:28:25Z goblinhack $
*/

#ifndef _GH_GAME_H_
#define _GH_GAME_H_

#include <gh_messages.h>

class gh_game : public gh_stream
{
public:
    gh_game (string config = "");
    ~gh_game (void);

    void save(void);
    void auto_save(void);
    void auto_save_remove(void);

    //
    // Bones files
    //
    void bones_save(void);
    void bones_remove(void);

    //
    // Runtime files
    //
    void runtime_save(void);
    void runtime_remove(void);

    void fixup(void);

    void update_settings(void);
    void display_messages(void);

    //
    // Current game frame
    //
    uint32                  frame;

    //
    // Map resolution
    //
    uint32                  res;

    //
    // Player related stuff
    //
    class gh_player         *player;
    string                  level_set_name;
    int                     level_no;
    string                  which_level;
    string                  new_player_name;
    string                  new_player_class;
    class gh_hiscores       *hiscores;
    class gh_level_maps     *level_maps;
    uint8                   paused:1;
    uint8                   help_screen:1;
    uint8                   quit_screen:1;
    uint8                   info_screen:1;
    uint8                   want_to_enter_level_editor_mode:1;
    uint8                   want_to_exit_level_editor_mode:1;
    uint8                   help_message:1;
    uint8                   full_moon_message:1;
    uint8                   midnight_message:1;
    uint8                   friday_13th_message:1;
    uint8                   halloween_message:1;
    uint8                   dark_message:1;

    //
    // The current list of messages being displayed
    //
    list<gh_game_message>   messages;
    
    void loop(void);

    void new_message(const string msg, const bool uniq = false);
    void replace_last_message(const string msg);
    string get_last_message(void);
    void delete_all_messages(void);

    friend void operator>> (gh_stream_in& is, gh_game *me);
    friend void operator<< (gh_stream_out& os, const gh_game *me);
};

#endif /* _GH_GAME_H_ */
