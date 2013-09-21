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

    $Id: door.cpp 264 2006-11-19 02:45:29Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing_registration.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_templates.h>
#include <gh_audio.h>
#include <gh_map.h>

static void init(class gh_thing_registration *my_thing);
static void fini(class gh_thing_registration *my_thing);
static bool open(gh_thing_registration_args *args);
static bool tick(gh_thing_registration_args *args);

static gh_thing_registration my_thing("room/doors/portcullis", &init, &fini);

//
// Initial install up on game start
//
static void init (class gh_thing_registration *my_thing)
{
    GH_FUNCTION()

    my_thing->open = &open;
    my_thing->tick = &tick;
}

//
// Final clean up on game end
//
static void fini (class gh_thing_registration *my_thing)
{
    GH_FUNCTION()

}

class gh_thing_door : public gh_stream
{
public:
    //
    // If opened with a key, where it came from.
    //
    gh_point3d key_source;

    gh_thing_door(void) {}
    ~gh_thing_door(void) {}
};

#define MY_CONTEXT(thing) ((gh_thing_door*)(thing->context))

//
// A thing is trying to open us - true on success
//
static bool open (gh_thing_registration_args *args)
{
    GH_FUNCTION()

    gh_thing *door = args->me;
    gh_thing *thing = args->initiator;
    gh_level *level = door->level;
    bool ret;

    vector<string> messages;

    if (door->is_open) {
	//
	// Try to close it
	//
	if (gh_thing_gfx_can_touch(door, thing)) {
	    //
	    // Is the thing in the way of closing it?
	    //
	    door->cant_see_past = true;
	    door->is_open = true;
	    door->is_closed = false;
	    door->is_destroyable = false;

	    messages.push_back("the " + door->name + " wont shut; you're in the way");

	    ret = false;
	} else {
	    door->cant_see_past = false;
	    door->is_open = false;
	    door->is_closed = true;
	    door->is_destroyable = true;

	    messages.push_back("the " + door->name + " shuts");
	    messages.push_back("the " + door->name + " slams shut");
	    messages.push_back("the " + door->name + " clangs shut");
	    messages.push_back("the " + door->name + " shuts with a resounding thoom");
	    if (thing->is_hero) {
		thing->play("Door_Slam_001");
	    }

	    //
	    // Put a new key back onto the level if we had one.
	    // On the *other side* of the door 8)
	    //
	    if (MY_CONTEXT(door)) {
		gh_thing *key;

		key = new gh_thing(level, "treasure/keys/1",
				   MY_CONTEXT(door)->key_source,
				   NULL /* template thing */);

		level->map.push(key);

		key->decref(GH_THING_REF_FIRST);

		delete MY_CONTEXT(door);
		door->context = NULL;
	    }

	    ret = true;
	}
    } else {
	if (!thing->keys) {
	    messages.push_back(thing->name +
			       string(" has no keys; can't open the ") +
			       door->name);
	    ret = false;
	} else {
	    //
	    // Try to open it
	    //
	    if (!MY_CONTEXT(door)) {
		door->context = new gh_thing_door();
	    }

	    MY_CONTEXT(door)->key_source = thing->at;

	    thing->keys--;

	    door->cant_see_past = true;
	    door->is_open = true;
	    door->is_closed = false;
	    door->is_destroyable = false;

	    messages.push_back("the " + door->name + " opens");
	    messages.push_back("the " + door->name + " creaks open");
	    messages.push_back("the " + door->name + " groans open");
	    messages.push_back("the " + door->name + " shudders open");
	    ret = true;

	    if (thing->is_hero) {
		thing->play("Door_Latch_002");
	    }
	}
    }

    //
    // open/close all other joining doors
    //
    list <gh_thing *> other_doors =
	    level->map.find_all_touching_things_in_adjoining_squares(door);
    list <gh_thing *>::iterator o;

    for (o = other_doors.begin(); o != other_doors.end(); o++) {
	gh_thing *other_door = *o;

	other_door->is_open = door->is_open;
	other_door->is_closed = door->is_closed;
	other_door->is_destroyable = door->is_destroyable;
	other_door->cant_see_past = door->cant_see_past;
	other_door->get_dir_and_mode(&other_door->dir,
				     &other_door->mode);
    }

    if (messages.size()) {
	if (thing->is_hero) {
	    level->game->new_message(one_of(messages));
	}
    }

    return (ret);
}

//
// A time tick has elapsed. Do I want to do anything?
//
static bool tick (gh_thing_registration_args *args)
{
    GH_FUNCTION()

    return (true);
}

