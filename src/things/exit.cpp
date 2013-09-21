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

    $Id: meet.cpp 746 2008-03-15 18:33:36Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_player.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_room.h>
#include <gh_game.h>
#include <gh_math.h>

//
// A thing has met the return exit
//
void gh_thing::hero_meet_return_exit (gh_thing *target)
{
    GH_FUNCTION()

    //
    // To prevent bouncing between return and exits.
    //
    if (target->sleeping()) {
        if (level->level_no == 1) {
            level->game->new_message("The entrance is not open yet");
        } else {
            level->game->new_message("The exit is not open yet");
        }
        return;
    }

    gh_player *player = level->game->player;

    //
    // Return to the last user level like random/1/1
    //
    if (player->last_level_set_name == "") {
        level->game->new_message("The exit seems to be broken");
        return;
    }

    //
    // Stop dead to avoid looping through return exits.
    //
    speed = 0;

    //
    // Trying to go back into a bonus level? Bad show.
    // Straight to hell!
    //
    if (player->last_level_set_name == "hell") {
        player->level_set_name = "random";
        player->level_no = 65;
        player->which_level = "dummy";
    } else {
        vector<string> level_name =
            string_split(player->last_level_set_name, '/');

        if (level_name.size() > 0) {
            player->level_set_name = level_name[0];
        } else {
            player->level_set_name = "";
        }
    
        if (level_name.size() > 1) {
            player->level_no = toint(level_name[1]) - 1;
        } else {
            player->level_no = 0;
        }
    
        if (level_name.size() > 2) {
            player->which_level = level_name[2];
        } else {
            player->which_level = "dummy";
        }
    }

    gh_global::done = true;

    //
    // Move the player off of the level so they can't be saved as runtime
    // date.
    //
    if (is_on_map) {
        level->map.pop(this);
    }

    target->sleeping_boost_start(10);
}

//
// A thing has met the exit
//
void gh_thing::hero_meet_exit (gh_thing *target)
{
    GH_FUNCTION()

    //
    // To prevent bouncing between return and exits.
    //
    if (target->sleeping()) {
        level->game->new_message("The exit is not working yet");
        return;
    }

    gh_player *player = level->game->player;

    level->is_level_completed = true;

    //
    // Stop dead to avoid looping through return exits.
    //
    speed = 0;

    if (!target->is_open) {
        play("Lead_011");

        //
        // Mark the exit as open now so when we reenter it we know
        // we get no bonus.
        //
        open_this(target);

        //
        // Only do this the first time we exit a level.
        //
        if (can_self_teleport_once_per_level) {
            can_self_teleport = true;
        }
    }

    if (target->exit_name != "") {
        if (target->text != "") {
            level->game->new_message("Level warp to " + target->text);
        } else {
            level->game->new_message("Level warp");
        }

        //
        // Parse things like forest/3/2, to forest, level 3, sub 2
        //
        vector<string> level_name = string_split(target->exit_name,
                                                 '/');

        if (level_name.size() > 0) {
            player->level_set_name = level_name[0];
        } else {
            player->level_set_name = "";
        }

        if (level_name.size() > 1) {
            player->level_no = toint(level_name[1]) - 1;
        } else {
            player->level_no = 0;
        }

        if (level_name.size() > 2) {
            player->which_level = level_name[2];
        } else {
            player->which_level = "dummy";
        }

    } else if (target->exit_jump) {
        player->level_no += target->exit_jump - 1;
    }

    target->sleeping_boost_start(10);
}
