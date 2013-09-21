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

    $Id: tick.cpp 117 2006-09-17 22:32:03Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_game.h>

//
// Our thing has died! 8( Maybe it will be resurrected!? 8)
//
void gh_thing::dead (string killed_by, bool explode, bool drop_things)
{
    GH_FUNCTION()

    GH_THING_VERIFY(this);

    //
    // Protect against reentrancy.
    //
    if (is_dead) {
        return;
    }

    //
    // Protection from re-entry like when body parts instigate death
    // of the head! whilst we're destroying from the head to the tail.
    //
    is_dead = true;

    //
    // If our divine hero hath hit the bucket, shake the screen in outrage.
    //
    if (is_hero) {
        if (!level->game->want_to_exit_level_editor_mode) {
            gh_global::screen_shake = 100.0;
        }
    }

    //
    // Smaller explosion for other things.
    //
    if (shake_screen_on_death) {
        //
        // But only if close to the hero. And scale it up based on the
        // distance.
        //
        if (hero_distance() < 5) {
            gh_global::screen_shake = 100.0;
        }

        if (hero_distance() < 5) {
            gh_global::screen_shake = 100.0;
        }
    }

    //
    // If killing a body part, kill the whole body instead.
    //
    if (limb_prev || limb_next || prev || next) {
        dead_whole_body(killed_by);
    }

    if (next) {
        next->prev = NULL;
        next = NULL;
    }

    if (prev) {
        prev->next = NULL;
        prev = NULL;
    }

    if (limb_next) {
        limb_next->limb_prev = NULL;
        limb_next = NULL;
    }

    if (limb_prev) {
        limb_prev->limb_next = NULL;
        limb_prev = NULL;
    }

    //
    // tmp ref so we don't vanish whilst demolishing the thing
    //
    incref(GH_THING_REF_TERMINAL);

    health = 0;

    string dead_string;

    if (!is_missile && !is_boring) {
        if (killed_by != "") {
            dead_string = "KILLED: " + killed_by;
        } else {
            dead_string = "KILLED";
        }
    } else {
        dead_string = "";
    }

    //
    // Do this at this level in the code to keep the debug indent in scope.
    //
    GH_THING_LOG(this, dead_string);

    //
    // Check to see if we want to play audio. Unless dying from a real
    // event (not spawning) then be quiet.
    //
    if (drop_things) {
        if (is_monst || is_hard || is_boring) {
            if (audio_on_dead != "") {
                play(audio_on_dead);
            }
        } else {
            play(audio_on_dead);
        }
    }

    //
    // For the likes of electro doors, destroy them all once one is hit
    //
    if (destroy_joined && !level->being_deleted) {
        list <gh_thing *> other = level->map.find_all_touching_things(this);
        list <gh_thing *>::iterator o;

        for (o = other.begin(); o != other.end(); o++) {
            gh_thing *other = *o;

            if (!other->is_dead) {
                other->dead(killed_by, true /* explode */);
            }
        }
    }

    if (this->killed_by == "") {
        if (is_monst || is_hero) {
            GH_THING_LOG(this, "dead, reason: " + killed_by);
        }

        this->killed_by = killed_by;
    }

    //
    // Get rid of our weapon.
    //
    if (weapon) {
        GH_THING_LOG(this, "is dead and unwielding " + weapon->the_named());

        unwield();
    }

    //
    // Or if we're a weapon, tell our owner.
    //
    if (is_weapon) {
        if (weapon_wielder) {
            GH_THING_LOG(this, "weapon is broken, telling " +
                         weapon_wielder->named() + " killed by " + killed_by);

            weapon_wielder->unwield();
        }
    }

    GH_THING_VERIFY(this);

    //
    // And pull the corpse off of the level.
    //
    if (is_on_map) {
        //
        // Does the thing explode upon death?
        //
        if (explode && (explosion_type != "")) {
            //
            // If an explosion from a bazooka, than pass the blame back
            // to the firer and not the bazooka!
            //
            if (!is_missile && !is_monst && !is_hero) {
                GH_THING_LOG(this, "exploded");

                if (hero_distance() > 15) {
                    level->game->new_message("You hear a distant rumble");
                } else {
                    level->game->new_message(the_named() + " explodes!");
                }
            }

            if (missile_thrower) {
                try_to_throw_something(NULL, /* target */
                                       missile_thrower, /* owner */
                                       explosion_type, /* throw_this */
                                       false, /* want_thrown_list */
                                       true /* always_throw */
                                       );
            } else {
                try_to_throw_something(NULL, /* target */
                                       this, /* owner */
                                       explosion_type, /* throw_this */
                                       false, /* want_thrown_list */
                                       true /* always_throw */
                                       );
            }
        }
    }

    if (is_body_part) {
        //
        // Leave body parts on the level once dead!
        //
        if (!is_on_map) {
            //
            // We expect this to be on the map else we have no
            // ref when we leave here.
            //
            if (level->being_deleted) {
                //
                // Expected.
                //
            } else {
                //
                // Can be a body we failed to place at level creation.
                //
                level->add_dead(this);
            }
        }
    } else if (leaves_corpse) {
        //
        // Corpses litter the ground...
        //
        if (!is_on_map) {
            //
            // We expect this to be on the map else we have no
            // ref when we leave here.
            //
            if (level->being_deleted) {
                //
                // Expected.
                //
            } else {
                //
                // Happens in the level editor if carrying a monster.
                // Let it pass.
                //
                GH_WARN("corpse not on map");
            }
        }

        //
        // Make sure corpses can be stepped over.
        //
        render_depth = 1;
    } else {
        //
        // Add to the dead list which gets cleaned up each level tick.
        //
        level->add_dead(this);

        if (is_on_map) {
            level->map.pop(this);
        }
    }

    GH_THING_VERIFY(this);

    //
    // Now we've unwielded weapons we can destroy what else we carry.
    //
    if (drop_things) {
        drop_all_items();
    } else {
        destroy_all_items();
    }

    GH_THING_VERIFY(this);

    //
    // Just in case we couldn't place a destroyed carried item.
    //
    if (isref(GH_THING_REF_IM_BEING_CARRIED)) {
        //
        // Make sure the dropped item being destroyed is uncarried.
        //
        if (carrier) {
            carrier->drop_item(this, true /* force */);
        }

        decref(GH_THING_REF_IM_BEING_CARRIED);
    }

    //
    // In case this is still set
    //
    if (isref(GH_THING_REF_FIRST)) {
        decref(GH_THING_REF_FIRST);

        GH_THING_VERIFY(this);
    }

    decref(GH_THING_REF_TERMINAL);
}
