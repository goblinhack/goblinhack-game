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
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_player.h>
#include <gh_math.h>
#include <gh_string.h>

//
// One thing is using another thing, item potentially on victim
//
// returns false once the thing is used up
//
bool gh_thing::use_simple_item (gh_thing *target,
                                gh_thing *item,
                                bool switch_to_but_dont_activate)
{
    GH_FUNCTION()

    static int max_keys = 50;
    static int max_fate = 20;
    static int max_stamina = 1000;
    static int max_intelligence = 100;
    static int max_health = 1000;
    static int max_karma = 1000;
    static int min_karma = -1000;

    GH_THING_LOG(this, "uses " + item->named());

    /*
     * target MAY BE NULL IF USING AN ITEM ON YOURSELF.
     */
    if (target) {
        if (target->is_monst) {
            if (target->is_dead) {
                if (is_hero) {
                    level->game->new_message(
                         "%%fg=yellowcorpse does not appreciate being defiled" +
                         item->the_named());
                }
            } else {
                if (is_hero) {
                    level->game->new_message(
                         "%%fg=yellowdoes not appreciate being used" +
                         item->the_named());
                }
            }
            return (false);
        }
    }

    //
    // Too evil or holy to use the item?
    //
    if ((abs(item->karma - karma) > 500) 
        && (((item->karma < 0) && (karma > 0)) ||
            ((item->karma > 0) && (karma < 0)))) {
        if (item->is_chargeable ||
            item->is_weapon ||
            item->is_sword ||
            item->is_staff ||
            item->is_potion ||
            item->is_spell ||
            item->is_wand ||
            item->is_axe ||
            item->is_warhammer ||
            item->is_bow ||
            item->is_gun) {

            if (is_hero) {
                if (is_evil()) {
                    level->game->new_message(
                         "%%fg=yellowIt burns. You cannot use " +
                         item->the_named());
                } else if (is_nice()) {
                    level->game->new_message(
                         "%%fg=yellowIt feels gravely cold. You cannot use " +
                         item->the_named());
                } else {
                    level->game->new_message(
                         "%%fg=yellowIt makes you shiver. You cannot use " +
                         item->the_named());
                }
            }

            GH_THING_LOG(this, "can't use item, karma mismatch " +
                         item->named());

            return (false);
        }
    }

    if (item->intelligence_req > intelligence) {
        if (is_hero) {
            level->game->new_message(
                 "%%fg=yellowYou're not clever enough to use " +
                 item->the_named());
            level->game->new_message(
                 "%%fg=yellowYou need a brainpower of " +
                 tostring((int)item->intelligence_req));
        }

        GH_THING_LOG(this, "is not intelligent enough to use " + item->named());

        return (false);
    }

    //
    // Too weak to use the item?
    //
    if (item->strength_req > strength()) {
        if (is_hero) {
            level->game->new_message(
                 "%%fg=yellowYou're too weak to use " + item->the_named());
            level->game->new_message(
                 "%%fg=yellowYou need a strength of " +
                 tostring((int)item->strength_req) +
                 " lbs");
        }

        GH_THING_LOG(this, "is not strong enough to use " + item->named());

        return (false);
    }

    //
    // Too skillless 8) to use the item?
    //
    if (item->dex_req > dex()) {
        if (is_hero) {
            level->game->new_message(
                 "%%fg=yellowYou're not skillful enough to use " + item->the_named());
            level->game->new_message(
                 "%%fg=yellowYou need a skill of " +
                 tostring((int)item->dex_req));
        }

        GH_THING_LOG(this, "is not skillfull enough to use " + item->named());

        return (false);
    }

    if (item->is_armour) {
        if (!use_armour(item)) {
            GH_THING_LOG(this, "can't use armour " + item->named());
        
            return (false);
        }

        return (true);
    }

    if (item->keys) {
        if (keys >= max_keys) {
            if (is_hero) {
                level->game->new_message(
                         "%%fg=yellowCarrying too many keys!");
            }

            GH_THING_LOG(this, "can't use keys " + item->named());

            return (false);
        }
    }

    if (item->fate) {
        if (fate >= max_fate) {
            if (is_hero) {
                level->game->new_message(
                         "%%fg=yellowToo many fate points!");
            }

            GH_THING_LOG(this, "can't use fate points " + item->named());

            return (false);
        }
    }

    if (item->_stamina) {
        if (stamina() >= max_stamina) {
            if (is_hero) {
                level->game->new_message(
                         "%%fg=yellowYour stamina is at maximum!");
            }

            GH_THING_LOG(this, "can't use stamina points " + item->named());

            return (false);
        }
    }

    if (item->intelligence) {
        if (intelligence >= max_intelligence) {
            if (is_hero) {
                level->game->new_message(
                         "%%fg=yellowYou're too clever!");
            }

            GH_THING_LOG(this, "can't use intelligence points " + item->named());

            return (false);
        }
    }

    if (item->is_edible) {
        if (item->health) {
            if (health >= max_health) {
                if (is_hero) {
                    level->game->new_message(
                             "%%fg=yellowYou're as healthy as you can ever be!");
                }

                GH_THING_LOG(this, "can't use health points " + item->named());

                return (false);
            }

            //
            // Eating a poisonous thing?
            //
            if (item->bite_poison) {
                poisoned_boost_start(item->bite_poison);
            }

            //
            // Eating a stony thing?
            //
            if (item->bite_stone) {
                stoned_boost_start(item->bite_stone);
            }
        }
    }

    //
    // Handle scores
    //
    score += item->score;
    item->score = 0;
    if (item->score) {
        GH_THING_LOG(this, " + score from " + item->named() + " for " +
                     tostring(item->score));
    }

    //
    // Handle treasure
    //
    treasure += item->treasure;
    item->treasure = 0;
    if (item->treasure) {
        GH_THING_LOG(this, " + treasure from " + item->named() + " for " +
                     tostring(item->treasure));
    }

    //
    // Handle strength
    //
    _strength += item->_strength;
    item->_strength = 0;
    if (item->_strength) {
        GH_THING_LOG(this, " + strength from " + item->named() + " for " +
                     tostring(item->_strength));
    }

    //
    // Handle dex
    //
    _dex += item->_dex;
    item->_dex = 0;
    if (item->_dex) {
        GH_THING_LOG(this, " + dex from " + item->named() + " for " +
                     tostring(item->_dex));
    }

    //
    // Handle keys
    //
    keys += item->keys;
    item->keys = 0;
    keys = min(max_keys, (int)keys);
    if (item->keys) {
        GH_THING_LOG(this, " + keys from " + item->named() + " for " +
                     tostring(item->keys));
    }

    //
    // Handle fate
    //
    fate += item->fate;
    item->fate = 0;
    fate = min(max_fate, (int)fate);
    if (item->fate) {
        GH_THING_LOG(this, " + fate from " + item->named() + " for " +
                     tostring(item->fate));
    }

    //
    // Handle stamina
    //
    _stamina += item->_stamina;
    item->_stamina = 0;
    _stamina = min(max_stamina, (int)_stamina);
    if (item->_stamina) {
        GH_THING_LOG(this, " + stamina from " + item->named() + " for " +
                     tostring(item->_stamina));
    }

    //
    // Handle intelligence
    //
    intelligence += item->intelligence;
    item->intelligence = 0;
    intelligence = min(max_intelligence, (int)intelligence);
    if (item->intelligence) {
        GH_THING_LOG(this, " + intelligence from " + item->named() + " for " +
                     tostring(item->intelligence));
    }

    //
    // Handle food
    //
    if (item->is_edible) {
        if (item->health < 0) {
            if (is_hero) {
                level->game->new_message(
                         "That was nice but I feel a bit sick!");
            }
        }

        health += item->health;
        item->health = 0;
        health = min(max_health, (int)health);
        if (item->health) {
            GH_THING_LOG(this, " + health from " + item->named() + " for " +
                         tostring(item->health));
        }

        //
        // Did the thing poison us?
        //
        if (health <= 0) {
            level->game->new_message("Urgh. Really sick!");
            health = 1;
        }

        //
        // Handle karma food
        //
        if (item->karma < 0) {
            if (is_hero) {
                level->game->new_message("I feel more %%fg=redevil%%fg=green after that!");
            }
        } else if (item->karma > 0) {
            if (is_hero) {
                level->game->new_message("I feel %%fg=pinknicer%%fg=green after that!");
            }
        }

        karma += item->karma;
        item->karma = 0;
        karma = min(max_karma, (int)karma);
        karma = max(min_karma, (int)karma);
        if (item->karma) {
            GH_THING_LOG(this, " + karma from " + item->named() + " for " +
                         tostring(item->karma));
        }

        //
        // Did the thing poison us?
        //
        if (health <= 0) {
            if (is_hero) {
                level->game->new_message("Urgh. Really sick!");
            }
            health = 1;
        }
    }

    if (item->is_weapon) {
        //
        // swords, wand etc...
        //
        use_weapon(target, item, switch_to_but_dont_activate);
    } else {
        //
        // keys, armour etc...
        //
        if (switch_to_but_dont_activate) {
            //
            // Don'item decrement charges if just switching to the thing.
            //
        } else {
            //
            // Each use of a charged thing brings it closer to uselessness.
            //
            if (item->is_chargeable) {
                if (item->charges) {
                    item->charges--;
                }
            }
        }

        if (!item->charges) {
            //
            // Used the last charge on a wand?
            //
            item->dead("used", false /* explode */);

            destroy_item(item);
        }
    }

    return (true);
}

//
// Use an item in the inventory.
//
bool gh_thing::use_item (gh_thing *item)
{
    GH_FUNCTION()

    //
    // Find something to use. Try for a few times then give up.
    //
    int use_what = 0;

    uint8 u;

    //
    // Sneakily tryng to use something you don't own?
    //
    if (item->is_unpaid_for && (item->owner != this)) {
        used_unpaid_item_in_shop(item);
    }

    if (!item->use_action.size()) {
        //
        // This is a simple thing used later when shooting
        //
        if (!use_simple_item(NULL /* victim */,
                             item,
                             true /* switch to, don't active */)) {
            return (false);
        }

        return (true);
    }

    play(item->audio_on_use);

    for (u = 0; u < item->use_action.size(); u++) {
        use_what = rand() % item->use_action.size();

        if (use_chance.size() <= u) {
            break;
        }

        if (gh_math::rand100() < item->use_chance[use_what]) {
            break;
        }
    }

    //
    // Found anything? If not we say it's a dud.
    //
    if (u == item->use_action.size()) {

        if (is_hero) {
            level->game->new_message(
                         item->the_named() + " crumbles to dust!");
        } else {
            GH_THING_LOG(this, "crumbles to dust whilst using " + item->named());
        }

        //
        // I'm no longer being carried.
        //
        item->carrier = NULL;
        item->decref(GH_THING_REF_IM_BEING_CARRIED);

        return (true);
    }

    //
    // Get the action string and parse it.
    //
    GH_THING_LOG(this, "using: " + item->use_action[use_what]);
    vector<string> actions = string_split(item->use_action[use_what]);

    //
    // Pre-action
    //
    item->incref(GH_THING_REF_ACTION);

    if (actions[0] == "hit_all_visible") {
        hit_all_i_can_see(item, actions);
    } else if (actions[0] == "fast") {
        spell_fast(item);
    } else if (actions[0] == "throw") {
        spell_throw(item, actions[1]);
    } else if (actions[0] == "warp") {
        spell_teleport_self(item);
    } else if (actions[0] == "uncurse") {
        spell_remove_curse(item);
    } else if (actions[0] == "heal") {
        spell_heal(item);
    } else if (actions[0] == "raise_dead") {
        spell_raise_dead(item);
    } else if (actions[0] == "shield") {
        spell_shield(item);
    } else if (actions[0] == "invisible") {
        spell_invisible(item);
    } else if (actions[0] == "vision") {
        spell_vision(item);
    } else if (actions[0] == "info") {
        spell_info(item);
    } else if (actions[0] == "weightless") {
        spell_weightless(item);
    } else if (actions[0] == "summon_possessed") {
        string summon_what = char_to_summon_name(actions[1]);

        if (!monst_summon_adjacent(summon_what,
                                   !item->is_cursed /* possess */)) {
            if (!item->charges) {
                //
                // If the item had no charges, give it one so it can
                // persist.
                //
                item->charges = 2;
            } else {
                //
                // Allow another go.
                //
                item->charges++;
            }
        }
    } else if (actions[0] == "summon") {
        string summon_what = char_to_summon_name(actions[1]);

        if (!monst_summon_adjacent(summon_what,
                                   !item->is_cursed /* possess */)) {
            if (!item->charges) {
                //
                // If the item had no charges, give it one so it can
                // persist.
                //
                item->charges = 2;
            } else {
                //
                // Allow another go.
                //
                item->charges++;
            }
        }
    } else if (actions[0] == "summon_wall_possessed") {
        string summon_what = char_to_summon_name(actions[1]);

        if (!monst_summon_wall(summon_what, 
                          2 /* radius */,
                          false /* ignore_obstacle */,
                          true /* ignore_monst */,
                          !item->is_cursed /* possess */)) {
            if (!item->charges) {
                //
                // If the item had no charges, give it one so it can
                // persist.
                //
                item->charges = 2;
            } else {
                //
                // Allow another go.
                //
                item->charges++;
            }
        }
    } else if (actions[0] == "summon_wall") {
        string summon_what = char_to_summon_name(actions[1]);

        if (!monst_summon_wall(summon_what, 
                          2 /* radius */,
                          false /* ignore_obstacle */,
                          true /* ignore_monst */,
                          false /* possess */)) {
            if (!item->charges) {
                //
                // If the item had no charges, give it one so it can
                // persist.
                //
                item->charges = 2;
            } else {
                //
                // Allow another go.
                //
                item->charges++;
            }
        }
    } else if (actions[0] == "summon_large_wall_possessed") {
        string summon_what = char_to_summon_name(actions[1]);

        if (!monst_summon_wall(summon_what, 
                          4 /* radius */,
                          false /* ignore_obstacle */,
                          true /* ignore_monst */,
                          true /* possess */)) {
            if (!item->charges) {
                //
                // If the item had no charges, give it one so it can
                // persist.
                //
                item->charges = 2;
            } else {
                //
                // Allow another go.
                //
                item->charges++;
            }
        }
    } else if (actions[0] == "summon_large_wall") {
        string summon_what = char_to_summon_name(actions[1]);

        if (!monst_summon_wall(summon_what, 
                          4 /* radius */,
                          false /* ignore_obstacle */,
                          true /* ignore_monst */,
                          false /* possess */)) {
            if (!item->charges) {
                //
                // If the item had no charges, give it one so it can
                // persist.
                //
                item->charges = 2;
            } else {
                //
                // Allow another go.
                //
                item->charges++;
            }
        }
    }

    //
    // Post-action
    //
    item->decref(GH_THING_REF_ACTION);

    if (is_hero) {
        level->game->new_message("You used " + item->the_named());
    } else {
        GH_THING_LOG(this, "used the " + item->named());
    }

    if (item->charges) {
        item->charges--;
    }

    //
    // Last charge on a spell? Remove it.
    //
    if (!item->charges) {
        destroy_item(item);
    }

    return (true);
}

//
// Drop an item somewhere around the thing
//
bool gh_thing::use_item_name (string name)
{
    GH_FUNCTION()

    gh_thing *best_item = NULL;

    list<gh_thing *>::iterator i  = carrying.begin();

    while (i != carrying.end()) {
        //
        // Search for something the same name as this index.
        //
        if ((*i)->name == name) {
            //
            // Choose things that are unpaid for or uncursed if multiple
            // of the same name exist.
            //
            if (best_item) {
                //
                // Got one already. Is this one preferred?
                //
                if (best_item->is_cursed) {
                    best_item = *i;
                }

                if (best_item->is_unpaid_for) {
                    //
                    // But don't allow cursed items to override unpaid for!
                    //
                    if (!(*i)->is_cursed) {
                        best_item = *i;
                    }
                }
            } else {
                //
                // First time we've seen this name.
                //
                best_item = *i;
            }
        }

        i++;
    }

    if (best_item) {
        return (use_item(best_item));
    }

    GH_WARN("didn't find " + name + " to use");

    return (false);
}
