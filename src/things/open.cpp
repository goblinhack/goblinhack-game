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

    $Id: open.cpp 747 2008-03-16 01:45:09Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_thing_registration.h>
#include <gh_level.h>
#include <gh_game.h>

void gh_thing::open_nearest_thing (void)
{
    GH_FUNCTION()

    vector <gh_thing *> things = get_action_things(is_openable);

    if (!things.size()) {
        level->game->new_message("Nothing to open here");
        return;
    }

    gh_thing *open_this;

    open_this = things[0];

    open_this->open(this);

    open_this->update_anim();
}

void gh_thing::open_this (gh_thing *target)
{
    GH_FUNCTION()

    target->open(this);

    target->update_anim();
}
