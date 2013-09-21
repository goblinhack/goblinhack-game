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

    $Id: chasing.cpp 576 2007-12-01 22:23:00Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_thing.h>

int gh_thing::want_to_attack (gh_thing *target)
{
    GH_FUNCTION()

    vector<string>::iterator pattern;
    int result = 0;
    size_t index = 0;

    //
    // Attack my own body?
    //
    if (target->id == id) {
        return (0);
    }

    //
    // Heros are vicious.
    //
    if (is_hero) {
        return (100);
    }

    //
    // Attack walls?
    //
    if (is_monst) {
        if (target->is_boring) {
            return (0);
        }

        //
        // Is this a body part that doesn't bite?
        //
        if (is_body_part) {
             if (! bite_damage &&
                 ! bite_armour &&
                 ! bite_treasure &&
                 ! bite_karma &&
                 ! bite_karma_chance &&
                 ! bite_intelligence &&
                 ! bite_intelligence_chance &&
                 ! bite_poison &&
                 ! bite_poison_chance &&
                 ! bite_stone &&
                 ! bite_stone_chance &&
                 ! bite_fate &&
                 ! bite_fate_chance &&
                 (weapon == NULL)) {
                return (0);
             }
        }
    }

    //
    // No chasing corpses in this game!
    //
    if (target->is_dead) {
        return (0);
    }

    //
    // Hit your own weapon? Madness!
    //
    if (target == weapon) {
        return (0);
    }

    //
    // Chase a missile. Not wise.
    //
    if (target->is_missile) {
        return (0);
    }

    //
    // Attack my own possessions?
    //
    if (target->get_owner() == this) {
        return (0);
    }

    //
    // Swords always like to attack!
    //
    if (is_sword || is_staff || is_axe || is_warhammer) {
        //
        // But only if we are swinging.
        //
        if (weapon_spin) {
            return (100);
        }

        return (0);
    }

    if (is_missile) {
        if (target == missile_thrower) {
            return (0);
        }

        return (100);
    }

    //
    // Seems unfair to attack (see dimensional rifts) when invisible
    // e.g. they will throw lava balls when unseen - which is really mean.
    //
    if (is_invisible_anim()) {
        return (0);
    }

    //
    // A cloaked thing?
    //
    if (target->invisible()) {
        return (0);
    }

    //
    // Now, see if this is one of the things we like to attack?
    //
    for (pattern=attacks.begin(); pattern != attacks.end(); pattern++, index++) {
        if (!(*pattern).size()) {
            //
            // Too general. You end up chasing treasure. Rather be specific.
            //
            continue;
        }

        //
        // And what are the chances of us chasing it?
        //
        if (index >= attack_chance.size()) {
            //
            // We never attack it.
            //
            GH_THROW("overflow on attack table for " + name + 
                    " wanted index " + tostring(index) + 
                    " real size is " + tostring(attack_chance.size()));

            gh_err(this);
            break;
        }

        bool attack = target->matches(*pattern);

        if (attack) {
            if (!attack_chance[index]) {
                continue;
            }

            if (attack_chance[index] < 0) {
                result -= (gh_math::rand100() % -attack_chance[index]);
            } else {
                result += (gh_math::rand100() % attack_chance[index]);
            }
        }
    }

    //
    // No centipedes attacking centipedes.
    //
    if (target->name == name) {
        result -= 150;
    }

    //
    // A friend of ours.
    //
    if (target->id && (target->id == friend_id)) {
        result -= 100;
    }

    //
    // In the same possessed group?
    //
    if (target->friend_id && (target->friend_id == friend_id)) {
        result -= 100;
    }

    return (result);
}

bool gh_thing::possible_to_attack (gh_thing *target)
{
    GH_FUNCTION()

    vector<string>::iterator pattern;

    //
    // Attack my own body?
    //
    if (target->id == id) {
        return (false);
    }

    //
    // No chasing corpses in this game!
    //
    if (target->is_dead) {
        return (false);
    }

    //
    // Hit your own weapon? Madness!
    //
    if (target == weapon) {
        return (false);
    }

    //
    // Hit your own user? Madness!
    //
    if (target == weapon_wielder) {
        return (false);
    }

    //
    // Hit your own user? Madness!
    //
    if (target == missile_thrower) {
        return (false);
    }

    //
    // Chase a missile. Not wise.
    //
    if (target->is_missile) {
        return (false);
    }

    //
    // Attack my own possessions?
    //
    if (target->get_owner() == this) {
        return (false);
    }

    //
    // Attack walls?
    //
    if (is_monst) {
        if (target->is_boring) {
            return (false);
        }
    }

    //
    // Monsters always think about chasing heros.
    //
    if (is_monst) {
        if (target->is_hero) {
            return (true);
        }
    }

    //
    // Swords always like to attack!
    //
    if (is_sword || is_staff || is_axe || is_warhammer) {
        //
        // But only if we are swinging.
        //
        if (weapon_spin) {
            return (true);
        }

        return (false);
    }

    //
    // If I'm invisible, it seems a bit unfair to attack
    //
    if (is_invisible_anim()) {
        return (false);
    }

    if (is_missile) {
        return (true);
    }

    //
    // Now, see if this is one of the things we like to attack?
    //
    for (pattern=attacks.begin(); pattern != attacks.end(); pattern++) {
        if (!(*pattern).size()) {
            //
            // To general. You end up chasing treasure. Rather be specific.
            //
            continue;
        }

        if (target->matches(*pattern)) {
            return (true);
        }
    }

    return (false);
}

void gh_thing::try_to_attack_victim (gh_thing *victim)
{
    GH_FUNCTION()

    if (is_monst) {
        if (is_invisible_anim()) {
            //
            // Just not fair to throw when cloaked!
            //
            return;
        }

        if (!gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                           bite_last_when)) {
            return;
        }
    }

    if (weapon) {

        if ((weapon->is_sword || weapon->is_staff || is_axe || is_warhammer)) {
            if (GH_DISTANCE(at.x, at.y, victim->at.x, victim->at.y) >
                GH_RES * 4) {
                //
                // Not close enough to swing.
                //
                return;
            }
        }

        //
        // If using a weapon, throw that. If the weapon runs out,
        // we switch to a backup weapon and failing that, whatever
        // the thing naturally throws.
        //
        GH_THING_LOG(this, "using " + 
                     weapon->the_named() + " on " +
                     victim->the_named());

        if (use_simple_item(victim, weapon)) {
            //
            // Don't allow to swing too often.
            //
            bite_last_when = gh_get_time_milli();
            return;
        }
    } else {
        //
        // Else use what the thing naturally throws.
        //
        try_to_throw_something(victim,
                               this, /* owner */
                               "", /* throw_this */
                               false, /* want_thrown_list */
                               false /* always_throw */);
    }
}

void gh_thing::sleep_attack (gh_thing *target)
{
    GH_FUNCTION()

    //
    // Too cruel to keep on putting you to sleep.
    //
    if (target->sleeping()) {
        return;
    }

    if (target->is_hero || target->is_monst) {
        target->sleeping_boost_start(20);
    }

    if (is_hero && target->is_monst) {
        level->game->new_message(
             "%%fg=yellow" + target->the_named() + 
             " falls asleep%%fg=green");
    } else if (target->is_hero) {
        level->game->new_message("%%fg=redYou feel... snore%%fg=green");
    }
}

void gh_thing::paralysis_attack (gh_thing *target)
{
    GH_FUNCTION()

    if (target->is_hero || target->is_monst) {
        target->sleeping_boost_start(1000);
    }

    if (is_hero && target->is_monst) {
        level->game->new_message(
             "%%fg=yellow" + target->the_named() + 
             " cannot move%%fg=green");
    } else if (target->is_hero) {
        level->game->new_message("%%fg=redYou feel... immobile%%fg=green");
    }
}

void gh_thing::speed_up_attack (gh_thing *target)
{
    GH_FUNCTION()

    if (target->is_hero || target->is_monst) {
        target->metabolism_boost_start(target->metabolism() / 2);
        target->stamina_boost_start(target->stamina() / 2);
    }

    if (is_hero && target->is_monst) {
        level->game->new_message(
             "%%fg=yellow" + target->the_named() + 
             " lunges forward%%fg=green");
    } else if (target->is_hero) {
        level->game->new_message(
         "%%fg=greenYou feel refreshed!%%fg=green");
    }
}

void gh_thing::slow_down_attack (gh_thing *target)
{
    GH_FUNCTION()

    if (target->is_hero || target->is_monst) {
        target->metabolism_boost_start(-target->metabolism() / 2);
        target->stamina_boost_start(-target->stamina() / 2);
    }

    if (is_hero && target->is_monst) {
        level->game->new_message(
             "%%fg=yellow" + target->the_named() + 
             " staggers slowly%%fg=green");
    } else if (target->is_hero) {
        level->game->new_message(
             "%%fg=redYou feel suddenly slug-like...%%fg=green");
    }
}

//
// Use an item in the inventory.
//
void gh_thing::bite_attack (gh_thing *target)
{
    GH_FUNCTION()

    GH_THING_VERIFY(this);
    GH_THING_VERIFY(target);

    //
    // Find something to use. Try for a few times then give up.
    //
    int bite_with_what = -1;
    uint8 u;

    if (!bite_action.size()) {
        return;
    }

    for (u = 0; u < bite_action.size(); u++) {
        bite_with_what = rand() % bite_action.size();

        if (use_chance.size() <= u) {
            break;
        }

        if (gh_math::rand100() < use_chance[bite_with_what]) {
            break;
        }
    }

    if (bite_with_what == -1) {
        bite_with_what = 0;
    }

    //
    // Get the action string and parse it.
    //
    GH_THING_LOG(this, "biting with: " + bite_action[bite_with_what]);

    vector<string> actions = string_split(bite_action[bite_with_what]);

    //
    // Pre-action
    //
    incref(GH_THING_REF_ACTION);

    if (actions[0] == "slow") {
        //
        // Slow it right down.
        //
        slow_down_attack(target);
    } else if (actions[0] == "sleep") {
        //
        // Put it to sleep for a while.
        //
        sleep_attack(target);
    } else if (actions[0] == "polymorph") {
        //
        // Transform it!
        //
        polymorph_attack(target);
    } else if (actions[0] == "paralysis") {
        //
        // Put it to sleep for a long while.
        //
        paralysis_attack(target);
    }

    //
    // Post-action
    //
    decref(GH_THING_REF_ACTION);
}
