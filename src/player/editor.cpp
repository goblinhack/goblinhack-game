/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (look your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: loop.cpp 1240 2009-07-10 02:20:51Z Ghack $
*/

#include <ghack.h>
#include <hacklib.h>
#include <gh_player.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_term.h>
#include <gh_templates.h>
#include <gh_audio.h>
#include <gh_math.h>

void gh_player::enter_level_editor (void)
{
    GH_FUNCTION()

    gh_level *level = thing->level;

    GH_LOG("Level editor start");

    if (game->want_to_enter_level_editor_mode) {
        game->want_to_enter_level_editor_mode = false;

        gh_point3d middle((level->map.size.w * GH_RES) / 2,
                          (level->map.size.h * GH_RES) / 2,
                          0);
        thing->move_to(middle);
    }

    thing->destroy_all_items();

    level->is_editing = true;

    level->reset_all();

    level->cursor_at.x = level->map.hero_at.x;
    level->cursor_at.y = level->map.hero_at.y;

    SDL_WM_GrabInput(SDL_GRAB_ON);

    level->game->delete_all_messages();

    level->game->new_message("Level editor mode. Press H for help.", true /* unique */);

    myscreen.auto_repeat_rate = 5;
    myscreen.auto_repeat_delay = 50;
}

void gh_player::exit_level_editor (void)
{
    GH_FUNCTION()

    GH_LOG("Level editor end");

    gh_level *level = thing->level;

    game->delete_all_messages();

    level->is_editing = false;

    level->cursor_at.x = level->map.hero_at.x;
    level->cursor_at.y = level->map.hero_at.y;

    SDL_WM_GrabInput(SDL_GRAB_OFF);

    myscreen.auto_repeat_rate = 1;
    myscreen.auto_repeat_delay = 5;

    editing_thing = NULL;
}

gh_level *gh_player::new_level_editor (void)
{
    gh_level *level;

    GH_LOG("Level editor create a new level");

    string level_name = "levels/" +
                        game->level_set_name + "/" +
                        tostring(game->level_no, 3, '0') + "/" +
                        game->which_level;
                
    level = new gh_level(game,
                         game->level_no,
                         game->which_level,
                         game->level_set_name,
                         level_name,
                         true /* want_empty_level */);

    level->is_editing = true;

    //
    // Reset the level draw bounds.
    //
    level->draw_start   = gh_point3d(-1,-1,-1);
    level->draw_end     = gh_point3d(-1,-1,-1);

    gh_point3d middle((level->map.size.w * GH_RES) / 8,
                      (level->map.size.h * GH_RES) / 8,
                      0);

    thing = new gh_thing(level,
                         "things/hero/wizard",
                         middle,
                         NULL, /* template */
                         this);

    level->map.push(thing);
    level->map.add_border();
    level->map.hero_at = middle;
    level->map.hero_map_at = middle / level->map. res;
    level->cursor_at.x = level->map.hero_at.x;
    level->cursor_at.y = level->map.hero_at.y;

    //
    // The player holds onto a ref so the this can't totally
    // destroy it without telling us.
    //
    thing->incref(GH_THING_REF_PLAYER);

    game->delete_all_messages();
    game->replace_last_message("New level");

    SDL_WM_GrabInput(SDL_GRAB_ON);

    myscreen.auto_repeat_rate = 10;
    myscreen.auto_repeat_delay = 5;

    return (level);
}
