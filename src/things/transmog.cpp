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

    $Id: transmog.cpp 62 2006-09-04 04:09:48Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

bool gh_thing::monst_transmog (void)
{
    GH_FUNCTION()

    //
    // Just in case...
    //
    if (is_dead) {
        return (false);
    }

    if (!transmogrifies_into.size()) {
        return (false);
    }

    //
    // Don't allow the thing to transmog too often.
    //
    if (!gh_have_x_tenths_passed_since(transmog_delay_in_tenths,
                                       transmog_last_when)) {
        return (false);
    }

    //
    // Add a random delta so the next transmogs don't all happen together.
    //
    transmog_last_when = gh_get_time_milli() + gh_math::rand1000();

    //
    // Find something to transmogrify into. Try a few times then give up.
    //
    int transmogrify_into_what = 0;

    uint8 i;
    for (i = 0; i < transmogrifies_into.size(); i++) {
        transmogrify_into_what = rand() % transmogrifies_into.size();

        if (gh_math::rand100() < transmogrify_chance[transmogrify_into_what]) {
            break;
        }
    }

    //
    // Found anything?
    //
    if (i == transmogrifies_into.size()) {
        return (false);
    }

    //
    // Create it! At the same location of our thing.
    //
    gh_thing *new_thing =
        new gh_thing(level, transmogrifies_into[transmogrify_into_what], at);

    new_thing->incref(GH_THING_REF_TRANSMOG);
    new_thing->decref(GH_THING_REF_FIRST);

    GH_THING_VERIFY(new_thing);

    new_thing->at = gh_point3d(at);

    new_thing->attacks = attacks;
    new_thing->attack_chance = attack_chance;
    new_thing->follows = follows;
    new_thing->follow_chance = follow_chance;

    if (!level->map.push(new_thing)) {
        new_thing->decref(GH_THING_REF_TRANSMOG);
        delete new_thing;
        return (false);
    }

    //
    // Check that we can exist in this space on the map.
    //
    if (new_thing->hit_obstacle(true /* ignore_hero */)) {
        level->map.pop(new_thing);
        new_thing->decref(GH_THING_REF_TRANSMOG);
        delete new_thing;
        return (false);
    }

    GH_THING_LOG(this, "transmogrified into " + new_thing->named());

    //
    // Keep our new thing. It should be on the map.
    //
    new_thing->decref(GH_THING_REF_TRANSMOG);
    new_thing->meet_all_at_new_position();
    new_thing = NULL;

    //
    // new_thing might be dead now!
    //

    dead("transmogrified", false /* explode */, false /* drop things */);

    return (true);
}
