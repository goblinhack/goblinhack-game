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

//
// One thing is using another thing, t
//
// returns false once the thing is used up
//
bool gh_thing::use_armour (gh_thing *item)
{
    GH_FUNCTION()

    if (!is_hero) {
        return (true);
    }

    //
    // We've not actually picked the armour up yet so we can check to see
    // if we'll inherit any attributes and warn the user.
    //
    if (item->hates_suffocation && !wearer_hates_suffocation()) {
        level->game->new_message("%%fg=blueYou feel ... choke!...%%fg=green");
    }

    if (item->hates_cold && !wearer_hates_cold()) {
        level->game->new_message("%%fg=blueYou feel cold...%%fg=green");
    }

    if (item->hates_fire && !wearer_hates_fire()) {
        level->game->new_message("%%fg=redYou feel hot...%%fg=green");
    }

    if (item->hates_acid && !wearer_hates_acid()) {
        level->game->new_message("%%fg=redYou feel a burning sensation...%%fg=green");
    }

    return (true);
}

//
// Returns the amount of armour being worn
//
int gh_thing::how_much_armour_am_i_wearing (void)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;
    int a = 0;

    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (!t->is_armour) {
            i++;
            continue;
        }

        a += t->armour;

        i++;
    }

    //
    // Add on the natural armour rating.
    //
    return (a + armour);
}

//
// Take a blow to your armour!
//
void gh_thing::hit_armour (int damage)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i;

    //
    // Check we have some armour to damage.
    //
    if (!how_much_armour_am_i_wearing()) {
        return;
    }

redo:
    //
    // First walk our worn armour and damage what we can!
    //
    i = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *t = *i;

        //
        // The damage might be worn down to 0 from previous hits.
        //
        if (!damage) {
            i++;
            return;
        }

        //
        // Only interested in armour.
        //
        if (!t->is_armour) {
            i++;
            continue;
        }

        //
        // Have we destroyed a piece of armour completely?
        //
        if (damage >= t->armour) {
            //
            // Yes, Kill the armour piece.
            //
            if (is_hero) {
                level->game->new_message("Your " + t->named() +
                                         " is destroyed");
            }

            //
            // Might still be some more damage to do.
            //
            damage -= t->armour;
            t->armour = 0;

            destroy_item(t);
            goto redo;
        }

        //
        // Partially destroyed some armour.
        //
        t->armour -= damage;
        damage = 0;

        return;
    }

    //
    // Still some damage left to do? Hit the natural armour.
    //
    if (damage) {
        //
        // Have we destroyed the natural armour completely?
        //
        if (damage >= armour) {
            damage = 0;
            armour = 0;
        }

        //
        // Partially destroyed your natural armour. Still some left.
        //
        armour -= damage;
        damage = 0;
    }

    //
    // Check to see if we've lost all our armour.
    //
    if (is_hero) {
        if (!how_much_armour_am_i_wearing()) {
            level->game->new_message("%%fg=yellowYour armour is gone!");
            level->game->new_message("You are practically naked now!");
        }
    }
}
