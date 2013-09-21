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

    $Id: spawn.cpp 1139 2008-12-25 22:39:10Z Ghack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

bool gh_thing::monst_spawn (void)
{
    GH_FUNCTION()

    if (gh_global::monst_count > 50) {
        return (false);
    }

    if (!spawns.size()) {
        return (false);
    }

    //
    // Don't allow the thing to spawn too often.
    //
    if (!gh_have_x_tenths_passed_since(spawn_delay_in_tenths,
                                       spawn_last_when)) {
        return (false);
    }

    //
    // Add a random delta so the next spawns don't all happen together.
    //
    spawn_last_when = gh_get_time_milli() + gh_math::rand1000();

    if (gh_global::really_slow) {
        return (false);
    }

    if (gh_global::too_slow) {
        return (false);
    }

    //
    // Find something to spawn. Try for a few times then give up.
    //
    int spawn_what = 0;

    uint8 i;
    for (i = 0; i < spawns.size(); i++) {
        spawn_what = rand() % spawns.size();

        if (gh_math::rand100() < spawning_chance[spawn_what]) {
            break;
        }
    }

    //
    // Found anything?
    //
    if (i == spawns.size()) {
        return (false);
    }

    //
    // Work out how big we are to see where we can spawn the spawnee
    //
    if (!update_anim()) {
        return (false);
    }

    const gh_anim_frame *anim_a = get_anim();

    //
    // Create it!
    //
    gh_thing *new_thing = new gh_thing(level, spawns[spawn_what], at);

    //
    // Now see where it will fit around the current thing?
    //
    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);
    gh_point3d orig_at = at;

    vector<gh_point3d> possible;

    //
    // Find a direction.
    //
    int r = gh_math::rand100();
    int x, y;

    if (r < 25) {
        x = -1;
        y = 0;
    } else if (r < 50) {
        x = 1;
        y = 0;
    } else if (r < 75) {
        x = 0;
        y = -1;
    } else {
        x = 0;
        y = 1;
    }

    //
    // Cater for large things spawning small things or vice
    // versa.
    //
    const gh_anim_frame *anim_b = new_thing->get_anim();

    gh_point3d spawn_location((int)(((anim_a->pix_max_w +
                                      anim_b->pix_max_w) / 2) * x),
                              (int)(((anim_a->pix_max_h +
                                      anim_b->pix_max_h) / 2) * y), 0);

    spawn_location += at;

    new_thing->at = gh_point3d(spawn_location);

    bool failed = false;

    if (!level->map.push(new_thing)) {
        new_thing->leaves_corpse = false;
        new_thing->dead("spawn failed; map push",
                        false /* explode */,
                        false /* drop items */);
        new_thing = NULL;
        failed = true;
    }

    //
    // Check that we can exist in this space on the map.
    //
    if (!failed) {
        if (new_thing->hit_obstacle(true /* ignore_hero */)) {
            new_thing->leaves_corpse = false;
            new_thing->dead("spawn failed; hit obstacle",
                            false /* explode */,
                            false /* drop items */);
            new_thing = NULL;
            failed = true;
        }
    }

    if (failed) {
        if (gh_global::debug) {
            GH_THING_LOG(this, "failed to spawn; slow spawn down from " +
                         tostring(spawn_delay_in_tenths) + " to " +
                         tostring(spawn_delay_in_tenths * 2));
        }

        spawn_delay_in_tenths += spawn_delay_in_tenths / 2;

        //
        // We return true to say that an attempt at least was made.
        //
        return (true);
    }

    //
    // Crank the delay back down.
    //
    gh_thing *template_thing = gh_thing::find(short_name, true /* error */);

    spawn_delay_in_tenths = template_thing->spawn_delay_in_tenths;

    if (gh_global::debug) {
        GH_THING_LOG(this, "spawned " + new_thing->name);
    }

    new_thing->meet_all_at_new_position();
    if (new_thing->is_dead) {
        //
        // What a way to die.
        //
        return (true);
    }

    new_thing->decref(GH_THING_REF_FIRST);

    //
    // Make the new thing do thy bidding!
    //
    if (is_monst) {
        possess_monst(new_thing);
    }

    GH_THING_VERIFY(new_thing);

    new_thing->group_id = id;

    return (true);
}
