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
#include <gh_math.h>

//
// Sum up all armour and aumulet benefits to find the overall proofing.
//
int gh_thing::acid_proof (void)
{
    GH_FUNCTION()

    //
    // Add in the base plus bonuses
    //
    int proofing = _acid_proof + acid_proof_boost;

    //
    // Intrinsic benefits
    //
    if (loves_acid) {
        proofing += 100;
    }

    if (hates_acid) {
        proofing -= 20;
    }

    //
    // Sum up all arour and amulet benfits.
    //
    list<gh_thing *>::iterator i = carrying.begin();

    while (i != carrying.end()) {
        gh_thing *t = *i;

        if (t->is_armour || t->is_amulet) {
            proofing += t->_acid_proof;
        }

        i++;
    }

    //
    // The end proofing result
    //
    return (proofing);
}

//
// Take into account all bonuses and armour and see if we randomly
// survive this 'attack/function call'
//
bool gh_thing::is_acid_proof (void)
{
    GH_FUNCTION()

    return (gh_math::rand100() < acid_proof());
}

//
// Give the thing a boost of "acid_proof", that fades away
//
void gh_thing::acid_proof_boost_start (int boost)
{
    GH_FUNCTION()

    if (!boost) {
        return;
    }

    acid_proof_boost += boost;
}

//
// If a boost is in operation, gradually ease off of its effects.
//
void gh_thing::acid_proof_boost_tick (void)
{
    GH_FUNCTION()

    if (acid_proof_boost > 0) {
        acid_proof_boost--;
    } else if (acid_proof_boost < 0) {
        acid_proof_boost++;
    }
    if (acid_proof_boost == 0) {
        acid_proof_boost_stop();
    }
}

//
// The boost is done. Undo any settings.
//
void gh_thing::acid_proof_boost_stop (void)
{
    GH_FUNCTION()
}
