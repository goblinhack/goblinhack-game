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

    $Id: spawn.cpp 903 2008-05-25 13:56:38Z Ghack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

//
// Summon a thing at a fixed point, with little chance of failure.
//
bool gh_thing::monst_summon_forced_at (string summon_what,
                                       gh_point3d summon_location)
{
    GH_FUNCTION()

    //
    // Create it!
    //
    gh_thing *new_thing = new gh_thing(level, summon_what, summon_location);

    if (!level->map.push(new_thing)) {
        delete new_thing;
        new_thing = NULL;
        return (false);
    }

    if (!new_thing->monst_create_followers()) {
        new_thing->leaves_corpse = false;
        new_thing->dead("summon failed; followers",
                        false, /* explosion */
                        false /* drop items */
                       );
        new_thing = NULL;
        return (false);
    }

    new_thing->decref(GH_THING_REF_FIRST);
    GH_THING_VERIFY(new_thing);

    return (true);
}

//
// Summon a thing next to the creature. It is not owned or possessed.
//
bool gh_thing::monst_summon_adjacent (string summon_what,
                                      bool possess)
{
    GH_FUNCTION()

    string summoned_name;

    const gh_anim_frame *anim_a = get_anim();

    int tries = 0;

    while (tries++ < 200) {
        //
        // Create it!
        //
        gh_thing *new_thing = new gh_thing(level, summon_what, at);

        summoned_name = new_thing->name;

        //
        // Find a direction.
        //
        int x = (gh_math::rand100() - 50) / 30;
        int y = (gh_math::rand100() - 50) / 30;

        //
        // Cater for large things summoning small things or vice
        // versa.
        //
        const gh_anim_frame *anim_b = new_thing->get_anim();

        gh_point3d summon_location((int)(((anim_a->pix_max_w/2) +
                                          (anim_b->pix_max_w/2)) * x),
                                   (int)(((anim_a->pix_max_h/2) +
                                          (anim_b->pix_max_h/2)) * y), 0);

        summon_location += at;

        new_thing->at = gh_point3d(summon_location);

        bool failed = false;

        if (!level->map.push(new_thing)) {
            delete new_thing;
            new_thing = NULL;
            failed = true;
        }

        //
        // Check that we can exist in this space on the map.
        //
        if (!failed) {
            if (new_thing->hit_obstacle(true /* ignore_hero */)) {
                new_thing->leaves_corpse = false;
                new_thing->dead("summon failed; hit obstacle",
                                false, /* explosion */
                                false /* drop items */
                                );
                new_thing = NULL;
                failed = true;
            }
        }

        //
        // And we're not on an existing monster
        //
        if (!failed) {
            if (new_thing->met_biter()) {
                new_thing->leaves_corpse = false;
                new_thing->dead("summon failed; hit biter",
                                false, /* explosion */
                                false /* drop items */
                               );
                new_thing = NULL;
                failed = true;
            }
        }
            
        if (!failed && !new_thing->monst_create_followers()) {
            new_thing->leaves_corpse = false;
            new_thing->dead("summon failed; followers",
                            false, /* explosion */
                            false /* drop items */
                           );
            new_thing = NULL;
            failed = true;
        }

        if (failed) {
            continue;
        }

        if (is_hero) {
            level->game->new_message(name + " summoned " + summoned_name);
        }

        new_thing->meet_all_at_new_position();

        if (possess) {
            //
            // Else we end up trying to possess summoned fruit...
            //
            if (new_thing->is_monst) {
                possess_monst(new_thing);
            }
        }

        new_thing->decref(GH_THING_REF_FIRST);
        GH_THING_VERIFY(new_thing);

        return (true);
    }

    if (is_hero) {
        level->game->new_message(name + 
                                 " %%fg=redfailed%%fg=green to summon " + summoned_name);
    }

    return (false);
}

//
// Summon a thing randomly into the level. It is not owned or possessed.
//
bool gh_thing::monst_summon_in_level (string summon_what)
{
    GH_FUNCTION()

    string summoned_name;

    int attempts = 1000;

    while (attempts--) {
        gh_point3d at((rand() % level->map.size.w) * GH_RES + GH_RES / 2,
                      (rand() % level->map.size.h) * GH_RES + GH_RES / 2,
                      0);
        //
        // Allow teleport onto corridors or rooms.
        //
        if (!level->map.is_on_corridor(at) && !level->map.is_on_floor(at)) {
            continue;
        }

        gh_thing *new_thing = new gh_thing(level, summon_what, at);

        summoned_name = new_thing->name;

        new_thing->debug();

        if (!level->map.push(new_thing)) {
            delete new_thing;
            continue;
        }

        //
        // Check that we can exist in this space on the map.
        //
        if (new_thing->hit_obstacle(true /* ignore_hero */)) {
            new_thing->leaves_corpse = false;
            new_thing->dead("summon failed; hit hero",
                            false, /* explosion */
                            false /* drop items */
                           );
            continue;
        }

        //
        // And we're not on an existing monster
        //
        if (new_thing->met_biter()) {
            new_thing->leaves_corpse = false;
            new_thing->dead("summon failed; hit biter",
                            false, /* explosion */
                            false /* drop items */
                           );
            continue;
        }

        if (!new_thing->monst_create_followers()) {
            new_thing->leaves_corpse = false;
            new_thing->dead("summon failed; followers",
                            false, /* explosion */
                            false /* drop items */
                           );
            continue;
        }

        level->game->new_message(name + " summoned " + summoned_name);

        new_thing->meet_all_at_new_position();
        new_thing->decref(GH_THING_REF_FIRST);
        GH_THING_VERIFY(new_thing);

        return (true);
    }

    level->game->new_message(name + " fails to summon " + summoned_name);

    return (false);
}

//
// Create a wall of protection
//
bool gh_thing::monst_summon_wall (string summon_what,
                                  int radius, 
                                  bool ignore_obstacles,
                                  bool ignore_monsters,
                                  bool possess)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    const gh_anim_frame *anim_a = get_anim();

    GH_THING_LOG(this, "summon " + summon_what);

    int cnt = 0;

    for (int x = -radius; x <= radius; x++) {
        for (int y = -radius; y <= radius; y++) {
            if (!x && !y) {
                continue;
            }

            int tries = 0;

            gh_thing *new_thing = NULL;

            while (tries++ < 100) {
                //
                // Create it!
                //
                new_thing = new gh_thing(level, summon_what, at);

                //
                // Cater for large things summoning small things or vice
                // versa.
                //
                const gh_anim_frame *anim_b = new_thing->get_anim();

                gh_point3d summon_location((int)(((anim_a->pix_min_w/2) +
                                                  (anim_b->pix_min_w/2)) * x),
                                           (int)(((anim_a->pix_min_h/2) +
                                                  (anim_b->pix_min_h/2)) * y), 0);

                if (new_thing->is_movable) {
                    summon_location.x += rand() % res;
                    summon_location.y += rand() % res;
                    summon_location.x -= rand() % res;
                    summon_location.y -= rand() % res;
                }

                GH_THING_LOG(new_thing, "summon");

                summon_location += at;

                new_thing->at = gh_point3d(summon_location);

                bool failed = false;

                if (!level->map.push(new_thing)) {
                    GH_THING_LOG(new_thing, "summon failed");

                    delete new_thing;
                    new_thing = NULL;
                    failed = true;
                }

                //
                // Check that we can exist in this space on the map.
                //
                if (!failed && !ignore_obstacles) {
                    if (new_thing->hit_obstacle(true /* ignore_hero */)) {
                        new_thing->leaves_corpse = false;
                        new_thing->dead("summon failed; hit obstacle",
                                        false, /* explosion */
                                        false /* drop items */
                                        );
                        new_thing = NULL;
                        failed = true;
                    }
                }

                //
                // Check that we can exist in this space on the map.
                //
                if (!failed && !ignore_obstacles) {
                    //
                    // Check for trees overlapping rocks for example.
                    //
                    if (!new_thing->is_monst && new_thing->touching_anything()) {
                        new_thing->leaves_corpse = false;
                        new_thing->dead("summon failed; touching something",
                                        false, /* explosion */
                                        false /* drop items */
                                        );
                        new_thing = NULL;
                        failed = true;
                    }
                }

                //
                // And we're not on an existing monster
                //
                if (!failed && !ignore_monsters) {
                    if (new_thing->met_biter()) {
                        new_thing->leaves_corpse = false;
                        new_thing->dead("summon failed; hit biter",
                                        false, /* explosion */
                                        false /* drop items */
                                        );
                        new_thing = NULL;
                        failed = true;
                    }
                }

                if (!failed && !new_thing->monst_create_followers()) {
                    new_thing->leaves_corpse = false;
                    new_thing->dead("summon failed; followers",
                                    false, /* explosion */
                                    false /* drop items */
                                    );
                    new_thing = NULL;
                    failed = true;
                }

                if (!failed && new_thing->met_hero()) {
                    new_thing->leaves_corpse = false;
                    new_thing->dead("summon failed; hit hero",
                                    false, /* explosion */
                                    false /* drop items */
                                    );
                    new_thing = NULL;
                    failed = true;
                }
                    
                if (!failed) {
                    break;
                }
            }

            if (!new_thing) {
                continue;
            }

            new_thing->meet_all_at_new_position();

            if (possess) {
                possess_monst(new_thing);
            }

            new_thing->decref(GH_THING_REF_FIRST);
            GH_THING_VERIFY(new_thing);

            cnt++;
        }
    }

    if (is_hero) {
        if (!cnt) {
            level->game->new_message(name + " failed to summon " + summon_what);
        } else {
            level->game->new_message(name + " summoned " + summon_what);
        }
    }

    return (cnt > 0);
}
