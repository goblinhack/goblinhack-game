/*
    ghack.
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

    $Id: game.cpp 307 2006-12-03 15:20:21Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_player.h>
#include <gh_ui_event.h>
#include <gh_subsys.h>
#include <gh_hiscore.h>
#include <gh_global.h>
#include <gh_sdl.h>
#include <gh_audio.h>
#include <gh_music.h>

void gh_game::loop (void)
{
    GH_FUNCTION()

    bool need_remove_level;
    gh_event_ret e;
	
    //
    // Perform crash recovery.
    //
    if (gh_ui_new_player_auto_save_loop(this) == GH_UI_EVENT_CONTINUE) {
	//
	// Got an auto save file, try again.
	//
	GH_LOG("*** Found an auto save file, try again.");
	return;
    }

    int last_bravery = -1;
    bool want_level_start_intro = true;

redo:
    for (;;) {
	update_settings();

	//
	// If not playing yet
	//
	if (player == NULL) {
	    player = new gh_player(this);

	    last_bravery = -1;
	    want_level_start_intro = true;

	    delete_all_messages();

	    //
	    // Remove old runtime saved levels before a new game.
	    //
	    runtime_remove();

	    switch (gh_ui_intro_loop(this)) {
	    case GH_UI_EVENT_CONTINUE:
		switch (gh_ui_new_player_class_chooser_loop(this)) {
		    case GH_UI_EVENT_RESTART:
		    case GH_UI_EVENT_ESCAPE:
		    case GH_UI_EVENT_QUIT:
			delete player;
			player = NULL;
			goto redo;

		    default:

			switch (gh_ui_new_level_type_chooser_loop(this)) {
			    case GH_UI_EVENT_RESTART:
			    case GH_UI_EVENT_ESCAPE:
			    case GH_UI_EVENT_QUIT:
				delete player;
				player = NULL;
				goto redo;

			    default:
				continue;
			}

			continue;
		}

	    case GH_UI_EVENT_RESTART:
	    case GH_UI_EVENT_ESCAPE:
	    case GH_UI_EVENT_QUIT:
		delete player;
		player = NULL;
		GH_LOG("Intro loop quit");
		return;

	    case 'l':
		switch (gh_ui_new_player_chooser_loop(this)) {
		    case GH_UI_EVENT_QUIT:
			delete player;
			player = NULL;
			GH_LOG("Player chooser loop quit.");
			return;

		    case GH_UI_EVENT_ESCAPE:
			delete player;
			player = NULL;
			goto redo;

		    default:
			delete player;
			player = NULL;
			GH_LOG("New player chooser quit");
			return;
		}
		break;

	    case 'h':
		(void) (gh_ui_player_new_help_text_loop(this));
		delete player;
		player = NULL;
		goto redo;

	    case 'e':
choose_new_level:
		switch (gh_ui_new_level_chooser_loop(this)) {
		    case GH_UI_EVENT_QUIT:
			delete player;
			player = NULL;
			GH_LOG("New char chooser quit.");
			return;

		    case GH_UI_EVENT_ESCAPE:
			delete player;
			player = NULL;
			goto redo;

		    default:
			break;
		}

		if (!level_no) {
		    level_no = 1;
		    which_level = "dummy";
		}

		//
		// This is level editor mode, just force select a character.
		//
		new_player_class = "things/hero/wizard";
		want_to_enter_level_editor_mode = true;
		continue;

	    case 'c':
		(void) gh_ui_new_config_loop(this);

		delete player;
		player = NULL;

		if (gh_global::config_changed) {
		    GH_LOG("Game rerun.");
		    return;
		}

		goto redo;

	    default:
		break;
	    }

	    update_settings();
	}

	if ((player->thing == NULL) || (player->thing->level == NULL)) {
	    //
	    // Start on the first or move to the next level
	    //
	    try {
		player->new_level(level_set_name,
				  level_no,
				  which_level);
	    } catch (...) {
		GH_LOG("Failed to create new level: " +
		       level_set_name + "/" +
		       tostring(level_no, 3, '0') + "/" +
		       which_level);

		delete player;
		player = NULL;
		goto redo;
	    }

	    if (!player->thing || !player->thing->level) {
		GH_LOG("Failed to load level: " +
		       level_set_name + "/" +
		       tostring(level_no, 3, '0') + "/" +
		       which_level);

		delete player;
		player = NULL;
		goto redo;
	    }

	    player->thing->level->freeze_things = true;

	    player->level_start();

	    if (!want_to_enter_level_editor_mode) {
		if (player->thing->bravery_curr >= 500) {
		    if (player->thing->bravery_curr != last_bravery) {
			(void) gh_ui_new_player_powerup_chooser_loop(this);
		    }
		}
	    }

	    last_bravery = player->thing->bravery_curr;

	    player->level_begin();

	    //
	    // Level editor mode? Just slip the intro stuff.
	    //
	    if (want_to_enter_level_editor_mode) {
		want_level_start_intro = false;
	    }

	    //
	    // The level loaded ok, play it!
	    //
	    if (want_level_start_intro) {
		want_level_start_intro = false;

		switch (gh_ui_level_start_loop(this)) {
		case GH_UI_EVENT_QUIT:
		    delete player;
		    player = NULL;
		    goto redo;

		default:
		    break;
		}
	    }
	} else {
	    //
	    // Loading a player
	    //
	    player->thing->level->freeze_things = true;

	    player->level_start();

	    player->level_begin();
	}

	player->thing->level->freeze_things = false;

	sdl_loop();

	need_remove_level = false;
	gh_global::done = false;

	if (player->thing) {
	    player->thing->level->level_completed();
	}

	if (want_to_exit_level_editor_mode) {
	    want_to_exit_level_editor_mode = false;
	    player->level_end();
	    player->remove_level();
	    player->myscreen.hide();
	    goto choose_new_level;
	}

	//
	// Did we finish the level alive?
	//
	if (player->thing->health > 0) {
	    player->level_end();

	    e = gh_ui_level_end_loop(this);

	    //
	    // If they quit now, don't record the hiscore level as + 1
	    //
	    if (e == GH_UI_EVENT_CONTINUE) {
		//
		// Move to the next level.
		//
		level_set_name = player->level_set_name;
		level_no = player->level_no;
		which_level = "dummy";

		player->level_no = ++level_no;
	    }

	    //
	    // End of game?
	    //
	    if (player->thing->level->is_last_level) {
		if (hiscores->is_new_hiscore(player)) {
		    e = gh_ui_player_new_hiscore_loop(this);
		}
	    }

	    need_remove_level = true;

	    //
	    // No return to bonus levels.
    	    //
	    if (!player->thing->level->is_bonus) {
	        runtime_save();
	    }
	} else {
	    //
	    // We died. Start from level 1 again after the hiscore has been
	    // recorded.
	    //
	    if (hiscores->is_new_hiscore(player)) {
		e = gh_ui_player_new_hiscore_loop(this);
	    } else {
		e = gh_ui_player_croaked_it_loop(this);
	    }

	    //
	    // Save this level as a potential bones file.
	    //
	    if (!player->quit) {
		bones_save();
	    }

	    level_set_name = "random";
	    level_no = 1;
	    which_level = "dummy";

	    player->level_set_name = level_set_name;
	    player->level_no = level_no;
	    player->which_level = which_level;
	    player->level_end();

	    player->myscreen.hide();

	    //
	    // Remove old runtime saved levels.
	    //
	    runtime_remove();

	    delete player;
	    player = NULL;
	}

	switch (e) {
	case GH_UI_EVENT_RESTART:
	    if (player) {
		(void) gh_ui_player_croaked_it_loop(this);
		player->level_end();
		player->myscreen.hide();
		delete player;
		player = NULL;
	    }

	    level_set_name = "random";
	    level_no = 1;
	    which_level = "dummy";
	    break;

	case GH_UI_EVENT_QUIT:
	    if (player) {
		(void) gh_ui_player_croaked_it_loop(this);
		player->level_end();
		player->myscreen.hide();
		delete player;
		player = NULL;
	    }

	    level_set_name = "random";
	    level_no = 1;
	    which_level = "dummy";

	    GH_LOG("Game quit.");
	    return;

	default:
	    break;
	}

	if (player) {
	    if (need_remove_level) {
		player->remove_level();
	    }
	}
    }
}
