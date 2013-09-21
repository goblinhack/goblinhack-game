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

    $Id: collect.cpp 186 2006-10-08 03:31:22Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_thing.h>

bool gh_thing::wearer_hates_water (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    //
    // Does the thing have this ability intrinsically?
    //
    if (hates_water) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        //
        // If we're wearing something, we inherit its attributes.
        //
        if (t->is_armour || t->is_amulet) {
            if (t->hates_water) {
                return (true);
            }
        }

        i++;
    }

    return (false);
}

bool gh_thing::wearer_hates_fire (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    //
    // Does the thing have this ability intrinsically?
    //
    if (hates_fire) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        //
        // If we're wearing something, we inherit its attributes.
        //
        if (t->is_armour || t->is_amulet) {
            if (t->hates_fire) {
                return (true);
            }
        }

        i++;
    }

    return (false);
}

bool gh_thing::wearer_hates_acid (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    //
    // Does the thing have this ability intrinsically?
    //
    if (hates_acid) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        //
        // If we're wearing something, we inherit its attributes.
        //
        if (t->is_armour || t->is_amulet) {
            if (t->hates_acid) {
                return (true);
            }
        }

        i++;
    }

    return (false);
}

bool gh_thing::wearer_hates_cold (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    //
    // Does the thing have this ability intrinsically?
    //
    if (hates_cold) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        //
        // If we're wearing something, we inherit its attributes.
        //
        if (t->is_armour || t->is_amulet) {
            if (t->hates_cold) {
                return (true);
            }
        }

        i++;
    }

    return (false);
}

bool gh_thing::wearer_hates_suffocation (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    //
    // Does the thing have this ability intrinsically?
    //
    if (hates_suffocation) {
        return (true);
    }

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        //
        // If we're wearing something, we inherit its attributes.
        //
        if (t->is_armour || t->is_amulet) {
            if (t->hates_suffocation) {
                return (true);
            }
        }

        i++;
    }

    return (false);
}
