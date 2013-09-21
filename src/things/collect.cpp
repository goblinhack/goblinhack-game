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

    $Id: collect.cpp 1163 2009-02-07 18:06:44Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_player.h>
#include <gh_room.h>

//
// One thing is collecting another thing, item
//
// false on failure to collect.
//
bool gh_thing::collect (gh_thing *item)
{
    GH_FUNCTION()

    if (item->room && item->room->is_shop && item->is_unpaid_for) {
        //
        // Collecting an item in a shop
        //
        GH_THING_LOG(item, "looks at " + item->the_named());

        return (collect_in_shop(item));
    }

    if (collects_stuff_but_doesnt_use_it) {
        //
        // Some things just like to collect treasure and not use it.
        //
        GH_THING_LOG(item, "hoards " + item->the_named());

        return (collect_item(item));
    }

    if (backpack.size()) {
        //
        // Unloading our backpack. Add everything until it is empty.
        //
        if (item->is_monst) {
            possess_monst(item);
        }

        return (collect_item(item, true /* force */));
    }

    if (item->is_inventory_item) {
        //
        // Add this item to our backpack.
        //
        return (collect_item(item));
    }

    //
    // Use the item immediately.
    //
    if (!use_simple_item(NULL /* target */, item,
                         true /* don't fire weapons on collecting */)) {
        //
        // Try and can carry it for later.
        //
        if (collect_item(item)) {
            return (false);
        }
    }

    if (is_hero) {
        if (item->charges) {
            //
            // Wands
            //
            level->game->new_message("You wave " + item->the_named());
            play("magic_tinkle");
        } else if (item->charges) {
            //
            // Swords
            //
            level->game->new_message("You wield " + item->the_named());
            play("Metal_Hit_007");
        } else if (item->is_edible) {
            //
            // Food
            //
            play("Slime_001");

            if (item->bite_stone) {
                level->game->new_message(item->name + ", tastes like stone!");
            } else if (item->bite_poison) {
                level->game->new_message(item->name + ", tastes odd!");
            } else {
                level->game->new_message(item->name + ", yum!");
            }
        } else if (item->is_treasure) {
            //
            // Treasure
            //
            if (item->is_potion || item->is_spell) {
                //
                // No sound yet.
                //
            } else {
                play("treasure");
            }

            level->game->new_message("You pocket " + item->the_named());
        } else {
            //
            // Junk
            //
            level->game->new_message("You pick up " + item->the_named());
            play("Power_On_002");
        }
    }

    return (true);
}

//
// Add an item to the carrying list if possible.
//
bool gh_thing::collect_item (gh_thing *item, bool force)
{
    GH_FUNCTION()

    if (item->isref(GH_THING_REF_IM_BEING_CARRIED)) {
        return (true);
    }

    //
    // Can I lift it?
    //
    if (!force) {
        if (!can_i_carry(item)) {
            //
            // Failed to lift an item.
            //
            if (is_hero) {
                level->game->new_message(
                         "%%fg=yellowYour not strong enough to lift " +
                         item->the_named() + " (" + tostring(item->weight) + "lbs)");
            }

            GH_THING_LOG(this, "can't lift " +
                         item->the_named() +
                         " (" + tostring(item->weight) + "lbs)");

            return (false);
        }

        //
        // Unpaid for items get other logs from the shopkeeper.
        //
        if (!item->is_unpaid_for) {
            if (backpack.size()) {
                //
                // If unloading our backpack, then do it quietly.
                //
            } else if (is_hero) {
                if (item->is_wand) {
                    //
                    // Wands
                    //
                    level->game->new_message("You collect " + item->the_named());
                    play("magic_tinkle");
                } else if (item->is_sword) {
                    //
                    // Swords
                    //
                    level->game->new_message("You collect " + item->the_named());
                    play("Metal_Hit_007");
                } else if (item->is_staff) {
                    //
                    // Staffs
                    //
                    level->game->new_message("You collect " + item->the_named());
                    play("Metal_Hit_007");
                } else if (item->is_axe) {
                    //
                    // Axes
                    //
                    level->game->new_message("You collect " + item->the_named());
                    play("Metal_Hit_007");
                } else if (item->is_warhammer) {
                    //
                    // Warhammers
                    //
                    level->game->new_message("You collect " + item->the_named());
                    play("Metal_Hit_007");
                } else if (item->is_gun) {
                    //
                    // Guns
                    //
                    level->game->new_message("You collect " + item->the_named());
                    play("Power_On_002");
                } else if (item->is_edible) {
                    //
                    // Food
                    //
                    level->game->new_message("You store " +
                                             item->the_named() + " for later");
                    play("Slime_001");
                } else if (item->is_armour) {
                    //
                    // Armour
                    //
                    level->game->new_message("You put on " + item->the_named());
                    play("Metal_Hit_004");
                } else if (item->is_amulet) {
                    //
                    // Amulet
                    //
                    level->game->new_message("You put on " + item->the_named());
                    play("Power_On_002");
                } else if (item->is_treasure) {
                    //
                    // Treasure
                    //
                    level->game->new_message("You pocket " + item->the_named());
                    play("treasure");
                } else {
                    //
                    // Junk
                    //
                    level->game->new_message("You pick up " + item->the_named());
                    play("Power_On_002");
                }
            }
        }
    }

    GH_THING_LOG(this, "collects " + item->the_named());
    GH_THING_LOG(item, " this");

    //
    // I'm being carried now.
    //
    item->incref(GH_THING_REF_IM_BEING_CARRIED);
    item->carrier = this;

    carrying.push_front(item);

    if (item->is_on_map) {
        item->level->map.pop(item);
    }

    if (item->is_staff || item->is_sword ||
        item->is_wand || item->is_bow ||
        item->is_axe || item->is_gun ||
        item->is_warhammer) {
        //
        // Start using this weapon.
        //
        use_weapon(NULL, /* victim */
                   item,
                   true /* don't fire/swing */);
    }

    return (true);
}
