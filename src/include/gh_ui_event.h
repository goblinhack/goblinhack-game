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

    $Id: gh_ui_event.h 307 2006-12-03 15:20:21Z goblinhack $
*/

#ifndef _GH_UI_EVENT_H_
#define _GH_UI_EVENT_H_

typedef enum {
    GH_UI_EVENT_NONE,
    GH_UI_EVENT_ESCAPE, // leaving level editor
    GH_UI_EVENT_QUIT,   // quitting a game
    GH_UI_EVENT_RESTART,
    GH_UI_EVENT_CONTINUE,
} gh_event_ret;

extern gh_event_ret gh_ui_event(void);

extern gh_event_ret gh_ui_intro_loop(class gh_game *game);
extern void gh_ui_intro_border(class gh_game *game);
extern gh_event_ret gh_ui_level_start_loop(class gh_game *game);
extern gh_event_ret gh_ui_level_end_loop(class gh_game *game);
extern gh_event_ret gh_ui_player_dead_loop(class gh_game *game);
extern gh_event_ret gh_ui_player_croaked_it_loop(class gh_game *game);
extern gh_event_ret gh_ui_player_new_hiscore_loop(class gh_game *game);
extern string gh_ui_player_new_read_text_loop(class gh_game *game,
                                              string what);

typedef void (*gh_ui_callback)(void *context, string result);
extern string gh_ui_player_new_read_small_text_loop(class gh_game *game,
                                                    void *context,
                                                    gh_ui_callback callback);

extern gh_event_ret gh_ui_new_level_chooser_loop(class gh_game *game);
extern gh_event_ret gh_ui_new_config_loop(class gh_game *game);
extern gh_event_ret gh_ui_new_player_chooser_loop(class gh_game *game);
extern gh_event_ret gh_ui_new_player_class_chooser_loop(class gh_game *game);
extern gh_event_ret gh_ui_new_level_type_chooser_loop(class gh_game *game);
extern gh_event_ret gh_ui_new_player_powerup_chooser_loop(class gh_game *game);
extern gh_event_ret gh_ui_new_player_auto_save_loop(class gh_game *game);
extern gh_event_ret gh_ui_player_new_help_text_loop(class gh_game *game);

#endif /* _GH_UI_EVENT_H_ */
