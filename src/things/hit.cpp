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

    $Id: hit.cpp 1300 2009-08-24 22:46:03Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_player.h>
#include <gh_math.h>

//
// One thing is hitting another thing, t
//
void gh_thing::hit (gh_thing *target,
                    bool *killed,
                    int attack_bite_damage,
                    int attack_bite_armour,
                    int attack_bite_treasure,
                    int attack_bite_karma,
                    int attack_bite_karma_chance,
                    int attack_bite_intelligence,
                    int attack_bite_intelligence_chance,
                    int attack_bite_poison,
                    int attack_bite_poison_chance,
                    int attack_bite_stone,
                    int attack_bite_stone_chance,
                    int attack_bite_fate,
                    int attack_bite_fate_chance,
                    gh_thing *weapon,
                    bool mand_attack)
{                       
    GH_FUNCTION()

    GH_THING_VERIFY(this);
    GH_THING_VERIFY(target);

    //
    // Walls don't hit.
    //
    // is_rock we need for lavarock.
    //
    if (is_wall || is_corridor || is_dirt || is_dead) {
        return;
    }

    GH_THING_LOG(this, "hit " + target->the_named());

    if (weapon && 
        weapon->bite_action.size() && 
        (weapon->bite_action[0] == "polymorph")) {
        //
        // If this is a poly attack then allow it to hit the user so they
        // can change themselves.
        //
        if (is_wall) {
            //
            // But let it bounce off of walls.
            //
            return;
        }

        mand_attack = true;
    }

    if (!mand_attack) {
        //
        // Immune to poison?
        //
        if (target->bite_poison && attack_bite_poison) {
            GH_THING_LOG(target, " is immune to poison");
            return;
        }

        //
        // Immune to stoning?
        //
        if (target->bite_stone && attack_bite_stone) {
            GH_THING_LOG(target, " is immune to stoning");
            return;
        }

        //
        // Don't hit ourselves unless it is a poison attack.
        //
        if (want_to_attack(target) <= GH_THING_ANGER_LIMIT) {

            GH_THING_LOG(target, " ignore");
            return;
        }
    }

    //
    // Make sure poison (above) runs inside even if shielded.
    //
    if (target != this) {
        if (target->shielded()) {
            GH_THING_LOG(target, " is shielded");
            return;
        }
    }

    //
    // If this is a missile, don't let it damage the weapon firing it!
    //
    if (missile_thrower) {
        if (target == missile_thrower->weapon) {
            GH_THING_LOG(target, " is own weapon");
            return;
        }
    }

    if (target == weapon) {
        GH_THING_LOG(target, " is own weapon");
        return;
    }

    if (target == weapon_wielder) {
        GH_THING_LOG(target, " is self");
        return;
    }

    if (!target->is_destroyable) {
        GH_THING_LOG(target, " is not destroyable");
        return;
    }

    //
    // Don't allow shopkeepers to detroy their own stock. They do this
    // if monsters enter the shop. Still, allow heroes to do this!
    //
    if (is_shopkeeper) {
        if (target->is_unpaid_for) {
            GH_THING_LOG(target, " is unpaid for");
            return;
        }
    }

    //
    // Let us see that the thing is trying to hit.
    //
    if (is_monst) {
        flash(GH_DARKRED);
    }

    //
    // If hitting a wand say and it's not being wielded, then allow it
    // to be damaged more easily. If being wielded, then make it harder
    // so the wand persists in battle for longer. i.e. if on the ground
    // it is more easily damaged.
    //
    if (target->is_treasure) {
        //
        // Treasure and not being wielded is on the ground.
        //
        if (!target->weapon_wielder) {
            attack_bite_damage *= 10;
        }
    }

    //
    // Shooting in a shop? Bad move.
    //
    if (target->is_unpaid_for || target->is_closed) {
        destroyed_item_in_shop(target);
    }

    //
    // If a monster has thrown a missile at a target, only hit if that
    // target was the original. This protects heros from getting in the
    // way of shopkeepers missiles or from possessed monsters protective
    // shots.
    //
    if (is_missile) {
        if (missile_target != gh_point3d(-1,-1,-1)) {
            if (GH_DISTANCE(missile_target.x, missile_target.y,
                            target->at.x, target->at.y) > GH_RES) {
                GH_THING_LOG(target, " not missile target");
                return;
            }
        }
    }

    //
    // It's really the owner doing the hitting, not the missile.
    //
    if (missile_thrower) {
        missile_thrower->hit(target, killed,
                             attack_bite_damage,
                             attack_bite_armour,
                             attack_bite_treasure,
                             attack_bite_karma,
                             attack_bite_karma_chance,
                             attack_bite_intelligence,
                             attack_bite_intelligence_chance,
                             attack_bite_poison,
                             attack_bite_poison_chance,
                             attack_bite_stone,
                             attack_bite_stone_chance,
                             attack_bite_fate,
                             attack_bite_fate_chance,
                             this /* weapon */,
                             false /* mand attack */);

        if (!health) {
            //
            // This missile has reached the end of its damage.
            //
            if (killed != NULL) {
                *killed = true;
            }

            dead("Objective achieved", true /* explode */);
            return;
        }

        //
        // Other missiles like poison gas, hit repeatedly.
        //
        health--;

        GH_THING_LOG(target, " missile can hit this again");
        return;
    }

    //
    // It's really the owner doing the hitting, not the weapon.
    //
    if (weapon_wielder) {
        weapon_wielder->hit(target, killed,
                            attack_bite_damage,
                            attack_bite_armour,
                            attack_bite_treasure,
                            attack_bite_karma,
                            attack_bite_karma_chance,
                            attack_bite_intelligence,
                            attack_bite_intelligence_chance,
                            attack_bite_poison,
                            attack_bite_poison_chance,
                            attack_bite_stone,
                            attack_bite_stone_chance,
                            attack_bite_fate,
                            attack_bite_fate_chance,
                            this /* weapon */,
                            false /* mand attack */);

        GH_THING_LOG(this, " weapon can hit again");
        return;
    }

    //
    // Roar!
    //
    if (gh_math::rand100() > 80) {
        if (is_hero || target->is_hero) {
            play(target->audio_on_hit);
        }
    }

    //
    // Can it teleport attack too?
    //
    if (is_teleport_attack) {
        if (!target->teleport_proof()) {
            GH_THING_LOG(this, "teleport attack on " + target->named());

            teleport_attack(target);
        }
    }

    //
    // See if the attack on target results in double damage, or target lapping
    // the attack up.
    //
    bool double_damage = false;
    bool quad_damage = false;
    bool negative_damage = false;

    if (weapon) {
        if (weapon->is_cursed) {
            negative_damage = true;
        }

        if (weapon->is_fire) {
            if (target->wearer_hates_fire()) {
                double_damage = true;
            }

            if (target->loves_fire) {
                negative_damage = true;
            }
        }

        if (weapon->is_acid) {
            if (target->wearer_hates_acid()) {
                double_damage = true;
            }

            if (target->loves_acid) {
                negative_damage = true;
            }
        }

        if (weapon->is_water) {
            if (target->wearer_hates_water()) {
                double_damage = true;
            }

            if (target->loves_water) {
                negative_damage = true;
            }
        }

        if (weapon->is_death) {
            if (target->loves_death) {
                negative_damage = true;
            }
        }

        if (weapon->is_cold) {
            if (target->wearer_hates_cold()) {
                double_damage = true;
            }

            if (target->loves_cold) {
                negative_damage = true;
            }
        }

        if (weapon->is_suffocation) {
            double_damage = true;
        }

        if (weapon->karma > 100) {
            if (target->is_undead || (target->karma < -100)) {
                quad_damage = true;
            }
        }

        if (weapon->is_silver) {
            if (target->is_undead) {
                quad_damage = true;
            }
        }

        if (weapon->karma < -100) {
            if (target->is_holy || (target->karma > 100)) {
                quad_damage = true;
            }
        }

        if (double_damage) {
            attack_bite_damage *= 2;
            attack_bite_armour *= 2;
        }

        if (quad_damage) {
            attack_bite_damage *= 4;
            attack_bite_armour *= 4;
        }

        if (negative_damage) {
            attack_bite_damage = -attack_bite_damage;
            attack_bite_armour = -attack_bite_armour;
        }
    }

    if (raging()) {
        attack_bite_damage *= 2;
        attack_bite_armour *= 2;
    }

    //
    // How often to we try and hit and totally miss?
    //
    if (gh_math::rand100() > bite_chance) {
        if (is_hero) {
            if (target->is_monst) {
                level->game->new_message("You miss " + target->the_named());
            }
        } else if (target->is_hero) {
            if (!is_water) {
                if (weapon) {
                    level->game->new_message(the_named() + "'s " +
                                             weapon->named() + 
                                             " almost hits you",
                                             false /* unique */);
                } else {
                    level->game->new_message(the_named() +
                                             " almost gets you",
                                             false /* unique */);
                }

                if (is_missile) {
                    play("Woosh_Build");
                } else {
                    play("Metal_Hit_004");
                }
            }
        }

        //
        // Make the thing dislike the attacker. If it's a monster. No point
        // in get angry at lava!
        //
        if (is_monst || is_hero) {
            anger_all(target);
        }

        if (killed != NULL) {
            *killed = false;
        }

        target->flash(GH_RED);

        GH_THING_LOG(target, " missed me");
        return;
    }

    if (attack_bite_treasure) {
        //
        // If this blow can steal treasure, do it!
        //
        if (is_hero) {
            //
            // Hero stealing from a monster
            //
            if (target->is_monst) {
                //
                // Make the thing dislike our hero if it notices stealing
                // going on.
                //
                if (gh_math::rand100() < 10) {
                    if (!target->treasure) {
                        level->game->new_message(
                         target->the_named() +
                         "is outraged at its empty money bag!",
                         true /* unique */);
                    } else {
                        level->game->new_message(
                         target->the_named() +
                         "notices your greedy paw in its pocket!",
                         true /* unique */);
                    }

                    if (killed != NULL) {
                        *killed = false;
                    }

                    GH_THING_LOG(this, " attack noticed again");
                    return;
                }

                if (!target->treasure) {
                    level->game->new_message(
                         target->the_named() + " is penniless!",
                         true /* unique */);

                } else {
                    level->game->new_message("You feel richer!",
                                             true /* unique */);

                    play("cash_register2");
                }
            }
        } else {
            if (gh_math::rand100() < 10) {
                if (target->is_hero) {
                    if (treasure) {
                        level->game->new_message(
                            "Hmm, your money bag seems lighter..",
                            true /* unique */);

                        play("coin_roll", 5);
                    }
                }
            }
        }

        if (target->treasure) {
            target->treasure -= attack_bite_treasure;
            treasure += attack_bite_treasure;

            if (target->treasure < 0) {
                target->treasure = 0;
            }
        }
    }

    //
    // Check for exclusions first.
    //
    if (is_water || (weapon && weapon->is_water)) {
        if (target->water_proof()) {
            GH_THING_LOG(target, " undamaged");
            return;
        }
    }

    if (is_fire || (weapon && weapon->is_fire)) {
        if (target->fire_proof()) {
            if (is_hero) {
            } else if (target->is_hero) {
            }
            level->game->new_message(
                target->the_named() + " bathes in the flames.");

            GH_THING_LOG(target, " undamaged");
            return;
        }
    }

    if (is_acid || (weapon && weapon->is_acid)) {
        if (target->acid_proof()) {
            level->game->new_message(
                target->the_named() + " bathes in the acid.");

            GH_THING_LOG(target, " undamaged");
            return;
        }
    }

    if (is_cold || (weapon && weapon->is_cold)) {
        if (target->cold_proof()) {
            level->game->new_message(
                target->the_named() + " basks in the cold.");

            GH_THING_LOG(target, " undamaged");
            return;
        }
    }

    if (is_suffocation || (weapon && weapon->is_suffocation)) {
        if (target->suffocation_proof()) {
            level->game->new_message(
                target->the_named() + " basks in the lack of air.");

            GH_THING_LOG(target, " undamaged");
            return;
        }
    }

    //
    // If the thing has armour hit that first
    //
    if (weapon && weapon->is_spell && !attack_bite_armour) {
        //
        // Unless it is a spell. They bypass armour.
        //
    } else {
        //
        // So do we have armour?
        //
        if (target->how_much_armour_am_i_wearing()) {
            //
            // For things that don't explicitly hit the armour for a certain
            // amount of damage, see if we need to redirect the blow to the
            // armour.
            //
            if (!attack_bite_armour) {
                //
                // The thing has armour, so redirect the damage onto that.
                // However, armour blunts the blow somewhat.
                //
                attack_bite_armour = attack_bite_damage / 2;

                if (!attack_bite_armour) {
                    attack_bite_armour = 1;
                }

                attack_bite_damage = 0;

                GH_THING_LOG(this, " redirect blow to armour");
            }

            //
            // This blow can damage the armour.
            //
            if (is_hero) {
                //
                // Hero hitting a monster
                //
                if (target->is_monst) {
                    if (double_damage || quad_damage) {
                        if (is_water || (weapon && weapon->is_water)) {
                            level->game->new_message(
                                target->the_named() + "'s armour rusts and cracks");
                        } else if (is_fire || (weapon && weapon->is_fire)) {
                            level->game->new_message(
                                target->the_named() + "'s armour melts and cracks");
                        } else if (is_acid || (weapon && weapon->is_acid)) {
                            level->game->new_message(
                                target->the_named() + "'s armour dissolves and cracks");
                        } else if (is_cold || (weapon && weapon->is_cold)) {
                            level->game->new_message(
                                target->the_named() + "'s armour frosts and cracks");
                        } else if (is_suffocation || (weapon && weapon->is_suffocation)) {
                            level->game->new_message(
                                target->the_named() + "'s chokes!");
                        } else {
                            level->game->new_message(
                                target->the_named() + " reels from the blow");
                        }
                    } else if (negative_damage) {
                        level->game->new_message(
                            target->the_named() + "'s armour seems strengthened ");
                    } else {
                        level->game->new_message(
                            "Your blow damages " + target->the_named() + "'s armour");
                    }
                } else {
                    //
                    // Hero hitting something else; not a monster.
                    //
                    if (double_damage || quad_damage) {
                        level->game->new_message(
                            "Your blow severly damages " + target->the_named());
                    } else if (negative_damage) {
                        level->game->new_message(
                            target->the_named() + " glows");
                    } else {
                        level->game->new_message(
                            "Your blow damages " + target->the_named());
                    }
                }

                //
                // Flash the screen.
                //
                gh_global::flash_color.b += 1.0;

                target->flash(GH_RED);
            } else {
                //
                // Monster hitting a hero?
                //
                if (target->is_hero) {
                    if (double_damage || quad_damage) {
                        if (is_water || (weapon && weapon->is_water)) {
                            level->game->new_message(
                                 the_named() + " stings");
                        } else if (is_fire || (weapon && weapon->is_fire)) {
                            level->game->new_message(
                                 the_named() + " burns you");
                        } else if (is_acid || (weapon && weapon->is_acid)) {
                            level->game->new_message(
                                 the_named() + " sizzles on you");
                        } else if (is_cold || (weapon && weapon->is_cold)) {
                            level->game->new_message(
                                 the_named() + " freezes your bones");
                        } else if (is_suffocation || (weapon && weapon->is_suffocation)) {
                            level->game->new_message(
                                 the_named() + " chokes you!");
                        }

                        //
                        // Flash the screen.
                        //
                        gh_global::flash_color.r += 5.0;

                        target->flash(GH_DARKRED);
                    } else if (negative_damage) {
                        level->game->new_message("Your armour glows in the attack!");

                        //
                        // Flash the screen.
                        //
                        target->flash(GH_GOLD);
                    } else {
                        if (is_water || (weapon && weapon->is_water)) {
                            level->game->new_message(
                                 the_named() + " rusts your armour");

                            play("gurgle_x");

                            //
                            // Flash the screen brown.
                            //
                            gh_global::flash_color.r += 1.0;
                            gh_global::flash_color.g += 0.25;
                            gh_global::flash_color.b += 0.25;

                            target->flash(GH_BROWN);
                        } else if (is_fire || (weapon && weapon->is_fire)) {
                            level->game->new_message(
                                 the_named() + " burns your armour");

                            //
                            // Flash the screen.
                            //
                            gh_global::flash_color.r += 1.0;
                            gh_global::flash_color.g += 0.0;
                            gh_global::flash_color.b += 0.0;

                            target->flash(GH_DARKRED);
                        } else if (is_acid || (weapon && weapon->is_acid)) {
                            level->game->new_message(
                                 the_named() + " dissolves your armour");

                            //
                            // Flash the screen.
                            //
                            gh_global::flash_color.r += 0.0;
                            gh_global::flash_color.g += 1.0;
                            gh_global::flash_color.b += 0.0;

                            target->flash(GH_GREEN);
                        } else if (is_cold || (weapon && weapon->is_cold)) {
                            level->game->new_message(
                                 the_named() + " freezes your armour");

                            //
                            // Flash the screen.
                            //
                            gh_global::flash_color.r += 0.0;
                            gh_global::flash_color.g += 0.0;
                            gh_global::flash_color.b += 1.0;

                            target->flash(GH_BLUE);
                        } else if (is_suffocation || (weapon && weapon->is_suffocation)) {
                            level->game->new_message(
                                 the_named() + " chokes you!");

                            //
                            // Flash the screen.
                            //
                            gh_global::flash_color.r += 0.0;
                            gh_global::flash_color.g += 0.0;
                            gh_global::flash_color.b += 1.0;

                            target->flash(GH_BLUE);
                        } else {
                            level->game->new_message(
                                 the_named() + " hits your armour");

                            //
                            // Flash the screen.
                            //
                            gh_global::flash_color.r += 1.0;

                            target->flash(GH_RED);
                        }
                    }
                }
            }

            //
            // Hit the things armour.
            //
            target->hit_armour(attack_bite_armour);

            int noise = gh_math::rand100();

            if (is_hero || target->is_hero) {
                if (noise > 75) {
                    play("Metal_Hit_004");
                } else if (noise > 50) {
                    play("Metal_Hit_005");
                } else if (noise > 25) {
                    play("Metal_Hit_006");
                } else {
                    play("Metal_Hit_007");
                }
            }

            //
            // Make the thing dislike the attacker. If it's a monster. No point
            // in get angry at lava!
            //
            if (is_monst || is_hero) {
                anger_all(target);
            }

            GH_THING_LOG(this, " hit armour");

            if (killed != NULL) {
                *killed = false;
            }
            return;
        }

        //
        // The thing has no armour. No point in hitting that.
        //
        attack_bite_armour = 0;
    }

    if (attack_bite_damage && (target->health <= attack_bite_damage)) {
        //
        // Blow a fate point to escape death?
        //
        if (target->fate > 0) {
            target->fate--;

            //
            // Give some protection to the victim
            //
            target->shielded_boost_start(6); // 3 secs

            target->health = target->orig_health / 2;
            target->orig_health /= 2;

            if (target->is_hero) {
                //
                // Flash the screen to warn the user.
                //
                gh_global::flash_color.r += 20.0;
                gh_global::flash_color.g += 0.0;
                gh_global::flash_color.b += 0.0;

                level->game->new_message(
                     "%%fg1=red%%bg2=white%%fg2=white%%bg2=red"
                     "You use a fate point to escape death from " +
                     the_named(),
                     true /* unique */);

                gh_global::screen_shake += 40.0;

                play("Red_Alert_FX_001", 20);

            } else if (target->is_monst) {
                if (is_hero) {
                    level->game->new_message(
                         target->the_named() +
                         " uses a fate point to escape death from " +
                         the_named(),
                         false /* unique */);

                    if (target->can_insult) {
                        target->insult(this);
                    }

                    play("Multi_Effected_004");
                }
            }
        
            target->flash(GH_DARKRED);

            GH_THING_LOG(target, " was hit");
            return;
        }

        if (killed != NULL) {
            *killed = true;
        }

        if (is_hero) {
            if (target->is_monst && !target->is_body_part) {
                if (target->is_shopkeeper) {
                    //
                    // Bad show, shooting a shopkeeper.
                    //
                    is_thief = true;

                    level->game->new_message(
                         "%%fg=redYou killed the honest, " +
                         target->named() + "%%fg=green",
                         false /* unique */);
                } else {
                    //
                    // Good show, shooting a monster.
                    //
                    level->game->new_message(
                         "%%fg=redYou killed " + target->the_named() +
                         "%%fg=green",
                         false /* unique */);
                }
            }

            //
            // Flash the screen.
            //
            gh_global::flash_color.b += 1.0;

            target->flash(GH_RED);

            //
            // Hitting a nice thing? Bad show old chap..
            //
            if (target->karma > 0) {
                karma -= target->karma + 10;
            }

            //
            // Hitting something evil? Not so bad.
            //
            // Note the difference. Killing good things is hard to undo.
            //
            if (target->karma < 0) {
                if (is_thief) {
                    //
                    // Even harder for thieves.
                    //
                    karma += -(target->karma / 50);
                } else {
                    karma += -(target->karma / 10);
                }
            }

            //
            // Any bonuses for killing this thing?
            //
            score += target->score;

            bravery_add_bonus(target);
        } else if (target->is_hero) {
            //
            // Flash the screen to warn the user.
            //
            gh_global::flash_color.r += 1.0;
            gh_global::flash_color.g += 1.0;
            gh_global::flash_color.b += 1.0;

            target->flash(GH_DARKRED);

            level->game->new_message(the_named() + " killed you");
        }

        target->health = 0;
        target->dead(named(), true /* explode */);

        GH_THING_LOG(target, " was killed");
        return;
    }

    if (attack_bite_karma && (gh_math::rand100() < attack_bite_karma_chance)) {
        if (target->is_hero) {
            if (attack_bite_karma > 0) {
                if (is_evil()) {
                    level->game->new_message(
                         "You bite " + target->the_named() +
                         "! You bask in the hate!",
                         false /* unique */);
                } else if (is_nice()) {
                    level->game->new_message(
                         "You bite " + target->the_named() +
                         "! You sob for your soul!",
                         false /* unique */);
                } else {
                    level->game->new_message(
                         "You bite " + target->the_named() +
                         "! You feel darkly evil!",
                         false /* unique */);
                }

                //
                // Flash the screen to warn the user.
                //
                gh_global::flash_color.r += 1.0;

                target->flash(GH_RED);
            } else {
                if (is_evil()) {
                    level->game->new_message(
                         the_named() + " bites! You feel less hateful!",
                         false /* unique */);
                } else if (is_nice()) {
                    level->game->new_message(
                         the_named() + " bites! You glow contentedly!",
                         false /* unique */);
                } else {
                    level->game->new_message(
                         the_named() + " bites! You feel somehow happier!",
                         false /* unique */);
                }
            }
        } else if (is_hero) {
            if (target->is_monst) {

                if (attack_bite_karma > 0) {
                    if (target->is_evil()) {
                        level->game->new_message(
                             "You bite " + target->the_named() +
                             "! It basks in your hate!",
                             false /* unique */);
                    } else if (target->is_nice()) {
                        level->game->new_message(
                             "You bite " + target->the_named() +
                             "! It weeps for your soul!",
                             false /* unique */);
                    } else {
                        level->game->new_message(
                             "You bite " + target->the_named() +
                             "! It grins evilly!",
                             false /* unique */);
                    }

                    //
                    // Flash the screen.
                    //
                    gh_global::flash_color.b += 1.0;

                    target->flash(GH_RED);

                } else {
                    if (target->is_evil()) {
                        level->game->new_message(
                             "You bite " + target->the_named() +
                             "! It seems less hateful!",
                             false /* unique */);
                    } else if (target->is_nice()) {
                        level->game->new_message(
                             "You bite " + target->the_named() +
                             "! It glows happily!",
                             false /* unique */);
                    } else {
                        level->game->new_message(
                             "You bite " + target->the_named() +
                             "! It seems somehow happier now!",
                             false /* unique */);
                    }
                }
            }
        }

        //
        // Make the thing dislike the attacker
        //
        anger_all(target);

        GH_THING_LOG(this,
                     "sullies " + target->named() + " [HP " +
                     tostring(target->health) + "/KR " +
                     tostring(target->karma) + "] for " +
                     tostring(attack_bite_karma));

        target->karma -= attack_bite_karma;

        if (killed != NULL) {
            *killed = false;
        }

        return;
    }

    if (attack_bite_intelligence && (gh_math::rand100() < attack_bite_intelligence_chance)) {
        if (target->is_hero) {
            if (attack_bite_intelligence > 0) {
                level->game->new_message(
                     the_named() + " plays with your mind.",
                     false /* unique */);
                //
                // Flash the screen to warn the user.
                //
                gh_global::flash_color.r += 1.0;

                target->flash(GH_RED);
            } else {
                level->game->new_message(
                     the_named() + " plays with your mind. "
                     "You feel smarter!",
                     false /* unique */);
            }
        } else if (is_hero) {
            if (target->is_monst) {

                if (attack_bite_intelligence > 0) {
                    level->game->new_message(
                         "You hit " + target->the_named() +
                         "! It seems confused.",
                         false /* unique */);
                    //
                    // Flash the screen.
                    //
                    gh_global::flash_color.b += 1.0;

                    target->flash(GH_RED);

                } else {
                    level->game->new_message(
                         "You hit " + target->the_named() +
                         "! It looks smarter...",
                         false /* unique */);
                }
            }
        }

        //
        // Make the thing dislike the attacker
        //
        anger_all(target);

        GH_THING_LOG(this,
                     "brain delves " + target->named() + " [HP " +
                     tostring(target->health) + "/KR " +
                     tostring(target->intelligence) + "] for " +
                     tostring(attack_bite_intelligence));

        target->intelligence -= attack_bite_intelligence;

        //
        // Check we can still use our weapons.
        //
        if (target->weapon &&
            (target->intelligence < target->weapon->intelligence_req)) {
            level->game->new_message("%%fg=red"
                                     "You drop your weapon"
                                     "%%fg=green",
                                     false /* unique */);
            target->unwield();
            target->wield_next_best();
        }

        //
        // Too stupid to breath?
        //
        if (target->intelligence <= 0) {
            if (killed != NULL) {
                *killed = true;
            }

            level->game->new_message("%%fg=red"
                                     "You forget how to breath"
                                     "%%fg=green",
                                     false /* unique */);

            target->dead("Stupidity", false /* explode */);

            GH_THING_LOG(target, " is killed");
            return;
        }

        if (killed != NULL) {
            *killed = false;
        }

        return;
    }

    //
    // Poison a monster. But not a wall.
    //
    if (attack_bite_poison && ((target->is_hero) || (target->is_monst))) {

        if (target != this) {
            //
            // Immune to poison?
            //
            if (target->poison_proof()) {
                if (target->is_hero) {
                    level->game->new_message("%%fg=yellow"
                                             "You take no poison damage"
                                             "%%fg=green",
                                             false /* unique */);
                } else if (is_hero) {
                    level->game->new_message("%%fg=yellow" +
                                             target->the_named() +
                                             " takes no poison damage"
                                             "%%fg=green",
                                             false /* unique */);
                }

                GH_THING_LOG(target, " no poison damage");
                return;
            }

            //
            // A new poison attack by some other creature.
            //
            if (gh_math::rand100() < attack_bite_poison_chance) {
                target->poisoned_boost_start(attack_bite_poison);

                GH_THING_LOG(this, "has poisoned " + target->named());
            }

            //
            // Flash the screen to warn the user.
            //
            if (target->is_hero) {
                gh_global::flash_color.r += 20.0;
                gh_global::flash_color.g += 20.0;
                gh_global::flash_color.b += 0.0;
            }
        } else if (!target->raging()) {
            GH_THING_LOG(this, "is poisoned for " + tostring(attack_bite_poison));

            //
            // We've been poisoned already. This is just it running
            // through our system.
            //
            if (target->is_hero) {
                level->game->new_message("%%fg=yellow"
                                         "You feel really sick..."
                                         "%%fg=green",
                                         false /* unique */);

                //
                // Disorientate the victim
                //
                gh_global::screen_shake += 5.0;

                //
                // Flash the screen yellow to warn the user.
                //
                gh_global::flash_color.r += (float)attack_bite_poison;
                gh_global::flash_color.g += (float)attack_bite_poison;
                gh_global::flash_color.b += 0.0;
            }

            target->flash(GH_YELLOW);

            target->health -= attack_bite_poison;

            if (killed != NULL) {
                *killed = false;
            }

            if (target->health > 0) {
                GH_THING_LOG(target, " poisoned");
                return;
            }

            //
            // Blow a fate point to escape death?
            //
            if (target->fate > 0) {
                target->fate--;

                target->health = target->orig_health / 2;
                target->orig_health /= 2;

                if (target->is_hero) {
                    //
                    // Flash the screen to warn the user.
                    //
                    level->game->new_message(
                         "%%fg1=yellow%%bg2=white%%fg2=white%%bg2=red"
                         "You use a fate point to escape poisoning",
                         true /* unique */);

                    play("Red_Alert_FX_001", 30);

                } else if (target->is_monst) {
                    if (is_hero) {
                        level->game->new_message(
                             target->the_named() +
                             " uses a fate point to escape poisoning",
                             false /* unique */);
                    }
                }

                target->flash(GH_YELLOW);

                GH_THING_LOG(target, " poisoned, used fate");
                return;
            }

            if (killed != NULL) {
                *killed = true;
            }

            target->health = 0;
            target->dead("Poisoned", false /* explode */);

            GH_THING_LOG(target, " poisoned, is dead");
            return;
        }
    }

    //
    // Stone a monster. But not a wall.
    //
    if (attack_bite_stone && ((target->is_hero) || (target->is_monst))) {

        if (target != this) {
            //
            // Immune to stone?
            //
            if (target->stone_proof()) {
                if (target->is_hero) {
                    level->game->new_message("%%fg=yellow"
                                             "Avoids petrification"
                                             "%%fg=green",
                                             false /* unique */);
                } else if (is_hero) {
                    level->game->new_message("%%fg=yellow" +
                                             target->the_named() +
                                             " takes no petrification damage"
                                             "%%fg=green",
                                             false /* unique */);
                }

                GH_THING_LOG(target, " no stone damage");
                return;
            }

            //
            // A new stone attack by some other creature.
            //
            if (gh_math::rand100() < attack_bite_stone_chance) {
                target->stoned_boost_start(attack_bite_stone);

                GH_THING_LOG(this, "has stoned " + target->named());
            }

            //
            // Flash the screen to warn the user.
            //
            if (target->is_hero) {
                gh_global::flash_color.r += 20.0;
                gh_global::flash_color.g += 20.0;
                gh_global::flash_color.b += 0.0;
            }
        } else if (!target->raging()) {
            GH_THING_LOG(this, "is stoned for " + tostring(attack_bite_stone));

            //
            // We've been stoned already. This is just it running
            // through our system.
            //
            if (target->is_hero) {
                level->game->new_message("%%fg=yellow"
                                         "You feel suddenly sluggish..."
                                         "%%fg=green",
                                         false /* unique */);

                //
                // Disorientate the victim
                //
                gh_global::screen_shake += 5.0;

                //
                // Flash the screen yellow to warn the user.
                //
                gh_global::flash_color.r += (float)attack_bite_stone;
                gh_global::flash_color.g += (float)attack_bite_stone;
                gh_global::flash_color.b += 0.0;
            }

            target->flash(GH_YELLOW);

            target->health -= attack_bite_stone;

            if (killed != NULL) {
                *killed = false;
            }

            if (target->health > 0) {
                GH_THING_LOG(target, " stoned");
                return;
            }

            //
            // Blow a fate point to escape death?
            //
            if (target->fate > 0) {
                target->fate--;

                target->health = target->orig_health / 2;
                target->orig_health /= 2;

                if (target->is_hero) {
                    //
                    // Flash the screen to warn the user.
                    //
                    level->game->new_message(
                         "%%fg1=yellow%%bg2=white%%fg2=white%%bg2=red"
                         "You use a fate point to escape stoning",
                         true /* unique */);

                    play("Red_Alert_FX_001", 30);

                } else if (target->is_monst) {
                    if (is_hero) {
                        level->game->new_message(
                             target->the_named() +
                             " uses a fate point to escape stoneing",
                             false /* unique */);
                    }
                }

                target->flash(GH_YELLOW);

                GH_THING_LOG(target, " stoned, used fate");
                return;
            }

            if (killed != NULL) {
                *killed = true;
            }

            target->health = 0;
            target->dead("Turned to stone", false /* explode */);

            GH_THING_LOG(target, " stoned, is dead");
            return;
        }
    }

    if (attack_bite_fate && target->fate) {

        GH_THING_LOG(this, "is losing a fate point");

        //
        // We've been poisoned already. This is just it running
        // through our system.
        //
        if (target->is_hero) {
            level->game->new_message("%%fg=yellow"
                                     "You feel a loss of fate points"
                                     "...%%fg=green",
                                     true /* unique */);
        }

        if (target->fate >= attack_bite_fate) {
            target->fate -= attack_bite_fate;

            if (target->is_hero) {
                //
                // Flash the screen to warn the user.
                //
                gh_global::flash_color.r += 20.0;
                gh_global::flash_color.g += 20.0;
                gh_global::flash_color.b += 0.0;

                target->flash(GH_RED);

                level->game->new_message(
                     "%%fg1=red%%bg2=white%%fg2=white%%bg2=red"
                     "You feel the loss of a fate point",
                     true /* unique */);

                play("Red_Alert_FX_001", 30);
            }

            GH_THING_LOG(target, " hit, used fate");
            return;
        }
    }

    if (target->is_hero) {
        if (is_water || (weapon && weapon->is_water)) {
            level->game->new_message("You gulp down water. Choke!",
                                     true /* unique */);

            play("gurgle_x");

            //
            // Flash the screen
            //
            gh_global::flash_color.r += 0.3;
            gh_global::flash_color.g += 0.6;
            gh_global::flash_color.b += 1.0;

            target->flash(GH_BLUE);
        } else {
            //
            // Hitting youself? Polymorph perhaps?
            //
            if (is_hero) {
                if (attack_bite_damage) {
                    level->game->new_message(
                         "You hit yourself for " + tostring(attack_bite_damage),
                         true /* unique */);
                } else {
                    level->game->new_message(
                         "You hit yourself ", true /* unique */);
                }
            } else {
                //
                // Is a monster attacking a hero.
                //
                // Don't allow fire resistant heros to get stronger if
                // bitten by lava.
                //
                if (attack_bite_damage < 0) {
                    attack_bite_damage = 0;
                }

                if (attack_bite_damage) {
                    level->game->new_message(
                         the_named() + " hits you for " + 
                         tostring(attack_bite_damage),
                         false /* unique */);
                }

                if (attack_bite_poison < 0) {
                    attack_bite_poison = 0;
                }

                if (attack_bite_poison) {
                    level->game->new_message(
                         the_named() + " poisons you for " + 
                         tostring(attack_bite_poison),
                         false /* unique */);
                }

                if (attack_bite_stone < 0) {
                    attack_bite_stone = 0;
                }

                if (attack_bite_stone) {
                    level->game->new_message(
                         the_named() + " is turning you to stone! ",
                         false /* unique */);
                }

                if (attack_bite_karma < 0) {
                    attack_bite_karma = 0;
                }

                if (attack_bite_karma) {
                    level->game->new_message(
                         the_named() + " drains your karma for " + 
                         tostring(attack_bite_karma),
                         false /* unique */);
                }
                
                if (attack_bite_intelligence < 0) {
                    attack_bite_intelligence = 0;
                }

                if (attack_bite_intelligence) {
                    level->game->new_message(
                         the_named() + " drains your intelligence for " + 
                         tostring(attack_bite_intelligence),
                         false /* unique */);
                }

                if (attack_bite_armour < 0) {
                    attack_bite_armour = 0;
                }

                if (attack_bite_armour) {
                    level->game->new_message(
                         the_named() + " damages your armour for " + 
                         tostring(attack_bite_armour),
                         false /* unique */);
                }

                if (attack_bite_fate < 0) {
                    attack_bite_fate = 0;
                }

                if (attack_bite_fate) {
                    level->game->new_message(
                         the_named() + " drains your life!",
                         false /* unique */);
                }

                if (!attack_bite_damage &&
                    !attack_bite_armour &&
                    !attack_bite_treasure &&
                    !attack_bite_karma &&
                    !attack_bite_intelligence &&
                    !attack_bite_poison &&
                    !attack_bite_stone &&
                    !attack_bite_fate) {
                    level->game->new_message(
                         the_named() + " tries to hit you", true /* unique */);
                }

                if (is_hero || target->is_hero) {
                    if (attack_bite_damage >= 90) {
                        play("scream_male");
                    } else {
                        if (is_meat_eater) {
                            play("croc_chomp_x");
                        } else {
                            play("Kick_005");
                        }
                    }
                }

                //
                // Flash the screen
                //
                gh_global::flash_color.r += 1.0;
            }

            target->flash(GH_RED);
        }
    } else if (is_hero) {
        if (target->is_monst) {
            //
            // Flash the screen.
            //
            if (attack_bite_armour && target->armour) {
                level->game->new_message(
                     "You hit " + target->the_named() + "'s armour for " +
                     tostring(attack_bite_armour),
                     false /* unique */);
            } else if (attack_bite_damage) {
                level->game->new_message(
                     "You hit " + target->the_named() + " for " +
                     tostring(attack_bite_damage),
                     false /* unique */);
            }

            int noise = gh_math::rand100();

            if (noise > 70) {
                play("Metal_Hit_001");
            } else if (noise > 30) {
                play("Metal_Hit_002");
            } else {
                play("Metal_Hit_003");
            }

            gh_global::flash_color.b += 1.0;

            target->flash(GH_RED);

            //
            // Make the thing dislike the attacker
            //
            anger_all(target);
        }
    }

    //
    // Too many logs for exploding potions.
    //
    if (!is_missile) {
        GH_THING_LOG(this,
                     "hits " + target->named() + " [HP " +
                     tostring(target->health) + "/AC " +
                     tostring(target->armour) + "] for " + tostring(attack_bite_damage));
    }

    //
    // Allow heroes to fire regularly.
    //
    if (!is_hero) {
        //
        // This does make monster firing a bit odd as the impacts delay
        // firing. I call this a feature.
        //
        bite_last_when = gh_get_time_milli();
    }

    target->health -= attack_bite_damage;

    //
    // Make sure negative damage can't make you invincible!
    //
    if (attack_bite_damage < 0) {
        if (target->health > target->orig_health) {
            target->health = target->orig_health;
        }
    }

    if (killed != NULL) {
        *killed = false;
    }

    //
    // If wielding a sword and hitting a wall, dent it.
    //
    if (weapon &&
        (weapon->is_sword || weapon->is_staff || weapon->is_axe ||
         weapon->is_warhammer)) {
        bool damage_weapon = false;

        damage_weapon =  (target->is_hard && !weapon->is_hard);
        damage_weapon |= (target->is_fire && weapon->hates_fire);
        damage_weapon |= (target->is_cold && weapon->hates_cold);
        damage_weapon |= (target->is_suffocation && weapon->hates_suffocation);
        damage_weapon |= (target->is_acid && weapon->hates_acid);

        if (damage_weapon) {
            //
            // Every x hits, damage the weapon?
            //
            if (gh_math::rand100() < 4) {
                //
                // Damage the weapon
                //
                uint16 damage = (gh_math::rand100() / 5) + 1;

                if (weapon->health <= damage) {
                    //
                    // Weapon is destroyed.
                    //
                    weapon->health = 0;
                } else {
                    //
                    // Weapon is damaged.
                    //
                    weapon->health -= damage;
                }

                if (weapon->health <= 0) {
                    if (weapon->is_sword) {
                        play("sword_dead");
                    } else if (weapon->is_staff) {
                        play("sword_dead");
                    } else if (weapon->is_bow) {
                        play("sword_dead");
                    } else if (weapon->is_axe) {
                        play("sword_dead");
                    } else if (weapon->is_warhammer) {
                        play("sword_dead");
                    } else if (weapon->is_wand) {
                        play("wand_dead");
                    } else if (weapon->is_gun) {
                        play("gun_dead");
                    }

                    GH_THING_LOG(this, "destroys their " + weapon->named());

                    if (is_hero) {
                        level->game->new_message("%%fg=yellow"
                                                 "You destroy your " +
                                                 weapon->named() +
                                                 "! 8(%%fg=green");
                    }

                    weapon->dead("Broken " + weapon->named(),
                                 true /* explode */);
                    weapon = NULL;
                } else {
                    if (is_hero) {
                        level->game->new_message("You damage your " +
                                                 weapon->named());
                    }

                    GH_THING_LOG(this, "damages their " + weapon->named());

                    weapon->flash(GH_RED);
                }
            }
        }
    }

    int other_attack_chance;

    if (!attack_bite_damage &&
        !attack_bite_armour &&
        !attack_bite_treasure &&
        !attack_bite_karma &&
        !attack_bite_karma_chance &&
        !attack_bite_intelligence &&
        !attack_bite_intelligence_chance &&
        !attack_bite_poison &&
        !attack_bite_poison_chance &&
        !attack_bite_stone &&
        !attack_bite_stone_chance &&
        !attack_bite_fate &&
        !attack_bite_fate_chance) {
        other_attack_chance = 100;
    } else {
        other_attack_chance = 50;
    }

    if (gh_math::rand100() < other_attack_chance) {
        //
        // Care to do something else bad to the target?
        //
        if (weapon) {
            //
            // Does this weapon have a special attack?
            //
            if (weapon->bite_action.size()) {
                weapon->bite_attack(target);
            }
        } else {
            //
            // Does this hero have a special attack?
            //
            if (bite_action.size()) {
                bite_attack(target);
            }
        }
    }
}
