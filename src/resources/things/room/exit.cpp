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

    $Id: exit.cpp 781 2008-04-07 01:37:10Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_thing_registration.h>

static void init(class gh_thing_registration *my_thing);
static void fini(class gh_thing_registration *my_thing);
static bool tick(gh_thing_registration_args *args);
static bool open(gh_thing_registration_args *args);

static gh_thing_registration my_thing("room/exits/1", init, fini);
static gh_thing_registration my_thing2("room/exits/2", init, fini);
static gh_thing_registration my_thing3("room/exits/3", init, fini);
static gh_thing_registration my_thing4("room/exits/4", init, fini);
static gh_thing_registration my_thing5("room/exits/5", init, fini);
static gh_thing_registration my_thing6("room/exits/6", init, fini);
static gh_thing_registration my_thing7("room/exits/7", init, fini);
static gh_thing_registration my_thing_warp("room/exits/warp", init, fini);

//
// A time tick has elapsed. Do I want to do anything?
//
static bool tick (gh_thing_registration_args *args)
{
    GH_FUNCTION()

    return (true);
}

//
// Initial install up on game start
//
static void init (class gh_thing_registration *my_thing)
{
    GH_FUNCTION()

    my_thing->tick = tick;
    my_thing->open = &open;
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

    gh_thing *exit = args->me;

    exit->is_open = !exit->is_open;

    return (true);
}
