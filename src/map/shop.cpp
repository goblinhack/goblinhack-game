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

    $Id: map.cpp 279 2006-11-24 21:49:26Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_room.h>

//
// Make all items in shops owned by shopkeepers
//
void gh_map::fixup_for_shops (void)
{
    GH_FUNCTION()

    gh_point3d at;

    //
    // Find all shopkeepers and bind them to their rooms!
    //
    for (at.z=0; at.z<size.d; at.z++) {
        for (at.x=tl.x; at.x<=br.x; at.x++) {
            for (at.y=tl.y; at.y<=br.y; at.y++) {

                gh_point3d p = at;
                
                p *= res;

                vector<gh_thing*> * v = real_isat(p);

                if (v == NULL) {
                    continue;
                }

                vector<gh_thing*>::reverse_iterator i;

                for (i = v->rbegin(); i != v->rend(); i++) {
                    gh_thing *t = *i;
                    gh_room *r = t->room;

                    if (!r) {
                        continue;
                    }

                    if (!r->is_shop) {
                        continue;
                    }

                    //
                    // Record that there's a shop on this level.
                    //
                    t->level->has_shop = true;

                    //
                    // Search for the shopkeeper.
                    //
                    if (!t->is_shopkeeper) {
                        continue;
                    }

                    r->shopkeeper = t;
                    r->shopkeeper->incref(GH_THING_REF_SHOPKEEPER);
                }
            }
        }
    }

    //
    // Bind all things to the shopkeeper!
    //
    for (at.z=0; at.z<size.d; at.z++) {
        for (at.x=tl.x; at.x<=br.x; at.x++) {
            for (at.y=tl.y; at.y<=br.y; at.y++) {

                gh_point3d p = at;
                
                p *= res;

                vector<gh_thing*> * v = real_isat(p);

                if (v == NULL) {
                    continue;
                }

                vector<gh_thing*>::reverse_iterator i;

                for (i = v->rbegin(); i != v->rend(); i++) {
                    gh_thing *t = *i;
                    gh_room *r = t->room;

                    if (!r) {
                        continue;
                    }

                    if (!r->is_shop) {
                        continue;
                    }

                    if (r->shopkeeper == NULL) {
                        GH_THROW("shop with no keeper");
                    }

                    //
                    // Make sure that food becomes an item to collect
                    // and pay for and is not consumed immediately.
                    //
                    if (t->is_edible) {
                        t->is_inventory_item = true;
                    }


                    if (!t->is_inventory_item) {
                        continue;
                    }

                    //
                    // Don't want the shopkeeper to own themselves!
                    //
                    if (t->is_shopkeeper) {
                        continue;
                    }

                    //
                    // Increment a reference on ourselves so we can get notified
                    // when this missile is destroyed.
                    //
                    gh_thing *shopkeeper;

                    shopkeeper = r->shopkeeper;

                    if (!shopkeeper) {
                        GH_THROW("shop with no keeper");
                    }

                    t->owner = shopkeeper;
                    shopkeeper->incref(GH_THING_REF_OWNER);

                    t->is_unpaid_for = true;
                }
            }
        }
    }
}

static bool set_room (gh_thing *t, void *arg)
{
    t->room = (gh_room*) arg;

    return (true);
}

static bool is_not_hard_and_is_not_room (gh_thing *thing, void *arg)
{
    if (thing->is_hard) {
        return (false);
    }

    if (thing->room) {
        return (false);
    }

    return (true);
}

void gh_level::fixup_dynamic_room_for_shops (void)
{
    GH_FUNCTION()

    gh_point3d at;

    bool got_one = false;

    //
    // Find all shopkeepers and bind them to their rooms!
    //
    for (at.z=0; at.z<map.size.d; at.z++) {
        for (at.x=map.tl.x; at.x<=map.br.x; at.x++) {
            for (at.y=map.tl.y; at.y<=map.br.y; at.y++) {

                gh_point3d p = at;
                
                p *= map.res;

                vector<gh_thing*> * v = map.real_isat(p);

                if (v == NULL) {
                    continue;
                }

                vector<gh_thing*>::reverse_iterator i;

                for (i = v->rbegin(); i != v->rend(); i++) {
                    gh_thing *t = *i;

                    //
                    // Search for the shopkeeper.
                    //
                    if (!t->is_shopkeeper) {
                        continue;
                    }

                    //
                    // Already have a room?
                    //
                    if (t->room) {
                        continue;
                    }

                    got_one = true;

                    gh_room *r = new gh_room("");

                    rooms.push_back(r);

                    //
                    // Flood fill setting room type.
                    //
                    map.fill_while(p, set_room,
                                   is_not_hard_and_is_not_room, r);

                    break;
                }
            }
        }
    }

    if (got_one) {
        map.fixup_for_shops();
    }
}
