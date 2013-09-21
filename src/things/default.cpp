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

    $Id: default.cpp 621 2008-01-04 15:25:24Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_thing_registration.h>

//
// Default callbacks for when none are installed.
//

//
// A thing is trying to open us - true on success
//
static bool xxx (gh_thing_registration_args *args)
{
    GH_FUNCTION()

    return (false);
}

//
// Does the thing have an open callback other than the default?
//
bool is_openable (gh_thing *thing, void *initiator)
{
    GH_FUNCTION()

    gh_thing_registration_args args;

    args.initiator = (gh_thing *) initiator;

    return (thing->reg->open != &xxx);
}

//
// Call the things open callback?
//
bool gh_thing::open (gh_thing *initiator)
{
    GH_FUNCTION()

    gh_thing_registration_args args;

    args.initiator = (gh_thing *) initiator;
    args.me = (gh_thing *) this;

    return (reg->open(&args));
}

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

    my_thing->open = &xxx;
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
// Initial initial! install up on game start
//
void gh_thing_registration_default_init (void)
{
    GH_FUNCTION()

    static gh_thing_registration my_thing("default", init, fini);
}
