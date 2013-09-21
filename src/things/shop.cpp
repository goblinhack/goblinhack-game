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

    $Id: collect.cpp 279 2006-11-24 21:49:26Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_room.h>
#include <gh_math.h>
#include <gh_player.h>
#include <gh_templates.h>

static int item_cost_for (gh_thing *buyer, gh_thing *item)
{
    if (buyer->karma <= -200) {
        return (item->cost * 2);
    }

    return (item->cost);
}

//
// One thing is collecting another thing, item
//
// false on failure to collect.
//
bool gh_thing::collect_in_shop (gh_thing *item)
{
    GH_FUNCTION()

    bool collected;

    vector<string> messages;

    if (!item->room) {
        return (true);
    }

    gh_thing *shopkeeper = item->room->shopkeeper;
    if (!shopkeeper) {
        //
        // Hmm. Suspicious. Not sure how this would happen.
        //
        item->is_unpaid_for = false;

        level->game->new_message("Eeerily empty shop.");

        return (collect_item(item));
    }

    if (shopkeeper->is_dead) {
        //
        // Dead keepers don't complain.
        //
        level->game->new_message("Looks to be a free purchase...");

        //
        // Still unsure if this should be thievery - most likely murdery!
        //
        item->is_unpaid_for = false;

        return (collect_item(item));
    }

    collected = collect_item(item);
    if (!collected) {
        //
        // We're carrying too much!
        //
        if (is_hero) {
            messages.push_back("You're carrying too much to carry more!");
            level->game->new_message(one_of(messages));
        }

        return (false);
    }

    if (is_hero) {
        //
        // A noble purchase.
        //
        if (gh_math::rand100() < 50) {
            messages.push_back("A most wise and noble purchase.");
            messages.push_back("Only used by one careful old wizard.");
            messages.push_back("Even my mother would use that one!");
            messages.push_back("Even my pet rabbit would use that one!");
            messages.push_back("Even my pet mososaur would use that one!");
            messages.push_back("Every home needs one of those!");
            messages.push_back("Every dungoneer needs at least one of those!");
            messages.push_back("Every dungoneer needs at least three of those!");
            messages.push_back("A bargain there!");
            messages.push_back("I'm going out of business with these bargains!");
            messages.push_back("I'm malfunctioning with these bargains!");
            messages.push_back("I'm as crazy as a banana with these prices!");
            messages.push_back("I'm as crazy as a plesiosaur with these prices!");
            messages.push_back("Fifty percent off that item!");
            messages.push_back("Must be sold today! Moving to a new dungeon!");
            messages.push_back("A most high quality item!");
            messages.push_back("A most polished piece!");
            messages.push_back("Why that fine piece, a veritale steal!");
            messages.push_back("Guaranteed results!");
            messages.push_back("You'll never need to return that!");
            messages.push_back("Sure to have a satisfying result!");
            messages.push_back("Sure to have an interesting result!");
            messages.push_back("Surely you should consider more?");
            messages.push_back("I'm insane at these prices!");
            messages.push_back("I'm going mad at these prices!");
            messages.push_back("I'm suffering the heat at these prices!");
            messages.push_back("I'm giving that away!");
            messages.push_back("I'm giving this to charity at these prices!");
            messages.push_back("Why am I selling at these prices?");
            messages.push_back("I insult myself at these prices!");
            messages.push_back("I insult my mother at these prices!");
            messages.push_back("I insult all mothers at these prices!");
        }
    }

    if (messages.size()) {
        level->game->new_message(one_of(messages));
    }

    if (is_hero) {
        level->game->new_message("To you, only %%fg=yellow" +
                                 tostring(item_cost_for(this, item)) +
                                 "%%fg=green Zorkmids");
        level->game->new_message("Press %%fg=redP%%fg=green to pay, "
                                 "%%fg=redBEFORE%%fg=green leaving");
    }

    return (true);
}

void gh_thing::leave_shop (void)
{
    GH_FUNCTION()

    vector<string> messages;

    gh_thing *shopkeeper = NULL;

    //
    // Remove safely from the list.
    //
    list<gh_thing *>::iterator i  = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *item = *i;

        if (item->owner) {
            item->owner->decref(GH_THING_REF_OWNER);
            item->owner = NULL;
        }

        if (item->room) {
            shopkeeper = item->room->shopkeeper;

            if (!shopkeeper) {
                item->is_stolen = false;
                item->is_unpaid_for = false;
                continue;
            }
        }

        item->room = NULL;

        if (item->is_unpaid_for) {
            item->is_stolen = true;

            if (is_thief) {
                i++;
                continue;
            }

            is_thief = true;
            karma -= 500;

            //
            // Call the police
            //
            monst_summon_in_level("monst/police/1");
            monst_summon_in_level("monst/police/1");
            monst_summon_in_level("monst/police/1");
            monst_summon_in_level("monst/police/1");
            monst_summon_adjacent("monst/police/1");
            monst_summon_adjacent("monst/police/1");

            messages.push_back("You steal from me, I break your legs!");
            messages.push_back("You steal from me, I break your antennae!");
            messages.push_back("You steal from me, I break your tentacles!");
            messages.push_back("You steal from me, I break your probiscus!");
            messages.push_back("You steal from me, I break your horns!");
            messages.push_back("You steal from me, I break your long nose!");
            messages.push_back("You steal from me, I break your arms!");
            messages.push_back("You steal from me, I break your pinkies!");
            messages.push_back("You steal from me, I break your wand!");
            messages.push_back("You steal from me, I break your sword!");
            messages.push_back("Come back you scoundrel!");
            messages.push_back("Thief! Thief!");
            messages.push_back("Thief! Scoundrel!");
            messages.push_back("Thief! Not again... <sob>!");
            messages.push_back("Thief! Thief! I say a thief!");
            messages.push_back("Thief! I spit on your shoes!");
            messages.push_back("Thief! I spit on your eldeberry bush!");
            messages.push_back("Thief! I spit in your general direction!");
            messages.push_back("Thief! A pox on you!");
            messages.push_back("Thief! May a pigeon eat your shoes!");
            messages.push_back("Thief! May a hippogriff sit on you!");
            messages.push_back("Thief! May a goblin eat your shoes!");
            messages.push_back("Thief! May a troll drop on your head!");
            messages.push_back("Thief! May you find fourteen spiders in your lunch!");
            messages.push_back("Thief! May a stoat crawl up your pants!");
            messages.push_back("Thief! May a hulk find you attractive!");
            messages.push_back("Thief! May you find maggots in your food!");
            messages.push_back("Thief! May you find dragons in your food!");
            messages.push_back("Thief! Come back so I may thank you!");
            messages.push_back("Thief! You are banned! banned I say!");
            messages.push_back("Thief! And you seemed so honest!");
            messages.push_back("Thief! And your mother was a goblin!");
            messages.push_back("Thief! And your mother was a half goblin!");
            messages.push_back("Thief! And your father was an orc!");
            messages.push_back("Thief! May a camel drool on you!");
            messages.push_back("Thief! May your drown in your own drool!");
        } else {
            item->is_stolen = false;
        }

        i++;
    }

    if (is_hero) {
        if (messages.size()) {
            level->game->new_message("%%fg=yellow" +
                                     one_of(messages) +
                                     "%%fg=green");
        }
    }
}

bool gh_thing::pay_in_shop (void)
{
    GH_FUNCTION()

    vector<string> messages;
    bool tried_to_pay = false;

    //
    // Remove safely from the list.
    //
    list<gh_thing *>::iterator i  = carrying.begin();
    while (i != carrying.end()) {
        gh_thing *item = *i;

        if (item->is_unpaid_for) {
            tried_to_pay = true;

            gh_thing *owner = item->owner;

            if (!owner->is_shopkeeper) {
                messages.push_back("There's no-one to pay... Odd...");
                item->is_unpaid_for = false;
                item->room = NULL;

                if (item->owner) {
                    item->owner->decref(GH_THING_REF_OWNER);
                    item->owner = NULL;
                }

                i++;
                continue;
            }

            //
            // Dead keepers don'item complain.
            //
            if (owner->is_dead) {
                messages.push_back("There's no-one to pay... The keeper is dead!");
                item->is_unpaid_for = false;
                item->room = NULL;

                if (item->owner) {
                    item->owner->decref(GH_THING_REF_OWNER);
                    item->owner = NULL;
                }

                i++;
                continue;
            }

            if (treasure < item_cost_for(this, item)) {
                messages.push_back("Alas you can't afford that!");
                messages.push_back("Alas too expensive for you!");
                messages.push_back("Can't afford that. Overpriced anyway!");
                messages.push_back("Can't afford that. Overpriced junk!");
            } else {
                treasure -= item_cost_for(this, item);
                item->is_unpaid_for = false;

                play("coin_roll", 10);

                messages.push_back("Blessings upon you! I eat tonight!");
                messages.push_back("Blessings upon you! My children eat tonight!");
                messages.push_back("Blessings upon you! My dog eats tonight!");
                messages.push_back("Blessings upon you! My pet dragon eats tonight!");
                messages.push_back("Blessings upon you! My pet slime mold eats tonight!");
                messages.push_back("Blessings upon you! My pet goblin eats tonight!");
                messages.push_back("A worthy purchase!");
                messages.push_back("May the sun shine on your armour!");
                messages.push_back("A thousand thank-yous!");
                messages.push_back("A thousand salutations!");
                messages.push_back("May your camel always be watered!");
                messages.push_back("May your goblin always be watered!");
                messages.push_back("May your dragon always be firey!");
                messages.push_back("May your helmet always be shiny!");
                messages.push_back("May your pet goblin always be ferocious!");
                messages.push_back("May goodly knights accompany you!");
                messages.push_back("May you meet no slime!");
                messages.push_back("Hurry back!");
                messages.push_back("Hurry back! It's lonely here!");
                messages.push_back("Hurry back! It's lonely here! In the dark..");
                messages.push_back("Come again fine customer!");
                messages.push_back("Come again noble sire!");
                messages.push_back("Refer me to your friends!");

                item->room = NULL;

                if (item->owner) {
                    item->owner->decref(GH_THING_REF_OWNER);
                    item->owner = NULL;
                }
            }
        }

        i++;
    }

    if (is_hero) {
        if (messages.size()) {
            level->game->new_message(one_of(messages));
        }
    }

    return (tried_to_pay);
}

void gh_thing::destroyed_item_in_shop (gh_thing *item)
{
    GH_FUNCTION()

    vector<string> messages;
    gh_room *r;

    r = item->room;
    if (!r) {
        return;
    }

    gh_thing *shopkeeper = r->shopkeeper;
    if (!shopkeeper) {
        return;
    }

    //
    // Dead keepers don't complain.
    //
    if (shopkeeper->is_dead) {
        return;
    }

    if (!item_cost_for(this, item)) {
        //
        // Shooting a wall perhaps?
        //
        if (is_hero) {
            level->game->new_message(
                    "%%fg=red"
                    "Clumsy oaf! Damage my shop? Grr."
                    "%%fg=green");
        }
        anger(shopkeeper);
    } else {
        //
        // Destroying something of worth
        //
        if (treasure < item_cost_for(this, item)) {
            //
            // That you can't afford!
            //
            if (is_hero) {
                level->game->new_message(
                            "%%fg=red"
                            "Destroy my stuff! Grr!"
                            "%%fg=green");
            }
            anger(shopkeeper);
        } else {
            treasure -= item_cost_for(this, item);

            if (is_hero) {
                level->game->new_message(
                    "%%fg=yellow"
                    "Clumsy oaf! That cost you, " +
                    tostring(item_cost_for(this, item)) + " Zorkmids"
                    "%%fg=green");
            }
        }
    }

    if (is_hero) {
        if (messages.size()) {
            level->game->new_message(one_of(messages));
        }
    }
}

//
// Used some armour in a shop wihout paying for it?
//
void gh_thing::used_unpaid_item_in_shop (gh_thing *item)
{
    GH_FUNCTION()

    vector<string> messages;
    gh_room *r;

    r = item->room;
    if (!r) {
        return;
    }

    gh_thing *shopkeeper = r->shopkeeper;
    if (!shopkeeper) {
        return;
    }

    //
    // Dead keepers don't complain.
    //
    if (shopkeeper->is_dead) {
        return;
    }

    if (item->is_weapon) {
        //
        // Should be okay to allow heroes to switch to a weapon.
        //
        return;
    }

    //
    // This is a form of skullduggery... Not sure how much I should punish
    // for it...
    //
    is_thief = true;
    karma -= 200;

    level->game->new_message(
                        "%%fg=red"
                        "Use my stuff without paying? Thief!"
                        "%%fg=green"
                        );

    if (is_hero) {
        if (messages.size()) {
            level->game->new_message(one_of(messages));
        }
    }

    anger(shopkeeper);
}
