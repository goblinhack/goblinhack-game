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
#include <gh_room.h>

//
// Drop all we can on the level.
//
void gh_thing::drop_all_items (void)
{
    GH_FUNCTION()

    if (carrying.begin() == carrying.end()) {
        return;
    }

    GH_THING_LOG(this, "is trying to drop all items");

    for (;;) {

        if (carrying.begin() == carrying.end()) {
            return;
        }

        gh_thing *t;

        GH_THING_LOG(this, "is trying to drop an item in drop all action");

        t = *carrying.begin();

        GH_THING_VERIFY(t);

        if (!drop_item(t, true /* force */)) {
            t->dead("can't drop", false /* explosion */);
        }
    }
}

//
// Drop an item somewhere around the thing
//
bool gh_thing::drop_item (gh_thing *item, bool force)
{
    GH_FUNCTION()

    GH_THING_VERIFY(item);

    GH_THING_LOG(this, "is trying to drop " + item->the_named());
    GH_THING_LOG(item, " this");

    //
    // Check we are carrying it.
    //
    if (!item->isref(GH_THING_REF_IM_BEING_CARRIED)) {
        return (true);
    }

    //
    // No longer being carried. This is cleared before the above reference,
    // so it is possible whilst destroying items to try and unwield and
    // hence miss the above check.
    //
    if (!item->carrier) {
        return (true);
    }

    vector<gh_thing *> created;

    gh_point3d where = at;

    if (!force) {
        if (item->is_cursed) {
            GH_THING_LOG(this, item->the_named() + " is cursed");
            GH_THING_LOG(item, "  is cursed");

            if (is_hero) {
                level->game->new_message("You can't seem to drop this item...");

                play("Filtered_Roller");
            }

            item->curse_noticed = true;
            return (false);
        }
    }

    //
    // Keep a reference so we can pop/unpop it with impunity.
    //
    item->incref(GH_THING_REF_DROP_ITEM);

    //
    // Now see where it will fit around the last thing?
    //
    vector<gh_point3d> possible;

    //
    // An item being carried and wielded is on the map already, so this is
    // expected.
    //
    bool placed = false;

    if (item->is_on_map) {
        GH_THING_LOG(this, item->the_named() +
                     " is on the map already; pop and replace");
        GH_THING_LOG(item, "  this");

        level->map.pop(item);

        //
        // Restore back to the map graphics.
        //
        item->anim_frames = item->template_thing->anim_frames;

        item->update_anim();
    }

    if (!placed) {
        for (int rad = 1; rad < 2; rad++) {
            for (int i = -rad; i <= rad; i++) {
                for (int j = -rad; j <= rad; j++) {
                    //
                    // Can'item place_item on top of thyself!
                    //
                    if (!i && !j) {
                        continue;
                    }

                    const gh_anim_frame *anim_a = get_anim();
                    const gh_anim_frame *anim_b = item->get_anim();

                    gh_point3d place_item_location(
                      (int)((anim_a->pix_max_w/2 + anim_b->pix_max_w/2) * i),
                      (int)((anim_b->pix_max_h/2 + anim_b->pix_max_h/2) * j),
                      0);

                    place_item_location += where;

                    item->at = gh_point3d(place_item_location);

                    GH_THING_VERIFY(item);

                    //
                    // If dropping an unpaid for item in a shop, put it
                    // back where it was created, to stop dropping things 
                    // outside a shop's walls.
                    //
                    if (item->is_unpaid_for) {
                        if (item->room && item->room->is_shop) {
                            item->at = item->born;
                        }
                    }

                    if (!level) {
                        continue;
                    }

                    //
                    // Try and place it.
                    //
                    if (!level->map.push(item)) {
                        continue;
                    }

                    //
                    // Check that we can exist in this space on the map.
                    //
                    if (item->hit_obstacle(true /* ignore_hero */)) {
                        level->map.pop(item);
                        continue;
                    }

                    placed = true;

                    if (placed) {
                        break;
                    }
                }

                if (placed) {
                    break;
                }
            }

            if (placed) {
                break;
            }
        }
    }

    if (!placed) {
        if (force || level->being_deleted) {
            //
            // Can't drop on level end is normal. Just force it.
            //
            GH_THING_LOG(item, "  can't drop, force it");

            placed = true;
        }
    }

    if (!placed) {
        //
        // Couldn't drop a weapon? Start swinging it again.
        //
        if (item->is_weapon) {
            wield_new_weapon(item);
        }

        //
        // We couldn't place the item
        //
        item->decref(GH_THING_REF_DROP_ITEM);

        GH_THING_LOG(this, " couldn't drop " + item->the_named());
        GH_THING_LOG(item, "  this");

        if (is_hero) {
            //
            // Don't log things when we die. Looks messy.
            //
            if (!is_dead && !item->is_dead) {
                if (level) {
                    level->game->new_message("No space to drop the " + item->name);
                }
            }
        }

        return (false);
    }

    //
    // Should be safely on the map.
    //
    item->decref(GH_THING_REF_DROP_ITEM);

    GH_THING_VERIFY(item);

    //
    // Destroyed, dropped sword? don't log it.
    //
    GH_THING_LOG(this, " dropping " + item->the_named());
    GH_THING_LOG(item, "  this");

    if (is_hero) {
        //
        // Don't log things when we die. Looks messy.
        //
        if (!is_dead && !item->is_dead) {
            if (level) {
                level->game->new_message("You dropped " + 
                                         item->the_named());
            }
        }
    }

    //
    // Remove safely from the list.
    //
    list<gh_thing *>::iterator i = carrying.begin();

    while (i != carrying.end()) {
        GH_THING_LOG(this, "  item " + (*i)->named());

        if (*i == item) {
            GH_THING_VERIFY(item);

            carrying.erase(i);

            //
            // We are unwielding this by dropping it.
            //
            if (item->weapon_wielder) {
                //
                // Remove the ref from the carrying list before we try and
                // unwield else we hold onto it. We should be guaranteed a
                // lock as someone is holding us.
                //
                item->carrier = NULL;
                item->decref(GH_THING_REF_IM_BEING_CARRIED);

                GH_THING_LOG(this, " is being dropped; unwielding " +
                             item->the_named());

                unwield();

                return (true);
            }

            GH_THING_LOG(this, " dropped " + item->the_named());
            GH_THING_LOG(item, "  this");

            //
            // Remove the ref from the carrying list.
            //
            item->carrier = NULL;
            item->decref(GH_THING_REF_IM_BEING_CARRIED);

            return (true);
        }

        i++;
    }

    //
    // Not on the carried list?
    //
    GH_THING_LOG(this, " is unable to drop " + item->the_named());
    GH_THING_LOG(item, "  this");

    GH_THROW(name + " is unable to drop " + item->the_named());

    return (false);
}

//
// Drop an item somewhere around the thing
//
bool gh_thing::drop_item_name (string name)
{
    GH_FUNCTION()

    list<gh_thing *>::iterator i  = carrying.begin();

    while (i != carrying.end()) {
        if ((*i)->name == name) {
            return (drop_item(*i, false /* force */));
        }

        i++;
    }

    GH_THROW("didn't find " + name + " to drop");

    return (false);
}
