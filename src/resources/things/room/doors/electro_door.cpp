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
#include <gh_map.h>

static void init(class gh_thing_registration *my_thing);
static void fini(class gh_thing_registration *my_thing);
static bool open(gh_thing_registration_args *args);
static bool tick(gh_thing_registration_args *args);

static gh_thing_registration my_thing("room/doors/electro_door", &init, &fini);

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

//
// A thing is trying to open us - true on success
//
static bool open (gh_thing_registration_args *args)
{
    GH_FUNCTION()

    gh_thing *door = args->me;
    gh_level *level = door->level;

    vector<string> messages;

    //
    // open/close all other joining doors
    //
    list <gh_thing *> other_doors =
	    level->map.find_all_touching_things_in_adjoining_squares(door);
    list <gh_thing *>::iterator o;

    for (o = other_doors.begin(); o != other_doors.end(); o++) {
	gh_thing *other_door = *o;

	other_door->dead("opened", true /* explode */);
    }

    door->dead("opened", true /* explode */);

    return (true);
}

//
// A time tick has elapsed. Do I want to do anything?
//
static bool tick (gh_thing_registration_args *args)
{
    GH_FUNCTION()

    return (true);
}

