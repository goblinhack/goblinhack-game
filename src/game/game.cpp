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

    $Id: game.cpp 1265 2009-08-02 15:59:12Z Ghack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_player.h>
#include <gh_ui_event.h>
#include <gh_subsys.h>
#include <gh_hiscore.h>
#include <gh_level_map.h>
#include <gh_global.h>
#include <gh_sdl.h>

gh_game::gh_game (string config)
{
    GH_FUNCTION()

    //
    // This is a hack so the main loop can dump messages without the game
    // class present. Don't use this if you can.
    //
    gh_global::game = this;

    frame                   = 0;
    player                  = NULL;
    level_set_name          = "random";
    level_no                = 1;
    which_level             = "dummy";
    new_player_name         = "";
    new_player_class        = "";
    hiscores                = NULL;
    level_maps              = NULL;
    res                     = GH_RES;
    paused                  = false;
    help_screen             = false;
    quit_screen             = false;
    info_screen             = false;
    want_to_enter_level_editor_mode = false;
    want_to_exit_level_editor_mode = false;
    help_message            = false;
    full_moon_message       = false;
    midnight_message        = false;
    friday_13th_message     = false;
    halloween_message       = false;
    dark_message            = false;

    try {
        if (config != "") {
            gh_read_file(this, config,
                   true, /* noisy */
                   true /* error */);
        }
    } catch (...) {
        gh_global::fullscreen   = true;
        gh_global::noframe      = true;
    }

    update_settings();

    level_maps = new gh_level_maps();
}

gh_game::~gh_game (void)
{
    gh_global::game = NULL;

    GH_FUNCTION()

    if (player) {
        delete player;
        player = NULL;
    }

    if (hiscores) {
        delete hiscores;
        hiscores = NULL;
    }

    if (level_maps) {
        delete level_maps;
        level_maps = NULL;
    }
}

void gh_game::update_settings (void)
{
    GH_FUNCTION()

    if (hiscores) {
        delete hiscores;
        hiscores = NULL;
    }

    hiscores = new gh_hiscores(level_set_name);
}

void operator>> (gh_stream_in& is, gh_game *me)
{
    GH_FUNCTION()

    //
    // No screensize change on loaded games.
    //
    if (me->get_opt_classp(is, &me->player, "player")) {
        return;
    }

    bool music;
    me->get_opt_named_flag(is, &music, "music", false);
    if (music) {
        gh_global::music = false;
    }

    bool sound;
    me->get_opt_named_flag(is, &sound, "sound", false);
    if (sound) {
        gh_global::sound = false;
    }

    me->get_opt_named_flag(is, &gh_global::debug, "debug", false);
    me->get_opt_named_flag(is, &gh_global::trace_mode, "trace_mode", false);
    me->get_opt_named_flag(is, &gh_global::fullscreen, "fullscreen", false);
    me->get_opt_named_flag(is, &gh_global::noframe, "noframe", false);
    me->get_opt_class(is, &gh_global::screensize, "screensize");
}

void operator<< (gh_stream_out& os, const gh_game *me)
{
    GH_FUNCTION()

    if (me->player) {
        me->put_class(os, me->player, "player");
    }

    if (!gh_global::music) {
        bool music = !gh_global::music;
        me->put_named_flag(os, &music, "music");
    }

    if (!gh_global::sound) {
        bool sound = !gh_global::sound;
        me->put_named_flag(os, &sound, "sound");
    }

    me->put_named_flag(os, &gh_global::debug, "debug");
    me->put_named_flag(os, &gh_global::trace_mode, "trace_mode");
    me->put_named_flag(os, &gh_global::fullscreen, "fullscreen");
    me->put_named_flag(os, &gh_global::noframe, "noframe");
    me->put_class(os, &gh_global::screensize, "screensize");
}
