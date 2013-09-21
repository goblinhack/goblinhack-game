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

    $Id: hit.cpp 419 2007-01-27 23:58:24Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_subsys.h>
#include <gh_math.h>

//
// One thing is teleporting another thing, victim
//
bool gh_thing::teleport_attack (gh_thing *victim)
{                       
    GH_FUNCTION()

    int attempts = 100000;

    gh_point3d old = victim->at;

    while (attempts--) {
        gh_point3d at((rand() % level->map.size.w) * GH_RES + GH_RES / 2,
                      (rand() % level->map.size.h) * GH_RES + GH_RES / 2,
                      0);

        //
        // Ensure we teleport a good distance.
        //
        if (GH_DISTANCE(at.x, at.y, victim->at.x, victim->at.y) < GH_RES * 20) {
            continue;
        }

        //
        // Allow teleport onto corridors or rooms.
        //
        if (level->map.is_on_corridor(at) ||
            level->map.is_on_floor(at) ||
            (level->is_hell && level->map.is_on_dirt(at)) ||
            (level->is_cave && level->map.is_on_dirt(at)) ||
            (level->is_forest && level->map.is_on_dirt(at)) ||
            (level->is_landscape && level->map.is_on_dirt(at)) ||
            //
            // If self teleport, don't teleport into the dirt areas.
            //
            (level->map.is_on_dirt(at) && !(victim == this))) {

            GH_THING_LOG(this, "teleport attack " + victim->name);

            if (level->map.move_to(victim, at)) {

                if (victim->touching_anything()) {
                    level->map.move_to(victim, old);
                    continue;
                }

                if (victim->is_hero) {
                    if (this == victim) {
                        //
                        // Slight shake for self teleport
                        //
                        gh_global::screen_shake += 100.0;

                        //
                        // Give some protection to the teleportee
                        //
                        victim->shielded_boost_start(3); // 1.5 secs
                    } else{
                        //
                        // Disorientate the victim
                        //
                        gh_global::screen_shake += 1000.0;

                        //
                        // No protection for an attack!
                        //
                    }

                    //
                    // Teleporting thyself? No warning.
                    //
                    if (victim != this) {
                        level->game->new_message(the_named() + " teleports you!");

                        play("Teleport");
                    }
                }

                return (true);
            }
        }
    }

    return (false);
}

//
// The thing is teleporting itself.
//
bool gh_thing::self_teleport (void)
{
    GH_FUNCTION()

    if (!is_hero) {
        if (!teleport_chance) {
            return (false);
        }
    }

    //
    // Don't allow the thing to teleport too often.
    //
    if (!gh_have_x_tenths_passed_since(teleport_delay_in_tenths,
                                       teleport_last_when)) {
        return (false);
    }

    //
    // Add a random delta so the next teleports don't all happen together.
    //
    teleport_last_when = gh_get_time_milli() + gh_math::rand1000();

    GH_THING_LOG(this, "teleported itself");

    bool ok = (teleport_attack(this));

    //
    // If we managed to self telport, turn the flag off for this level.
    //
    if (ok) {
        if (can_self_teleport_once_per_level) {
            can_self_teleport = false;
        }

        //
        // Give some protection to the teleportee
        //
        shielded_boost_start(6); // 3 secs

        //
        // Teleports are a bit tiring.
        //
        for (int tick = 0; tick < 40; tick++) {
            if (!tireless) {
                stamina_tick();
            }
        }

        if (is_hero) {
            play("Teleport");
        }
    }

    return (ok);
}

//
// Something has walked into a teleport.
//
bool gh_thing::teleport (gh_thing *a_teleport)
{
    GH_FUNCTION()

    if (is_teleport) {
        //
        // Teleports teleporting teleports?!
        //
        return (false);
    }

    if (a_teleport->sleeping()) {
        //
        // If asleep we do nothing.
        //
        return (false);
    }

    if (weapon_wielder) {
        //
        // Weapons will move with their owners.
        //
        return (false);
    }

    //
    // Can we teleport away?
    //
    gh_thing *destination_teleport;

    destination_teleport =
        level->map.find_furthest_teleport_awake_thing(a_teleport->at);

    if (destination_teleport == NULL) {
        self_teleport();
        return (false);
    }

    GH_THING_LOG(this, "is being teleported by " + a_teleport->named());

    if (level->map.move_to(this, destination_teleport->at)) {
        a_teleport->sleeping_boost_start(5);
    }

    //
    // Allow missiles to move through teleports.
    //
    if (is_missile) {
        missile_at.x = at.x;
        missile_at.y = at.y;
        missile_target = gh_point3d(-1,-1,-1);
    }

    //
    // Make the teleport 'explode' - an effect.
    //
    destination_teleport->try_to_throw_something(
                                NULL, /* target */
                                destination_teleport, /* owner */
                                destination_teleport->explosion_type,
                                false, /* want_thrown_list */
                                true /* always_throw */
                                );

    //
    // Put it to sleep for a while.
    //
    destination_teleport->sleeping_boost_start(5);

    //
    // Give some protection to the teleportee
    //
    shielded_boost_start(6); // 3 secs

    if (is_hero) {
        play("Teleport");

        level->game->new_message("The world spins around you briefly");
    }

    GH_THING_LOG(this, "teleport");

    return (true);
}
