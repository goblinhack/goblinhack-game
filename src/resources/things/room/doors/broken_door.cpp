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

    $Id: broken door.cpp 264 2006-11-19 02:45:29Z goblinhack $
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

static gh_thing_registration my_thing("room/doors/broken_door", &init, &fini);

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

    gh_thing *broken_door = args->me;
    gh_level *level = broken_door->level;
    bool ret;

    vector<string> messages;

    messages.push_back("the " + broken_door->name + " crumbles to dust");

    broken_door->dead("opened", true /* explode */);

    ret = false;

    if (messages.size()) {
	level->game->new_message(one_of(messages));
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

