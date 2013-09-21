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

    $Id: keys.cpp 1245 2009-07-12 16:46:06Z Ghack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_term.h>
#include <gh_templates.h>
#include <gh_audio.h>
#include <gh_io.h>
#include <gh_sdl_event.h>
#include <gh_music.h>

extern int end_in_ticks;
extern int ignore_game_start_events_ticks;

int gh_player::key_held (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_level *l = thing->level;
    if (l && l->is_editing) {
        return (editing_key_held(sym));
    }

    if (l && l->freeze_things) {
        return (false);
    }

    if (ignore_game_start_events_ticks > 0) {
        //
        // Keys used to start the game should not fall through to here;
        // like fire.
        //
        return (false);
    }

    if (thing->is_dead) {
        return (false);
    }

    switch (sym.sym) {
    case SDLK_KP7:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.x = -thing->speed;
        thing->direction_wanted.y = -thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP9:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.x = thing->speed;
        thing->direction_wanted.y = -thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP1:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.x = -thing->speed;
        thing->direction_wanted.y = thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP3:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.x = thing->speed;
        thing->direction_wanted.y = thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP4:
    case SDLK_LEFT:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.x = -thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP6:
    case SDLK_RIGHT:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.x = thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP8:
    case SDLK_UP:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.y = -thing->speed;
        thing->update_anim();
        break;

    case SDLK_KP2:
    case SDLK_DOWN:
        if (!thing->speed) {
            thing->speed = 1;
            thing->direction = gh_point3d(0,0,0);
        }
        thing->direction_wanted.y = thing->speed;
        thing->update_anim();
        break;

    case ' ':
        //
        // Fire!
        //
        thing->use_weapon(NULL, /* target */
                          NULL, /* weapon */
                          false /* switch_to_but_dont_activate */);

        if (!thing->weapon) {
            //
            // Use fists!
            //
            thing->wants_to_hit = true;
        }
        break;

    default:
        break;
    }

    return (true);
}

int gh_player::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_level *l = thing->level;
    if (l && l->is_editing) {
        return (editing_key_down(sym));
    }

    if (l && l->freeze_things) {
        return (false);
    }

    if (ignore_game_start_events_ticks > 0) {
        //
        // Keys used to start the game should not fall through to here;
        // like fire.
        //
        return (false);
    }

    if (game->paused) {
        game->paused = false;
        gh_audio::play("Light_Switch_004");
        return (false);
    }

    if (game->help_screen) {
        game->help_screen = false;
        gh_audio::play("Light_Switch_004");
        return (false);
    }

    if (game->info_screen) {
        game->info_screen = false;
        gh_audio::play("Light_Switch_004");
        return (false);
    }

    if (game->quit_screen) {
        game->quit_screen = false;

        if (sym.sym == 'y') {
            goto hard_quit;
        }

        return (false);
    }

    if (thing->is_dead) {
        return (false);
    }

    switch (sym.sym) {
    case SDLK_KP7:
    case SDLK_KP9:
    case SDLK_KP1:
    case SDLK_KP3:
    case SDLK_KP4:
    case SDLK_LEFT:
    case SDLK_KP6:
    case SDLK_RIGHT:
    case SDLK_KP8:
    case SDLK_UP:
    case SDLK_KP2:
    case SDLK_DOWN:
        return (key_held(sym));

    case 'a':
        make_item_list();

        //
        // move up in the selected item list
        //
        gh_audio::play("Button_Off_001");

        item_last_selected++;
        item_last_selected =
                        min(item_last_selected, (int)items.size() - 1);
        return (true);

    case 'z':
        make_item_list();

        //
        // move down in the selected item list
        //
        gh_audio::play("Button_Off_001");
        
        item_last_selected--;
        item_last_selected = max(0, item_last_selected);

        return (true);

    case SDLK_PAGEDOWN:

        make_item_list();

        gh_audio::play("Button_Off_001");

        item_last_selected -= 6;
        item_last_selected = max(0, item_last_selected);
        break;

    case SDLK_PAGEUP:

        make_item_list();

        gh_audio::play("Button_Off_001");

        item_last_selected += 6;
        item_last_selected = min(item_last_selected, (int)items.size() - 1);
        return (true);


    case 'o':
        thing->open_nearest_thing();
        break;

    case ' ':
        //
        // Fire!
        //
        thing->use_weapon(NULL, /* target */
                          NULL, /* weapon */
                          false /* switch_to_but_dont_activate */);
        break;

    case 'p':
        //
        // multi purpose key 8) pay or pause as appropriate.
        //
        if (thing->pay_in_shop()) {
            make_item_list();
            return (true);
        }

        game->paused = !game->paused;
        gh_audio::play("Button_Off_001");
        break;

    case 's':
        if (l->hero_start_where == gh_point3d(-1,-1,-1)) {
            //
            // Save the hero where the cursor is. It's where they
            // will be born.
            //
            l->hero_start_where = thing->at;
            l->hero_end_where = gh_point3d(-1,-1,-1);
        }

        save();
        gh_audio::play("Button_Off_001");
        break;

    case 'h':
        game->help_screen = !game->help_screen;
        gh_audio::play("Button_Off_001");
        break;

    case 'd':
        if (drop_item()) {
            gh_audio::play("Object_Drop_001");
        }

        make_item_list();
        break;

    case 'm':
        if (gh_global::music && gh_global::sound) {
            gh_global::music = false;
            gh_global::sound = true;
        } else if (!gh_global::music && gh_global::sound) {
            gh_global::music = false;
            gh_global::sound = false;
        } else {
            gh_global::music = true;
            gh_global::sound = true;
        }

        if (gh_global::music) {
            game->new_message("Music on", true /* unique */);
            gh_music::play("HackYourWay");
        } else {
            game->new_message("Music off", true /* unique */);
            gh_music::halt();
        }

        if (gh_global::sound) {
            game->new_message("Sound on", true /* unique */);
        } else {
            game->new_message("Sound off", true /* unique */);
        }
        break;

    case 'u':
    case SDLK_LSHIFT:
    case SDLK_RETURN:
    case SDLK_KP_ENTER:
        use_item();

        make_item_list();
        break;

    case 't':
        if (thing->can_self_teleport) {
            thing->self_teleport();
        } else {
            game->new_message("Can't teleport.");
        }
        break;

    case 'r':
        if (thing->can_rage()) {
            thing->rage();
        } else {
            game->new_message("Can't rage.");
        }
        break;

    case SDLK_ESCAPE:
    case 'q':
        game->quit_screen = true;
        break;

    case 'Q':
hard_quit:
        if (game->help_screen) {
            game->help_screen = false;
            return (false);
        }

        if (game->info_screen) {
            game->info_screen = false;
            return (false);
        }

        if (!thing->is_dead) {
            vector<string> messages;

            messages.push_back("You hear the sound of a rubber duck.");
            messages.push_back("You hear the sound of a chicken clucking.");
            messages.push_back("You hear the sound of goblins laughing.");
            messages.push_back("You return to the boring world of reality.");
            messages.push_back("You wave farewell to the monsters.");
            messages.push_back("The Gods are disappointed in you...");
            messages.push_back("And I thought you were tough!");
            messages.push_back("Not going yella, are you?");
            messages.push_back("Call yourself noble?");

            string which = one_of(messages);

            if (which == "You hear the sound of a rubber duck.") {
                gh_audio::play("rubber_ducky");
            }

            game->new_message(which);

            game->new_message("Our hero cowardly quit the Epic Quest...");

            //
            // No high score for cowards! Mean, I know. NetHack does this and
            // it seems equally unfair.
            //
            quit = true;

            thing->dead("Cowardly Quit the Epic Quest", true /* explode */);
        }
        break;

    case SDLK_F11:
        enter_level_editor();
        break;

    default:
        if (shortcut_key_use_item(sym)) {
            return (true);
        }

        break;
    }

    return (true);
}

int gh_player::key_up (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_level *l = thing->level;
    if (l && l->freeze_things) {
        return (false);
    }

    if (ignore_game_start_events_ticks > 0) {
        //
        // Keys used to start the game should not fall through to here;
        // like fire.
        //
        return (false);
    }

    if (game->paused) {
        return (false);
    }

    if (game->help_screen) {
        return (false);
    }

    if (game->info_screen) {
        return (false);
    }

    if (thing->is_dead) {
        return (false);
    }

    switch (sym.sym) {
    case SDLK_KP7:
        thing->direction_wanted.x = 0;
        thing->direction_wanted.y = 0;
        thing->update_anim();
        break;

    case SDLK_KP9:
        thing->direction_wanted.x = 0;
        thing->direction_wanted.y = 0;
        thing->update_anim();
        break;

    case SDLK_KP1:
        thing->direction_wanted.x = 0;
        thing->direction_wanted.y = 0;
        thing->update_anim();
        break;

    case SDLK_KP3:
        thing->direction_wanted.x = 0;
        thing->direction_wanted.y = 0;
        thing->update_anim();
        break;

    case SDLK_KP4:
    case SDLK_LEFT:
        thing->direction_wanted.x = 0;
        thing->update_anim();
        break;

    case SDLK_KP6:
    case SDLK_RIGHT:
        thing->direction_wanted.x = 0;
        thing->update_anim();
        break;

    case SDLK_KP8:
    case SDLK_UP:
        thing->direction_wanted.y = 0;
        thing->update_anim();
        break;

    case SDLK_KP2:
    case SDLK_DOWN:
        thing->direction_wanted.y = 0;
        thing->update_anim();
        break;

    default:
        break;
    }

    return (true);
}

int gh_player::editing_key_held (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_level *l = thing->level;

    if (game->help_screen) {
        return (false);
    }

    switch (sym.mod) {
        case KMOD_SHIFT:
        case KMOD_LSHIFT:
        case KMOD_RSHIFT:
            switch (sym.sym) {
                default:
                break;
            }
            break;

        case KMOD_ALT:
        case KMOD_LALT:
        case KMOD_RALT:
            switch (sym.sym) {
                default:
                break;
            }
            break;

        case KMOD_CTRL:
        case KMOD_LCTRL:
        case KMOD_RCTRL:
            switch (sym.sym) {
                default:
                break;
            }
            return (true);

        default:
            break;
    }

    switch (sym.sym) {
    case SDLK_KP7:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(-thing->speed, -thing->speed), gh_point(0,0));
        break;

    case SDLK_KP9:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(thing->speed, -thing->speed), gh_point(0,0));
        break;

    case SDLK_KP1:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(-thing->speed, thing->speed), gh_point(0,0));
        break;

    case SDLK_KP3:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(thing->speed, thing->speed), gh_point(0,0));
        break;

    case SDLK_KP4:
    case SDLK_LEFT:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(-thing->speed, 0), gh_point(0,0));
        break;

    case SDLK_KP6:
    case SDLK_RIGHT:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(thing->speed, 0), gh_point(0,0));
        break;

    case SDLK_KP8:
    case SDLK_UP:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(0, -thing->speed), gh_point(0,0));
        break;

    case SDLK_KP2:
    case SDLK_DOWN:
        thing->speed = l->is_editing_grid_res;
        mouse_movement(gh_point(0, thing->speed), gh_point(0,0));
        break;

    default:
        break;
    }

    switch (sdl_evt_to_char(sym)) {
    case 'a':
        l->is_editing_which_thing[l->is_editing_map_char]--;

        if (l->is_editing_which_thing[l->is_editing_map_char] < 0) {
            l->is_editing_which_thing[l->is_editing_map_char] = 0;
        }

        break;

    case 'z':
        l->is_editing_which_thing[l->is_editing_map_char]++;
        break;

    case ' ':
        //
        // Ok, does the same thing. Place a thing.
        //
        mouse_down(0);
        break;

    default:
        break;
    }

    return (true);
}

//
// Called upon completion of entering a name for this thing.
//
static void editing_thing_renamed_callback (void *context, string result)
{
    gh_thing *editing_thing = (gh_thing*)context;

    GH_THING_VERIFY(editing_thing);

    if (editing_thing) {
        string replace;

        //
        // If no colour specified, use white.
        //
        if (result[0] != '%') {
            string prefix = "%%fg=white";
        
            replace = prefix + result;
        } else {
            replace = result;
        }

        editing_thing->text = replace;
    }
}

//
// Called upon completion of entering an exit name for this thing.
//
static void editing_thing_exit_jump_callback (void *context, string result)
{
    gh_thing *editing_thing = (gh_thing*)context;

    GH_THING_VERIFY(editing_thing);

    if (editing_thing) {
        editing_thing->exit_name = result;
        editing_thing->is_exit = true;

        string replace;

        //
        // If no colour specified, use white.
        //
        if (result[0] != '%') {
            string prefix = "%%fg=white";
        
            replace = prefix + result;
        } else {
            replace = result;
        }
    }
}

int gh_player::editing_key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    if (game->help_screen) {
        game->help_screen = false;
        gh_audio::play("Light_Switch_004");
        return (false);
    }

    gh_level *l = thing->level;

    gh_thing *chosen_thing;

    string editing = l->map_char_nth(l->is_editing_map_char,
                                     l->is_editing_which_thing[
                                     l->is_editing_map_char]);

    chosen_thing = gh_thing::find(editing, false /* error */);

    switch (sym.mod) {
        case KMOD_SHIFT:
        case KMOD_LSHIFT:
        case KMOD_RSHIFT:
            switch (sym.sym) {
                default:
                break;
            }
            break;

        case KMOD_ALT:
        case KMOD_LALT:
        case KMOD_RALT:
            switch (sym.sym) {
                default:
                break;
            }
            break;

        case KMOD_CTRL:
        case KMOD_LCTRL:
        case KMOD_RCTRL:
            switch (sym.sym) {
                case 'f': {
                    if (chosen_thing) {
                        //
                        // Pop the hero so it doesn't collide with the flood
                        // fill routine.
                        //
                        gh_point3d at = thing->at;

                        l->map.pop(thing);
                        l->map.fill_until(at, chosen_thing,
                                          is_true, NULL);
                        l->map.push(thing);
                    }
                    return (true);
                }

                case 'g':
                    switch (l->is_editing_grid_res) {

                    case GH_RES:
                        l->is_editing_grid_res = 1;
                        break;

                    case GH_RES_CHAR:
                        l->is_editing_grid_res = GH_RES;
                        break;

                    case 1:
                        l->is_editing_grid_res = GH_RES_CHAR;
                        break;
                    }
                    return (true);

                default:
                break;

                case 'n': {
                    //
                    // Get the hero off of the level so they can persist
                    // the level destruction.
                    //
                    l->map.pop(thing);
                    thing->destroy_all_items();

                    game->delete_all_messages();

                    //
                    // Reset the level draw bounds.
                    //
                    l->draw_start = gh_point3d(-1,-1,-1);
                    l->draw_end   = gh_point3d(-1,-1,-1);

                    new_level(level_set_name,
                              level_no,
                              which_level,
                              true /* want_empty_level */);

                    //
                    // Push the cursor/hero back on at the center of the new
                    // map.
                    //
                    l = thing->level;
                    l->map.push(thing);
                    l->map.add_border();

                    gh_point3d middle((l->map.size.w * GH_RES) / 8,
                                      (l->map.size.h * GH_RES) / 8,
                                      0);
                    thing->move_to(middle);

                    game->replace_last_message("Level reset");

                    l->is_editing = true;

                    l->map.hero_at = middle;
                    l->map.hero_map_at = middle / l->map. res;

                    l->cursor_at.x = l->map.hero_at.x;
                    l->cursor_at.y = l->map.hero_at.y;

                    SDL_WM_GrabInput(SDL_GRAB_ON);

                    myscreen.auto_repeat_rate = 10;
                    myscreen.auto_repeat_delay = 5;

                    return (true);
                }

                case 's': {
                    //
                    // Save the hero where the cursor is. It's where they
                    // will be born.
                    //
                    l->hero_start_where = thing->at;
                    l->hero_end_where = gh_point3d(-1,-1,-1);

                    string name = game->level_set_name + "/" +
                                    tostring(level_no, 3, '0') + "/" +
                                    which_level + ".gz";

                    //
                    // Pop the hero so he doesn't get saved on the level.
                    //
                    l->map.pop(thing);
                    gh_write_file(l, "levels/", name);
                    l->map.push(thing);

                    game->delete_all_messages();
                    game->replace_last_message("Saved as " + name);

                    return (true);
                }

                case 'q': {
                    exit_level_editor();
                    quit = true;

                    game->want_to_exit_level_editor_mode = true;
                    gh_global::done = true;
                    return (true);
                }

                //
                // Grab a thing.
                //
                case ' ': {
                    mouse_down(2);
                    break;
                }

                //
                // Title a thing.
                //
                case 't': {
                    //
                    // Grab the nearest thing.
                    //
                    mouse_down(2);
                
                    if (editing_thing) {
                        game->replace_last_message("Enter a name for " +
                                          editing_thing->name +
                                          "%%fg=white: ");

                        editing_thing->text =
                            gh_ui_player_new_read_small_text_loop(
                                    game,
                                    (void*)editing_thing,
                                    editing_thing_renamed_callback);

                        editing_thing = NULL;
                    }
                    break;
                }

                //
                // Set dimension jump
                //
                case 'j': {
                    //
                    // Grab the nearest thing.
                    //
                    mouse_down(2);
                
                    if (editing_thing) {
                        game->replace_last_message("Enter a level jump for " +
                                          editing_thing->name +
                                          "%%fg=white: ");

                        editing_thing->exit_name =
                            gh_ui_player_new_read_small_text_loop(
                                    game,
                                    (void*)editing_thing,
                                    editing_thing_exit_jump_callback);

                        editing_thing = NULL;
                    }
                    break;
                }

            }
            return (true);


        default:
            break;
    }

    switch (sym.sym) {
    case SDLK_KP7:
    case SDLK_KP9:
    case SDLK_KP1:
    case SDLK_KP3:
    case SDLK_KP4:
    case SDLK_LEFT:
    case SDLK_KP6:
    case SDLK_RIGHT:
    case SDLK_KP8:
    case SDLK_UP:
    case SDLK_KP2:
    case SDLK_DOWN:
        return (editing_key_held(sym));

    case SDLK_PAGEDOWN:
        thing->_can_see_distance += 10;
        break;

    case SDLK_PAGEUP:
        thing->_can_see_distance -= 10;
        return (true);

    case SDLK_BACKSPACE:
        mouse_down(3);
        break;

    case SDLK_F11:
        exit_level_editor();
        break;

    default:
        break;
    }

    switch (sdl_evt_to_char(sym)) {
    case 'a':
    case 'z':
        return (editing_key_held(sym));

    default: {
        //
        // Choose a new filter for the things.
        //
        char n = sdl_evt_to_char(sym);

        if (n != '\0') {
            l->is_editing_map_char = sdl_evt_to_char(sym);
        }

        return (true);
    }

    case ' ':
        //
        // Ok, does the same thing.
        //
        mouse_down(0);
        break;

    case 'h':
        game->help_screen = !game->help_screen;
        gh_audio::play("Button_Off_001");
        break;
    }

    return (true);
}

int gh_player::mouse_up (const size_t button)
{
    GH_FUNCTION()

    if (thing->level && thing->level->freeze_things) {
        return (false);
    }

    return (false);
}

int gh_player::mouse_down (const size_t button)
{
    GH_FUNCTION()

    gh_thing *existing_thing;
    gh_thing *chosen_thing;

    gh_level *l = thing->level;

    if (l && l->is_editing) {
        string editing =
            l->map_char_nth(l->is_editing_map_char,
                             l->is_editing_which_thing[l->is_editing_map_char]);
        if (editing == "") {
            return (false);
        }

        chosen_thing = gh_thing::find(editing, false /* error */);
        if (!chosen_thing) {
            GH_THROW("no thing to edit, called " + editing);
        }

        //
        // Round the current cursor position to where we want to snap
        // this item onto.
        //
        gh_point3d at((thing->at.x / l->is_editing_grid_res) *
                          l->is_editing_grid_res,
                      (thing->at.y / l->is_editing_grid_res) *
                          l->is_editing_grid_res,
                      thing->at.z);

        at.x += l->is_editing_grid_res / 2;
        at.y += l->is_editing_grid_res / 2;

        switch (button) {
        case 0:
        case 1:
            editing_thing = NULL;

            //
            // If this is a repeat click at this location, just leave one thing.
            //
            existing_thing = l->map.find_first_thing_same_at(at,
                                                             chosen_thing);
            if (existing_thing != NULL) {
                //
                // Don't place multiple same name things at the same
                // co-ordinate.
                //
                if (existing_thing->at == at) {
                    return (true);
                }
            }

            thing->monst_summon_forced_at(chosen_thing->short_name, at);
            l->map.join();

            return (true);

        case 2:
            if (editing_thing) {
                editing_thing = NULL;
                return (true);
            }

            //
            // Delete the thing of the chosen type.
            //
            editing_thing = l->map.find_first_thing_same(at, chosen_thing);
            if (editing_thing != NULL) {
                if (editing_thing != thing) {
                    game->replace_last_message("Grabbed " + editing_thing->named());
                    return (true);
                }
            }

            //
            // Else just grab the first thing we see. NOT THE HERO.
            //
            editing_thing = l->map.find_thing(at, is_not_hero,
                                              NULL /* context */);
            if (editing_thing != NULL) {
                game->replace_last_message("Grabbed " + editing_thing->named());
                return (true);
            }

            return (true);

        case 3:
            editing_thing = NULL;

            //
            // Delete the thing of the chosen type.
            //
            existing_thing = l->map.find_first_thing_same(at, chosen_thing);
            if (existing_thing != thing) {
                if (existing_thing != NULL) {
                    existing_thing->dead("");;

                    if (existing_thing->is_on_map) {
                        l->map.pop(existing_thing);
                    }

                    existing_thing = NULL;

                    l->map.find_map_extremes();
                    l->map.join();
                    return (true);
                }
            }

            //
            // Else just grab the first thing we see. NOT THE HERO.
            //
            existing_thing = l->map.find_thing(at, is_not_hero,
                                               NULL /* context */);
            if (existing_thing != NULL) {
                existing_thing->dead("");;

                if (existing_thing->is_on_map) {
                    l->map.pop(existing_thing);
                }

                existing_thing = NULL;

                l->map.find_map_extremes();
                l->map.join();
                return (true);
            }

            return (true);

        case 4:
            l->is_editing_which_thing[l->is_editing_map_char]--;

            if (l->is_editing_which_thing[l->is_editing_map_char] < 0) {
                l->is_editing_which_thing[l->is_editing_map_char] = 0;
            }
            return (true);

        case 5:
            l->is_editing_which_thing[l->is_editing_map_char]++;
            return (true);
        }
    } else {
        switch (button) {
            case 4:
                make_item_list();

                //
                // move up in the selected item list
                //
                item_last_selected++;
                item_last_selected =
                                min(item_last_selected, (int)items.size() - 1);
                return (true);

            case 5:
                make_item_list();

                //
                // move down in the selected item list
                //
                item_last_selected--;
                item_last_selected = max(0, item_last_selected);

                return (true);
        }
    }

    return (false);
}

int gh_player::mouse_held (const size_t button)
{
    GH_FUNCTION()

    return (mouse_down(button));
}

int gh_player::mouse_movement (const gh_point & rel, const gh_point &at)
{
    GH_FUNCTION()

    gh_level *l = thing->level;

    if (l) {
        if (l->is_editing) {
            gh_point3d at((l->cursor_at.x / l->is_editing_grid_res) *
                              l->is_editing_grid_res,
                          (l->cursor_at.y / l->is_editing_grid_res) *
                              l->is_editing_grid_res,
                          l->cursor_at.z);

            at.x += l->is_editing_grid_res / 2;
            at.y += l->is_editing_grid_res / 2;

            //
            // Move the player and weapon which we use as a cursor.
            //
            if (thing->move_to(at)) {
                if (thing->weapon) {
                    thing->weapon->move_to(at);
                }

                if (editing_thing) {
                    editing_thing->move_to(at);
                    if (editing_thing->weapon) {
                        editing_thing->weapon->move_to(at);
                    }
                }

                l->cursor_at.x += rel.x;
                l->cursor_at.y += rel.y;
            }
        }
    }

    gh_fpoint f_at(
                  ((float)at.x / (float)gh_screen::vid.w),
            1.0 - ((float)at.y / (float)gh_screen::vid.h));

    gh_fpoint pixel_at(
            (int)(f_at.x * gh_screen::vid.w),
            (int)(f_at.y * gh_screen::vid.h));

    return (false);
}
