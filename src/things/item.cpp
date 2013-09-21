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
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_player.h>

//
// Returns the weight in pounds
//
int16 gh_thing::how_much_am_i_carrying (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;
    int16 w = 0;

    i = carrying.begin();
    while (i != carrying.end()) {
        w += (*i)->weight;
        i++;
    }

    return (w);
}

//
// Returns the weight in pounds
//
bool gh_thing::can_i_carry (gh_thing *item)
{
    GH_FUNCTION()

    if ((item->weight + how_much_am_i_carrying()) >
        (strength() + (strength() / 2))) {
        return (false);
    }

    return (true);
}

//
// Destroy all carried items
//
void gh_thing::destroy_all_items (void)
{
    GH_FUNCTION()

    if (carrying.begin() == carrying.end()) {
        return;
    }

    GH_THING_LOG(this, "destroy all items");

    for (;;) {

        if (carrying.begin() == carrying.end()) {
            return;
        }

        list<gh_thing *>::iterator i  = carrying.begin();
        gh_thing *item = *i;

        item->incref(GH_THING_REF_DESTROY_ITEM);

        //
        // Remove from the carry list.
        //
        carrying.pop_front();

        //
        // I'm no longer being carried.
        //
        item->carrier = NULL;

        GH_THING_LOG(item, "not being carried");

        //
        // Add to the dead list before removing the potentially
        // last ref
        //
        item->dead("destroyed all items", false /* explode */);
        item->decref(GH_THING_REF_DESTROY_ITEM);
    }
}

//
// Reset all carried items w.r.t. room ownership
//
void gh_thing::reset_all_items (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i  = carrying.begin();

    while (i != carrying.end()) {
        gh_thing *item = *i;

        item->room = NULL;

        i++;
    }
}

//
// Make all items weightless
//
void gh_thing::weightless_all_items (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i  = carrying.begin();

    while (i != carrying.end()) {
        gh_thing *item = *i;

        item->weight = 0;

        i++;
    }
}

//
// Make all items heavy
//
void gh_thing::heavy_all_items (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i  = carrying.begin();

    while (i != carrying.end()) {
        gh_thing *item = *i;

        item->weight = 100;

        i++;
    }
}

//
// Destroy one carried item
//
void gh_thing::destroy_item (gh_thing *item)
{
    GH_FUNCTION()

    //
    // Remove safely from the list.
    //
    list<gh_thing *>::iterator i  = carrying.begin();
    while (i != carrying.end()) {
        if (*i == item) {
            item->incref(GH_THING_REF_DESTROY_ITEM);

            carrying.erase(i);

            //
            // I'm no longer being carried.
            //
            item->carrier = NULL;

            //
            // Add to the dead list before removing the potentially
            // last ref
            //
            item->dead("destroyed", false /* explode */);
            item->decref(GH_THING_REF_DESTROY_ITEM);

            return;
        }

        i++;
    }
}
