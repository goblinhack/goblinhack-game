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

    $Id: collect.cpp 186 2006-10-08 03:31:22Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_player.h>

//
// Find the best weapon to use
//
void gh_thing::wield_next_best (void)
{
    GH_FUNCTION()

    string name;

    if (weapon) {
        name = weapon->name;
    } else {
        name = "";
    }

    GH_THING_LOG(this, "looking for next best weapon");

    //
    // Try to find a weapon of the same name first.
    //
    gh_thing *best = NULL;

    list<gh_thing *>::iterator i  = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *a_weapon;

        a_weapon = *i;

        if (a_weapon->name == name) {
            if (best) {
                if (a_weapon->charges > best->charges) {
                    best = a_weapon;
                }
            } else {
                best = a_weapon;
            }
        }

        i++;
    }

    //
    // Found something with some charges left?
    //
    if (best && best->charges) {
        wield_new_weapon(best);
    }
}

//
// Get rid of a weapon
//
void gh_thing::unwield (void)
{
    GH_FUNCTION()

    if (!weapon) {
        //
        // No weapon to unwield?
        //
        return;
    }

    gh_thing *w = weapon;
    weapon = NULL;

    const gh_thing *weapon_template;

    GH_THING_LOG(this, "is unwielding the " + w->named());
    GH_THING_LOG(w, "  this");

    //
    // Restore the old weapon.
    //
    weapon_template = w->template_thing;
    if (!weapon_template) {
        GH_THROW("can't unwield " + w->named());
    }

    //
    // Restore the graphics for this weapon
    //
    w->anim_frames = weapon_template->anim_frames;

    //
    // Return the animation back to the static representation
    // of the weapon on the level.
    //
    w->direction = gh_point3d(0,0,0);
    w->direction_wanted = gh_point3d(0,0,0);
    w->direction_last = gh_point3d(0,0,0);
    w->weapon_spin = 0;

    w->update_anim();

    //
    // Check we are still carrying it.
    //
    if (w->isref(GH_THING_REF_IM_BEING_CARRIED)) {
        GH_THING_LOG(this, " still carrying the " + w->named());
        GH_THING_LOG(w, "  this");

        if (w->is_dead) {
            //
            // Drop the useless weapon.
            //
            GH_THING_LOG(this, " broken; drop the " + w->named());

            if (is_hero) {
                level->game->new_message(
                     "%%fg=red" + w->the_named() + " is broken");
            }

            drop_item(w, false /* force */);

            if (weapon) {
                GH_THROW("unwielding and dropping and got another weapon");
            }
        } else {
            //
            // Pop it back in our bag.
            //
            if (w->is_on_map) {
                GH_THING_LOG(this, " putting the " + w->named() + " in bag");
                level->map.pop(w);
            }
        }
    }

    //
    // Decrememnt the weapon's lock on us
    //
    if (w->weapon_wielder) {
        decref(GH_THING_REF_WEAPON_WIELDER);
        w->weapon_wielder = NULL;
    }

    //
    // Decrememnt our lock on the weapon.
    //
    w->decref(GH_THING_REF_WEAPON);
}

//
// We're wielding a weapon, and want to reset it after loading.
//
void gh_thing::rewield (void)
{
    GH_FUNCTION()

    if (!weapon) {
        //
        // No weapon to unwield?
        //
        return;
    }

//    GH_THING_LOG(this, "rewielding " + weapon->name);

    //
    // When we're being wielded we need to look like an animation.
    // Switch to it now.
    //
    const gh_thing *wield_as;

    //
    // Change the graphics to the wielded one
    //
    wield_as = gh_thing::find(weapon->wield_as);
    if (!wield_as) {
        GH_THROW("can't wield new weapon " + weapon->named() + " as " +
                 weapon->wield_as);
    }

    //
    // Restore the graphics for this weapon
    //
    weapon->anim_frames = wield_as->anim_frames;
}

//
// Swap to a new weapon
//
void gh_thing::wield_new_weapon (gh_thing *new_weapon)
{
    GH_FUNCTION()

    //
    // Stop dragons wielding staffs!
    //
    if (!can_wield_weapons) {
        return;
    }

    GH_THING_LOG(this, "is switching to a " + new_weapon->named());
    GH_THING_LOG(new_weapon, "  this");

    //
    // Too thick to use the weapon? At load time, intelligence is 0 until the 
    // backpack is loaded.
    //
    if (intelligence && (new_weapon->intelligence_req > intelligence)) {
        if (is_hero) {
            level->game->new_message(
                 "%%fg=yellowYou're don't know how to use " + 
                 new_weapon->the_named());
        }

        GH_THING_LOG(this, "is not intelligent enough to use " + 
                     new_weapon->named());

        return;
    }

    //
    // Too weak to use the new_weapon? At load time, strength is 0 until the 
    // backpack is loaded.
    //
    if (strength() && (new_weapon->strength_req > strength())) {
        if (is_hero) {
            level->game->new_message(
                 "%%fg=yellowYou're too feeble to use " + 
                 new_weapon->the_named());
        }

        GH_THING_LOG(this, "is not strong enough to use " + 
                     new_weapon->named());

        return;
    }

    //
    // Too skillless to use the new_weapon? At load time, dex() is 0 until the 
    // backpack is loaded.
    //
    if (dex() && (new_weapon->dex_req > dex())) {
        if (is_hero) {
            level->game->new_message(
                 "%%fg=yellowYou're not skillfull enough to use " + 
                 new_weapon->the_named());
        }

        GH_THING_LOG(this, "is not skillfull enough to use " + 
                     new_weapon->named());

        return;
    }

    unwield();

    //
    // Give the hero a lock on the weapon.
    //
    weapon = new_weapon;
    weapon->incref(GH_THING_REF_WEAPON);

    //
    // And give the weapon a lock on us.
    //
    incref(GH_THING_REF_WEAPON_WIELDER);
    weapon->weapon_wielder = this;

    //
    // When we're being wielded we need to look like an animation.
    // Switch to it now.
    //
    const gh_thing *wield_as;

    //
    // Change the graphics to the wielded one
    //
    wield_as = gh_thing::find(weapon->wield_as);
    if (!wield_as) {
        GH_THROW("can't wield new weapon " + weapon->named() + " as " +
                 weapon->wield_as);

    }

    GH_THING_LOG(this, "wielding " + weapon->named());
    GH_THING_LOG(weapon, "  this");

    weapon->anim_frames = wield_as->anim_frames;

    //
    // Push the weapon on the map where the player is.
    //
    level->map.push(weapon);
    level->map.move_to(weapon, at);
}

//
// Use the currently wielded weapon.
//
void gh_thing::wield_existing_weapon (gh_thing *victim)
{
    GH_FUNCTION()

    if (is_dead) {
        return;
    }

    if (!weapon) {
        return;
    }

    if (victim) {
        GH_THING_VERIFY(victim);
    }

    GH_THING_VERIFY(weapon);

    if (gh_global::debug) {
        GH_THING_LOG(this, "wields weapon " + weapon->named());
        GH_THING_LOG(weapon, " this ");
    }

    //
    // Make the sword spin through all 8 compass directions in a swipe.
    //
    if ((weapon->is_staff || weapon->is_sword || weapon->is_axe || weapon->is_warhammer) &&
        !weapon->weapon_spin) {
        if (gh_global::debug) {
            GH_THING_LOG(this, " swings");
            GH_THING_LOG(weapon, "  this ");
        }

        weapon->weapon_spin += 8;
    }

    if (weapon->is_wand) {
        if (gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                          bite_last_when)) {
            if (gh_global::debug) {
                GH_THING_LOG(this, " zaps");
                GH_THING_LOG(weapon, "  this ");
            }

            //
            // Each use of a charged thing brings it closer to uselessness.
            //
            if (weapon->charges) {
                weapon->charges--;
            }

            try_to_throw_something(victim,
                                   this, /* owner */
                                   "", /* throw_this */
                                   false, /* want_thrown_list */
                                   false /* always_throw */);
        }

        if (!weapon) {
            return;
        }
    }

    if (weapon->is_bow) {
        if (gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                          bite_last_when)) {
            if (gh_global::debug) {
                GH_THING_LOG(this, " shoots");
                GH_THING_LOG(weapon, "  this ");
            }

            //
            // Each use of a bow thing brings it closer to uselessness.
            //
            if (weapon->charges) {
                weapon->charges--;
            }

            try_to_throw_something(victim,
                                   this, /* owner */
                                   "", /* throw_this */
                                   false, /* want_thrown_list */
                                   false /* always_throw */);
        }

        if (!weapon) {
            return;
        }
    }

    if (weapon->is_gun) {
        if (gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                          bite_last_when)) {
            if (gh_global::debug) {
                GH_THING_LOG(this, "fires");
                GH_THING_LOG(weapon, "  this ");
            }

            //
            // Each use of a charged thing brings it closer to uselessness.
            //
            if (weapon->charges) {
                weapon->charges--;
            }

            try_to_throw_something(victim,
                                   this, /* owner */
                                   "", /* throw_this */
                                   false, /* want_thrown_list */
                                   false /* always_throw */);
        }

        if (!weapon) {
            return;
        }
    }

    //
    // Is is a weapon that has charges that decrement?
    //
    if (weapon->is_chargeable) {
        //
        // All used up?
        //
        if (!weapon->charges) {
            //
            // If this is our current weapon, we're done with it.
            // Try to get a new weapon.
            //
            wield_next_best();
            if (!weapon) {
                return;
            }

            if (!weapon) {
                if (is_hero) {
                    level->game->new_message(
                             weapon->the_named() + " is all used up.");
                }
            }

            destroy_item(weapon);
            if (!weapon) {
                return;
            }
        }
    }
}

//
// Swap to this weapon and start using it if not already doing so.
//
void gh_thing::use_weapon (gh_thing *victim,
                           gh_thing *a_weapon,
                           bool switch_to_but_dont_activate)
{
    GH_FUNCTION()

    if (!a_weapon) {
        a_weapon = weapon;
        if (!a_weapon) {
            return;
        }
    }

    GH_THING_LOG(this, "uses weapon " + a_weapon->named());

    //
    // Are we using this weapon already and just want to use it again?
    //
    if (a_weapon == weapon) {
        if (!switch_to_but_dont_activate) {
            wield_existing_weapon(victim);
        }

        return;
    }

    //
    // Switch to the new wepaon.
    //
    wield_new_weapon(a_weapon);

    if (!switch_to_but_dont_activate) {
        //
        // And use it.
        //
        wield_existing_weapon(victim);
    }
}
