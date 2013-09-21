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

    $Id: throwing.cpp 1030 2008-07-26 16:10:48Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

list <gh_thing *> gh_thing::try_to_throw_something (gh_thing *victim,
                                                    gh_thing *owner,
                                                    string throw_this,
                                                    bool want_thrown_list,
                                                    bool always_throw)
{
    GH_FUNCTION()

    list <gh_thing *> threw;

    if (!always_throw) {
        //
        // Don't allow the thing to throw too often.
        //
        if (!gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                           bite_last_when)) {
            return (threw);
        }
    }

    if (is_monst) {
        //
        // Make the monster firing staggered else it can hurt the
        // frame rate if they all fire at the same time.
        //
        bite_last_when = gh_get_time_milli() + gh_math::rand100();
    } else {
        //
        // But not the hero. Fire as quick as you can.
        //
        bite_last_when = gh_get_time_milli();
    }

    //
    // If nothing is given, look to the weapon.
    //
    if (throw_this == "") {
        if (weapon) {
            //
            // Throw whatever the weapon item throws.
            //
            if (weapon->throws.size()) {
                throw_this = weapon->throws[0];
            }
        }
    }

    if (throw_this == "") {
        //
        // Default to what the thing naturally throws.
        //
        if (throws.size()) {
            size_t index = rand() % throws.size();

            throw_this = throws[index];
        }
    }

    //
    // Nothing to throw?
    //
    if (throw_this == "") {
        return (threw);
    }

    //
    // If no victim and we're not pointing any particular direction,
    // just choose one.
    //
    if (!victim) {
        if (!direction_last.x && !direction_last.y) {
            direction_last.x = 1;
            direction_last.y = 0;
        }
    }

    gh_fpoint delta;

    if (victim) {
        delta = gh_fpoint(victim->at.x - at.x, victim->at.y - at.y);
    } else {
        delta = gh_fpoint(direction_last.x * 2, direction_last.y * 2);
    }

    gh_thing *new_missile;

    //
    // Spawn a new missile and add it to the thrown list if wanted.
    //
    new_missile = new gh_thing(level, throw_this, at);
    if (want_thrown_list) {
        threw.push_back(new_missile);
    }

    new_missile->friend_id = id;

    play(new_missile->audio_on_use);

    //
    // The modifier we use to alternate the angle to left/right
    // so we only need to count the angles in one delta and they're
    // always split evenly - even if firing two/three
    //
    float modifier = 1;
    float angle = delta.anglerot();

    //
    // How many we throw
    //
    const int missile_spread = new_missile->missile_spread;
    const float missile_spread_angle = new_missile->missile_spread_angle;

    //
    // If a laser, send a long searching beam!
    //
    int max_range;

    //
    // If not a laser then fake a short range to get one missile out.
    //
    if (!new_missile->laser_range) {
        max_range = 1;
    } else {
        max_range = new_missile->laser_range;
    }

    if (new_missile->casts_lightning) {
        for (int spread = 0; spread <= missile_spread; spread++) {
            angle +=
                spread * modifier * gh_math::rad(missile_spread_angle);

            cast_lightning(new_missile, throw_this, at, max_range, angle, 0.6);

            new_missile = NULL;

            modifier = modifier * -1;
        }

        return (threw);
    }

    for (int spread = 0; spread <= missile_spread; spread++) {
        for (int range = 0; range < max_range; range++) {
            //
            // What direction/angle will it start of at.
            //
            angle +=
                spread * modifier * gh_math::rad(missile_spread_angle);

            gh_point3d new_at = at;

            //
            // Project this missile start point out to the current range.
            //
            new_at.x = (int)((float)at.x + (float)range * 8.0 * gh_math::fcos(angle));
            new_at.y = (int)((float)at.y + (float)range * 8.0 * gh_math::fsin(angle));
            new_at.z = 0;

            //
            // Off the map? Happens with lasers.
            //
            if (level->map.real_isatp(new_at) == NULL) {
                continue;
            }

            if (!new_missile) {
                //
                // Spawn a new missile and add it to the thrown list if wanted.
                //
                new_missile = new gh_thing(level, throw_this, new_at);

                new_missile->friend_id = id;

                if (want_thrown_list) {
                    threw.push_back(new_missile);
                }

                new_missile->_metabolism +=
                                ((rand() % new_missile->metabolism()) / 10);
                new_missile->_metabolism -=
                                ((rand() % new_missile->metabolism()) / 10);
            }

            new_missile->missile_angle = angle;

            //
            // Increment a reference on ourselves so we can get notified
            // when this missile is destroyed.
            //
            if (!is_dead) {
                //
                // Unless we're dead. We probably don't care.
                //
                if (owner) {
                    //
                    // Might be an explosion.
                    //
                    new_missile->missile_thrower = owner;
                    owner->incref(GH_THING_REF_MISSILE_THROWER);
                } else {
                    new_missile->missile_thrower = this;
                    incref(GH_THING_REF_MISSILE_THROWER);
                }
            }

            new_missile->missile_at =
                            gh_fpoint3d(new_at.x, new_at.y, new_at.z);

            if (victim) {
                new_missile->missile_target = victim->at;
            } else {
                new_missile->missile_target = gh_point3d(-1,-1,-1);
            }

            level->map.push(new_missile);

            new_missile->decref(GH_THING_REF_FIRST);
            GH_THING_VERIFY(new_missile);

            //
            // To force allocation of a new missile.
            //
            new_missile = NULL;

            modifier = modifier * -1;
        }
    }

    return (threw);
}

void gh_thing::cast_lightning (gh_thing *new_missile,
                               string throw_this,
                               gh_point3d fork_start,
                               int fork_range,
                               float angle,
                               float stddev)
{
    GH_FUNCTION()

    //
    // At the end of our lightning bolt?
    //
    if (fork_range <= 0) {
        return;
    }

    //
    // Fork the lightning.
    //
    int forks = (rand() % 2) + 1;

    while (forks-- > 0) {
        gh_point3d fork_end(
                (int)((float)fork_start.x + (float)fork_range * 8.0 * cos(angle)),
                (int)((float)fork_start.y + (float)fork_range * 8.0 * sin(angle)),
                0);

        int fork_len = gh_distance(fork_start, fork_end);

        while (fork_len-- > 0) {
            gh_point3d new_at(
                (int)((float)fork_start.x + (float)fork_len * 8.0 * cos(angle)),
                (int)((float)fork_start.y + (float)fork_len * 8.0 * sin(angle)),
                0);

            //
            // Off the map?
            //
            if (level->map.real_isatp(new_at) == NULL) {
                continue;
            }

            if (!new_missile) {
                //
                // Spawn a new missile and add it to the thrown list if wanted.
                //
                new_missile = new gh_thing(level, throw_this, new_at);

                new_missile->id = id;
            }

            new_missile->missile_angle = angle;

            //
            // Increment a reference on ourselves so we can get notified
            // when this missile is destroyed.
            //
            if (!is_dead) {
                //
                // Unless we're dead. We probably don't care.
                //
                new_missile->missile_thrower = this;
                incref(GH_THING_REF_MISSILE_THROWER);
            }

            new_missile->missile_at = gh_fpoint3d(new_at.x, new_at.y, new_at.z);
                new_missile->missile_target = gh_point3d(-1,-1,-1);

            level->map.push(new_missile);

            new_missile->decref(GH_THING_REF_FIRST);
            GH_THING_VERIFY(new_missile);

            //
            // To force allocation of a new missile.
            //
            new_missile = NULL;
        }

        float new_angle = gh_math::gauss(angle, stddev);
        int new_range = (int)((float)fork_range * gh_math::gauss(0.6, 0.1));

        cast_lightning(NULL, throw_this, fork_end, new_range, new_angle,
                       stddev * 0.5);
    }
}
