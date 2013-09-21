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

    $Id: loop.cpp 1241 2009-07-10 21:32:09Z Ghack $
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

int end_in_ticks;
int ignore_game_start_events_ticks;

void gh_player::level_start (void)
{
    GH_FUNCTION()

//    gh_surface::smooth_scroll(true);

    GH_LOG("");
    GH_LOG("*** Player starting level ***");
    GH_LOG("");

    gh_thing *player_thing = thing;
    if (!player_thing) {
        GH_THROW("no player thing?");
    }

    //
    // Sanity checking for loaded games
    //
    gh_level *level = player_thing->level;
    if (!level) {
        GH_THROW("no level?");
    }

    //
    // Loading a test level?
    //
    if (player_thing->name == "") {
        player_thing->copy(gh_thing::find("hero/wizard", true /* error */));
        player_thing->level = level;
        player_thing->at = level->hero_start_where;
        player_thing->born = level->hero_start_where;
        player_thing->player = this;
        player_thing->player->thing = player_thing;

//      level->draw_start               = gh_point3d(-1,-1,-1);
//      level->draw_end                 = gh_point3d(-1,-1,-1);
    }

    if (!player_thing->is_on_map) {
        level->map.push(player_thing);
    }

    player_thing->add_all_things();

    gh_game *game = level->game;
    if (!game) {
        GH_THROW("no game assoc with level?");
    }

    //
    // Ensure we don't start off auto moving somewhere
    //
    player_thing->direction = gh_point3d(0,0,0);
    player_thing->direction_wanted = gh_point3d(0,0,0);
    player_thing->update_anim();

    end_in_ticks = 0;

    level->reset_all();

    myscreen.show();

    //
    // Give some protection at the start
    //
    if (level->first_enter) {
        player_thing->shielded_boost_start(6); // 3 secs

        //
        // Controls when we allow the monsters to move
        //
        level->freeze_things = true;
    }
}

void gh_player::level_begin (void)
{
    GH_FUNCTION()

    GH_LOG("");
    GH_LOG("*** Player beginning level ***");
    GH_LOG("");

    gh_thing *player_thing = thing;
    if (!player_thing) {
        GH_THROW("no player thing?");
    }

    //
    // Sanity checking for loaded games
    //
    gh_level *level = player_thing->level;
    if (!level) {
        GH_THROW("no level?");
    }

    if (level->first_enter) {
        if (!game->help_message) {
            game->new_message("Press %%fg=redh%%fg=green for help");
            game->help_message = true;
        }

        if (level->has_shop) {
            game->new_message(
                         "You hear the distant chime of a cash register");

            gh_audio::play("cash_register2");
        }

        if (!game->full_moon_message) {
            if (phase_of_the_moon() == GH_FULL_MOON) {
                game->new_message(
                              "%%fg=white"
                              "Watch out. Full moon tonight..."
                              "%%fg=green"
                              );
                gh_audio::play("evil-wicked-spooky-laugh", 2);

                game->full_moon_message = true;
            }
        }

        if (!game->midnight_message) {
            if (midnight()) {
                game->new_message(
                              "%%fg=red"
                              "Midnight. You hear howls of evil joy..."
                              "%%fg=green"
                              );
                game->midnight_message = true;

                gh_audio::play("evil-wicked-spooky-laugh", 2);
            }
        }

        if (!game->friday_13th_message) {
            if (friday_13th()) {
                game->new_message(
                              "%%fg=random"
                              "Friday the 13th... Go back mortal!"
                              "%%fg=green"
                              );
                game->friday_13th_message = true;

                gh_audio::play("evil-wicked-spooky-laugh", 2);
            }
        }

        if (!game->halloween_message) {
            if (halloween()) {
                game->new_message(
                              "%%fg=random"
                              "Halloween... Thou art doomed mortal!"
                              "%%fg=green"
                              );
                game->halloween_message = true;

                gh_audio::play("evil-wicked-spooky-laugh", 2);
            }
        }

        if (!game->dark_message) {
            if (midnight_close()) {
                game->new_message(
                              "%%fg=red"
                              "The midnight hour is close at hand..."
                              "%%fg=green"
                              );
                game->dark_message = true;

                gh_audio::play("evil-wicked-spooky-laugh", 2);
            }
        }

        if (night() || halloween() || midnight() || friday_13th() ||
            (phase_of_the_moon() == GH_FULL_MOON)) {
            //
            // You were warned...
            //
            game->new_message(player_thing->named() +
                              " foolishly enters level " +
                              tostring(player_thing->level->level_no));

        } else {
            game->new_message(player_thing->named() +
                              " bravely enters level " +
                              tostring(player_thing->level->level_no));
        }
    }

    //
    // This is a bit obscure. We're relying on this routine to walk all
    // the items we're carrying and call add_all_things. If we don't do
    // this then when we auto save, our weapons will be missing.
    //
    make_item_list();

    //
    // Crash protection.
    //
    level->game->auto_save();

    //
    // Save where we began so bones files can start here.
    //
    if (level->hero_start_where == gh_point3d(-1,-1,-1)) {
        level->hero_start_where = thing->at;
    }
    level->hero_end_where = gh_point3d(-1,-1,-1);

    level->first_enter = false;

    thing->deposit_followers();
}

void gh_player::display (void)
{
    GH_FUNCTION()

    gh_thing *player_thing = thing;
    gh_level *level = player_thing->level;

    if (!level) {
        GH_THROW("no level?");
    }

    if (game->want_to_enter_level_editor_mode) {
        enter_level_editor();
    }

    if (game->paused) {
        display_pause();
        return;
    }

    if (game->quit_screen) {
        display_quit();
        return;
    }

    if (game->help_screen) {
        if (level->is_editing) {
            display_editor_help();
        } else {
            display_help();
        }
        return;
    }

    if (game->info_screen) {
        display_info();
        return;
    }

    //
    // Is the level ending soon?
    //
    if (end_in_ticks) {
        //
        // Allow the monsters to run around a bit before we end.
        //
        level->freeze_things = false;

        if (gh_global::debug) {
            GH_LOG("");
            GH_LOG("*** Ending in ticks " + tostring(end_in_ticks) + " ***");
            GH_LOG("");
        }

        //
        // Move the player off of the level so they can't be killed.
        //
        if (player_thing->is_on_map) {
            player_thing->collect_followers();

            level->map.pop(player_thing);
        }

        //
        // Allow enough time for things like explosions to vanish.
        //
        end_in_ticks--;

        if (!end_in_ticks) {
            gh_global::done = true;

            //
            // Ensure we don't hit this condition twice and stop
            // the hi score screen from running.
            //
            end_in_ticks--;
        }
    }

    if (player_thing->is_dead) {
        //
        // Allow the monsters to move
        //
        level->freeze_things = false;

        //
        // Allow the player's explosion to finish before we end
        // the level.
        //
        if (end_in_ticks == 0) {
            end_in_ticks = 50;

            GH_LOG("");
            GH_LOG("*** Ending level soon ***");
            GH_LOG("");
        }
    }

    if (!level->is_level_completed) {
        //
        // Don't allow the player to quit whilst the game is ending!
        //
        if (!player_thing->is_dead) {
            //
            // And don't allow them to move when dead!
            //
            level->tick_hero();
        }
    }

    level->tick_things();

    level->tick();

    display_game();

    level->handle_dead_all();

    if (level->is_level_completed) {
        //
        // If the end of the level, let the explosions complete.
        //
        if (end_in_ticks == 0) {
            end_in_ticks = 10;

            //
            // If the end of the game, allow for a long ending.
            //
            if (level->is_last_level) {
                end_in_ticks = 100;
            }
        }
    }

    if (ignore_game_start_events_ticks > 0) {
        ignore_game_start_events_ticks--;
    }
}

void gh_player::level_end (void)
{
    GH_FUNCTION()

    gh_thing *player_thing = thing;
    gh_level *level = player_thing->level;

    //
    // Record this level name so we can set level return exits to use it
    // on the new level.
    //
    last_level_set_name = level->level_set_name + "/" +
                          tostring(level->level_no, 3, '0') + "/" +
                          level->which_level;

    if (level->is_bonus) {
        // 
        // Ho ho ho. Player tries to go through a return exit into
        // the bonus level again, will get a surprise.
        //
        last_level_set_name = "hell";
    }

    level->reset_all();

    //
    // No longer in a room.
    //
    thing->room = NULL;

    level->game->auto_save_remove();
}
