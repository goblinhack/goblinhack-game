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

    $Id: collect.cpp 185 2006-10-08 01:50:52Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_thing.h>
#include <gh_game.h>
#include <gh_level.h>

bool gh_thing::can_rage (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    if (ability_can_rage) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (!t->is_armour && !t->is_amulet) {
            i++;
            continue;
        }

        if (!t->ability_can_rage) {
            i++;
            continue;
        }

        return (true);
    }

    return (false);
}

bool gh_thing::can_auto_rage (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    if (ability_can_auto_rage) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (!t->is_armour && !t->is_amulet) {
            i++;
            continue;
        }

        if (!t->ability_can_auto_rage) {
            i++;
            continue;
        }

        return (true);
    }

    return (false);
}

bool gh_thing::can_regenerate1 (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    if (ability_can_regenerate1) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (!t->is_armour && !t->is_amulet) {
            i++;
            continue;
        }

        if (!t->ability_can_regenerate1) {
            i++;
            continue;
        }

        return (true);
    }

    return (false);
}

bool gh_thing::can_regenerate2 (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    if (ability_can_regenerate2) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (!t->is_armour && !t->is_amulet) {
            i++;
            continue;
        }

        if (!t->ability_can_regenerate2) {
            i++;
            continue;
        }

        return (true);
    }

    return (false);
}

bool gh_thing::can_regenerate3 (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    if (ability_can_regenerate3) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (!t->is_armour && !t->is_amulet) {
            i++;
            continue;
        }

        if (!t->ability_can_regenerate3) {
            i++;
            continue;
        }

        return (true);
    }

    return (false);
}

