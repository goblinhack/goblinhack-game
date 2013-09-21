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

    $Id: tick.cpp 1251 2009-07-28 13:06:28Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_thing_registration.h>
#include <gh_player.h>
#include <gh_math.h>

//
// Reset a thing to initial settings and placement
//
void gh_thing::reset (void)
{
    GH_FUNCTION()

    scared_boost_stop();
}

void gh_thing::common_tick (void)
{
    GH_FUNCTION()

    if (is_dead) {
        return;
    }

    //
    // If being carred, don't move or transmog or spawn
    //
    if (isref(GH_THING_REF_IM_BEING_CARRIED)) {
        return;
    }

    if (is_hero) {
        hero_tick();

        if (is_dead) {
            return;
        }
    } else {
        if (level->is_editing) {
            if (!gh_global::debug) {
                return;
            }
        }

        //
        // Don't let things move
        //
        if (level->freeze_things) {
            return;
        }

        monst_tick();

        if (is_dead) {
            return;
        }
    }

    //
    // If not moving make sure we interact with things under or
    // appearing on us.
    //
    if (direction_wanted == gh_point3d(0,0,0)) {
        if (!speed) {
            meet_all_at_new_position();

            if (is_dead) {
                return;
            }
        }
    }

    //
    // Make our weapon animation spin
    //
    update_anim();

    if (weapon && weapon->weapon_spin) {
        weapon->weapon_spin--;
    }

    //
    // Check and see if we're carrying too much.
    //
    if (carrying.begin() != carrying.end()) {
        is_burdened = (how_much_am_i_carrying() > strength());
    }
}

void gh_thing::tick (void)
{
    GH_FUNCTION()

    //
    // Only the first torso does anything.
    //
    if (limb_prev) {
        return;
    }

    //
    // Dead things leave corpses that are left on the level. I need
    // an idle list to handle this else corpses will fire.
    //
    if (is_dead) {
        return;
    }

    if (is_monst) {
        gh_global::monst_count++;
    }

    gh_thing_registration_args args;

    gh_thing *hero = level->game->player->thing;

    if (hero && is_monst) {
        if (gh_global::really_slow) {
            int d = hero_distance();

            if (d < 7) {
                //
                // tick
                //
                if (gh_math::rand100() > 80) {
                    return;
                }
            } else if (d < 12) {
                if (gh_math::rand100() > 20) {
                    return;
                }
            } else {
                return;
            }
        } else if (gh_global::too_slow) {
            int d = hero_distance();

            if (d < 7) {
                //
                // tick
                //
            } else if (d < 12) {
                if (gh_math::rand100() > 20) {
                    return;
                }
            } else if (d < 20) {
                if (gh_math::rand100() > 5) {
                    return;
                }
            } else {
                return;
            }
        } else {
            //
            // tick
            //
        }
    }

    //
    // Ask the thing if it wants to do stuff. If it fails
    // we assume it's dead.
    //
    if (!reg->tick(&args)) {
        return;
    }

    //
    // Life hath expired?
    //
    if (!sleeping()) {
        lifespan_tick();
        if (is_dead) {
            //
            // Indeed it has.
            //
            return;
        }
    }

    //
    // Every 1/2 second modify any boosts
    //
    if (gh_have_x_tenths_passed_since(5, quick_tick_when)) {
        //
        // Make sure we enter here only occasionally.
        //
        quick_tick_when = gh_get_time_milli();

        stamina_tick();
        if (is_dead) {
            return;
        }

        if (is_burdened) {
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();
            stamina_tick();

            //
            // Just in case. Should never hit this.
            //
            if (how_much_am_i_carrying() > strength() * 2) {
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
                stamina_tick();
            }

            if (is_dead) {
                return;
            }
        }

        shielded_boost_tick();
        if (is_dead) {
            return;
        }

        invisible_boost_tick();
        if (is_dead) {
            return;
        }

        raging_boost_tick();
        if (is_dead) {
            return;
        }
    }

    //
    // Every second modify any boosts
    //
    if (gh_have_x_tenths_passed_since(10, last_boost_tick_when)) {
        if (is_hero) {
                if (level->time_left > 0) {
                    level->time_left--;
                    if (!level->time_left) {
                        level->is_level_completed = true;
                    }
                }
        }

        //
        // Make sure we enter here only occasionally.
        //
        last_boost_tick_when = gh_get_time_milli();

        sleeping_boost_tick();
        if (is_dead) {
            return;
        }

        strength_boost_tick();
        if (is_dead) {
            return;
        }

        scared_boost_tick();
        if (is_dead) {
            return;
        }

        metabolism_boost_tick();
        if (is_dead) {
            return;
        }

        stamina_boost_tick();
        if (is_dead) {
            return;
        }

        poisoned_boost_tick();
        if (is_dead) {
            return;
        }

        stoned_boost_tick();
        if (is_dead) {
            return;
        }
    }

    //
    // Every ten seconds modify any boosts
    //
    if (gh_have_x_tenths_passed_since(100, last_slow_boost_tick_when)) {

        last_slow_boost_tick_when = gh_get_time_milli();

        can_see_distance_boost_tick();
        if (is_dead) {
            return;
        }
    }

    //
    // If weak, and can auto rage, go mad!!!
    //
    if (can_auto_rage() && (health < orig_health / 3)) {
        if (!raging()) {
            raging_boost_start(3); // 1.5 secs
        }
    }

    tick_at += metabolism();

    if (tick_at < tick_max) {
        return;
    }

    tick_at = 0;
    common_tick();
}
