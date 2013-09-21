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

    $Id: hit.cpp 548 2007-08-06 22:11:45Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_level.h>

int gh_thing::bravery_level (void)
{
    GH_FUNCTION()

    return ((bravery / 1000) + 1);
}

void gh_thing::bravery_add_bonus (gh_thing *victim)
{
    //
    // If you kill a monster tougher than your level, you get
    // bravery.
    //
    if (!victim->is_monst) {
        return;
    }

    //
    // Shooting moving walls isn't brave.
    //
    if (!victim->intelligence) {
        //
        // Except golems.
        //
        if (!victim->is_undead) {
            return;
        }
    }

    int my_toughness = toughness();
    int victim_toughness = victim->toughness();

    if (victim_toughness < my_toughness) {
        //
        // Cowardly.
        //
        return;
    }

    int diff = victim_toughness - my_toughness;

    //
    // So about 1000 for killing a dragon
    //
    diff *= 5;

    bravery += diff;
    bravery_curr += diff;
}
