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

    $Id: player.cpp 1241 2009-07-10 21:32:09Z Ghack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_templates.h>

gh_player::gh_player (class gh_game *game)
{
    GH_FUNCTION()

    level_set_name = "random";
    level_no = 1;
    which_level = "dummy";

    thing = NULL;
    editing_thing = NULL;
    thing_name = "";
    this->game = game;
    item_last_selected = 0;
    last_level_set_name = "";
    quit = false;

    myscreen.name = __FILE__;
    myscreen.classbase = this;
    myscreen.priority = 10;

    myscreen.init =
        (gh_screen::gh_screen_init)&gh_player::init;
    myscreen.fini =
        (gh_screen::gh_screen_fini)&gh_player::fini;
    myscreen.tick =
        (gh_screen::gh_screen_tick)&gh_player::tick;
    myscreen.display =
        (gh_screen::gh_screen_display)&gh_player::display;
    myscreen.reset =
        (gh_screen::gh_screen_reset)&gh_player::reset;
    myscreen.key_down =
        (gh_screen::gh_screen_key_down)&gh_player::key_down;
    myscreen.key_up =
        (gh_screen::gh_screen_key_up)&gh_player::key_up;
    myscreen.key_held =
        (gh_screen::gh_screen_key_held)&gh_player::key_held;
    myscreen.mouse_up =
        (gh_screen::gh_screen_mouse_up)&gh_player::mouse_up;
    myscreen.mouse_down =
        (gh_screen::gh_screen_mouse_down)&gh_player::mouse_down;
    myscreen.mouse_held =
        (gh_screen::gh_screen_mouse_held)&gh_player::mouse_held;
    myscreen.mouse_movement =
        (gh_screen::gh_screen_mouse_movement)&gh_player::mouse_movement;
}

gh_player::~gh_player (void)
{
    GH_FUNCTION()

    //
    // Get rid of all we're carrying so the level cleanup gets it.
    //
    if (thing != NULL) {
        thing->destroy_all_items();
    }

    remove_level();

    if (thing != NULL) {
        //
        // We don't own the player thing anymore.
        //
        if (thing->isref(GH_THING_REF_FIRST)) {
            thing->decref(GH_THING_REF_FIRST);
        }

        thing->decref(GH_THING_REF_PLAYER);

        thing = NULL;
    }
}

void gh_player::remove_level (void)
{
    GH_FUNCTION()

    if (thing == NULL) {
        return;
    }

    GH_THING_VERIFY(thing);

    if (thing->level != NULL) {
        delete thing->level;
        thing->level = NULL;
    }

    //
    // Erase all visited room history.
    //
    erase(thing->visited);

    //
    // Make sure all carried items forget where they came from -
    // so we have no problems when loading this game again. The rooms were
    // part of the old level and should have been cleaned up when we left
    // a shop, for example. Still, best to make sure now that we are going
    // into a new level.
    //
    thing->reset_all_items();
}

void gh_player::new_level (string level_set_name,
                           uint32 level_no,
                           string which_level,
                           bool want_empty_level)
{
    GH_FUNCTION()

    gh_level *level;

    remove_level();

    this->level_set_name    = level_set_name;
    this->level_no          = level_no;
    this->which_level       = which_level;

    string level_name = "levels/" +
                        level_set_name + "/" +
                        tostring(level_no, 3, '0') + "/" +
                        which_level;
                
    GH_LOG("New level " + level_name);

    try {
        if (game->want_to_enter_level_editor_mode) {
            if (gh_global::exists(level_name)) {
                GH_LOG(" Existing edited level");
                level = new gh_level(game, level_no, which_level,
                                     level_set_name, level_name,
                                     want_empty_level);
            } else {
                GH_LOG(" New edited level");
                level = new_level_editor();
            }
        } else {
            level = new gh_level(game, level_no, which_level,
                                 level_set_name, level_name,
                                 want_empty_level);
        }

        if (thing) {
            thing->level = level;
        }
    } catch (...) {
        if (thing) {
            thing->level = NULL;
        }

        GH_THROW("failed to build level " + level_name);
    }

    if (!thing) {
       if (!want_empty_level) {
           GH_THROW("was there a thing specified in the level map?");
       }
    }

    GH_THING_VERIFY(thing);
}

void operator>> (gh_stream_in& is, gh_player *me)
{
    GH_FUNCTION()

    me->get(is, &me->level_set_name, "level_set");
    me->get(is, &me->level_no, "level_no");
    me->get(is, &me->thing_name, "thing");
    me->get(is, &me->last_level_set_name, "last_level_set_name");

    me->get_opt_container(is, &me->met, "met");

    //
    // If loading a new player then we need a placeholder whilst we
    // hook in the level things.
    //
    if (me->thing == NULL) {
        me->thing = new gh_thing();
    }

    me->get_opt_classp(is, &me->thing->level, "level");
}

void operator<< (gh_stream_out& os, const gh_player *me)
{
    GH_FUNCTION()

    me->put(os, &me->level_set_name, "level_set");
    me->put(os, &me->level_no, "level_no");
    me->put(os, &me->thing_name, "thing");
    me->put(os, &me->last_level_set_name, "last_level_set_name");

    me->put_container(os, &me->met, "met");

    if (me->thing) {
        me->put_class(os, me->thing->level, "level");
    }
}

void gh_player::init (void)
{
    GH_FUNCTION()

}

void gh_player::fini (void)
{
    GH_FUNCTION()
}

void gh_player::tick (void)
{
    GH_FUNCTION()
}

void gh_player::reset (void)
{
    GH_FUNCTION()
}
