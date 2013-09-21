/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (look your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: tick.cpp 3 2006-08-25 14:46:56Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_term.h>
#include <gh_thing.h>
#include <gh_templates.h>
#include <gh_game.h>

void gh_player::make_item_list (void)
{
    GH_FUNCTION()

    erase(items);

    //
    // For each item we're carrying, work out what row it
    // will appear in. Start from the end so that things
    // stay in the same row once popped.
    //
    // Also, count the items for uniquness.
    //
    list<gh_thing *>::iterator c = thing->carrying.begin();
    while (c != thing->carrying.end()) {

        gh_thing *item_thing = *c;
        bool got_one = false;

        item_thing->level = thing->level;
        item_thing->add_all_things();

        //
        // Already carrying it?
        //
        list<gh_item>::iterator i;
        for (i = items.begin(); i != items.end(); i++) {
            gh_item *item = &(*i);

            if (item->name == item_thing->name) {
                if (item_thing->is_chargeable) {
                    item->how_many += item_thing->charges;
                } else {
                    item->how_many++;
                }

                item->is_chargeable     |= 
                            item_thing->is_chargeable ? 1 : 0;
                item->is_stolen         |= 
                            item_thing->is_stolen ? 1 : 0;
                item->is_unpaid_for     |= 
                            item_thing->is_unpaid_for ? 1 : 0;
                item->is_weapon         |= 
                            item_thing->is_weapon ? 1 : 0;
                item->is_sword          |= 
                            item_thing->is_sword ? 1 : 0;
                item->is_staff          |= 
                            item_thing->is_staff ? 1 : 0;
                item->is_potion         |= 
                            item_thing->is_potion ? 1 : 0;
                item->is_spell          |= 
                            item_thing->is_spell ? 1 : 0;
                item->is_wand           |= 
                            item_thing->is_wand ? 1 : 0;
                item->is_axe            |= 
                            item_thing->is_axe ? 1 : 0;
                item->is_warhammer      |= 
                            item_thing->is_warhammer ? 1 : 0;
                item->is_bow            |= 
                            item_thing->is_bow ? 1 : 0;
                item->is_gun            |= 
                            item_thing->is_gun ? 1 : 0;
                item->is_armour         |= 
                            item_thing->is_armour ? 1 : 0;
                item->is_amulet         |= 
                            item_thing->is_amulet ? 1 : 0;
                item->is_cursed         |= 
                            item_thing->is_cursed ? 1 : 0;
                item->curse_noticed     |= 
                            item_thing->curse_noticed ? 1 : 0;

                //
                // If we can see cursed items, they shouldn't be in the
                // list, but just in case.
                //
                if (thing->can_see_cursed_items) {
                    if (item->is_cursed) {
                        item->curse_noticed = true;
                    }
                }

                if (item_thing->weight >= 50) {
                    item->is_heavy = true;
                }

                if (item_thing->weight >= 80) {
                    item->is_vheavy = true;
                }

                if (item_thing->is_weapon &&
                    (item_thing->health <= (item_thing->orig_health / 2))) {
                    item->is_damaged = true;
                }

                got_one = true;
                break;
            }
        }

        if (got_one) {
            //
            // An existing thing.
            //
            c++;
            continue;
        }

        //
        // A new unique item. Push it and record where it is.
        //
        int sort = 0;

        if (item_thing->is_wand) {
            sort = 1;
        }

        if (item_thing->is_axe) {
            sort = 2;
        }

        if (item_thing->is_warhammer) {
            sort = 3;
        }

        if (item_thing->is_bow) {
            sort = 4;
        }

        if (item_thing->is_sword) {
            sort = 5;
        }

        if (item_thing->is_staff) {
            sort = 6;
        }

        if (item_thing->is_gun) {
            sort = 7;
        }

        if (item_thing->is_potion) {
            sort = 8;
        }

        if (item_thing->is_spell) {
            sort = 9;
        }

        if (item_thing->is_armour) {
            sort = 10;
        }

        if (item_thing->is_amulet) {
            sort = 11;
        }

        //
        // Find its sort position.
        //
        for (i = items.begin(); i != items.end(); i++) {
            gh_item *item = &(*i);

            if (item->sort > sort) {
                break;
            }
        }

        gh_item new_item(item_thing->name, sort, 1, 0);

        if (item_thing->is_chargeable) {
            new_item.how_many = item_thing->charges;
        }

        new_item.is_chargeable  |= 
                    item_thing->is_chargeable ? 1 : 0;
        new_item.is_stolen      |= 
                    item_thing->is_stolen ? 1 : 0;
        new_item.is_unpaid_for  |= 
                    item_thing->is_unpaid_for ? 1 : 0;
        new_item.is_weapon      |= 
                    item_thing->is_weapon ? 1 : 0;
        new_item.is_sword       |= 
                    item_thing->is_sword ? 1 : 0;
        new_item.is_staff       |= 
                    item_thing->is_staff ? 1 : 0;
        new_item.is_potion      |= 
                    item_thing->is_potion ? 1 : 0;
        new_item.is_spell       |= 
                    item_thing->is_spell ? 1 : 0;
        new_item.is_wand        |= 
                    item_thing->is_wand ? 1 : 0;
        new_item.is_axe         |= 
                    item_thing->is_axe ? 1 : 0;
        new_item.is_warhammer   |= 
                    item_thing->is_warhammer ? 1 : 0;
        new_item.is_bow         |= 
                    item_thing->is_bow ? 1 : 0;
        new_item.is_gun         |= 
                    item_thing->is_gun ? 1 : 0;
        new_item.is_armour      |= 
                    item_thing->is_armour ? 1 : 0;
        new_item.is_amulet      |= 
                    item_thing->is_amulet ? 1 : 0;
        new_item.is_cursed      |= 
                    item_thing->is_cursed ? 1 : 0;
        new_item.curse_noticed  |= 
                    item_thing->curse_noticed ? 1 : 0;

        //
        // If we can see cursed items, they shouldn't be in the
        // list, but just in case.
        //
        if (thing->can_see_cursed_items) {
            if (new_item.is_cursed) {
                new_item.curse_noticed = true;
            }
        }

        if (item_thing->weight >= 50) {
            new_item.is_heavy = true;
        }

        if (item_thing->weight >= 80) {
            new_item.is_vheavy = true;
        }

        if (item_thing->is_weapon &&
            (item_thing->health <= (item_thing->orig_health / 5))) {
            new_item.is_damaged = true;
        }

        items.insert(i, new_item);

        c++;
    }

    //
    // Work out the rows things live in now, adding the padding break
    // between different types of items.
    //
    int row = GH_TERM_HEIGHT;
    int sort = 0;
    bool got_at_least_one_item = false;

    int j;
    list<gh_item>::iterator i;
    int last_shortcut_key_item_row = -1;
    int shortcut_key = -1;

    for (j = 0, i = items.begin(); i != items.end(); i++, j++) {
        gh_item *item = &(*i);

        row--;

        if (!got_at_least_one_item) {
            sort = item->sort;
            got_at_least_one_item = true;
        }

        if (sort != item->sort) {
            //
            // As long as the list stays small, allow gaps to make
            // it easier to read.
            //
            row -= 1;

            sort = item->sort;
        }

        item->row = row;

        item->item_no = j;

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

            if (item->row != last_shortcut_key_item_row) {
                last_shortcut_key_item_row = item->row;
                shortcut_key++;
            }

            item->shortcut_key = shortcut_key;
        }
    }
}

void gh_player::display_item_list (void)
{
    GH_FUNCTION()

    if (!items.size()) {
        return;
    }

    int row = 0;
    int sort = 0;
    bool got_at_least_one_item = false;

    list<gh_item>::iterator i;
    for (i = items.begin(); i != items.end(); i++) {
        gh_item *item = &(*i);

        row = item->row;

        //
        // Is this item in a different compartment? backpack/wands/etc...?
        //
        if (!got_at_least_one_item) {
            got_at_least_one_item = true;
            sort = item->sort;
        }

        //
        // Print the title of the previous element. Usually this is the
        // backpack.
        //
        
        //
        // Is this item in a different compartment? backpack/wands/etc...?
        //
        if (item->sort != sort) {
            //
            // Work out where to place the title of the previous block
            // of items. i.e. below this item, but how far below depends
            // on how much space we have on the scree.
            //
            gh_term_goto(GH_TERM_WIDTH - 26, row + 1);

            gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);

            switch (sort) {
            case 0:
                gh_term_puts("Backpack");
                break;
            case 1:
                gh_term_puts("Wands");
                break;
            case 2:
                gh_term_puts("Axes");
                break;
            case 3:
                gh_term_puts("Warhammers");
                break;
            case 4:
                gh_term_puts("Bows");
                break;
            case 5:
                gh_term_puts("Swords");
                break;
            case 6:
                gh_term_puts("Staffs");
                break;
            case 7:
                gh_term_puts("Guns");
                break;
            case 8:
                gh_term_puts("Potions");
                break;
            case 9:
                gh_term_puts("Spells");
                break;
            case 10:
                gh_term_puts("Armour");
                break;
            case 11:
                gh_term_puts("Amulets");
                break;
            }

            sort = item->sort;
        }

        //
        // Print the item name, key etc...
        //
        gh_term_goto(GH_TERM_WIDTH - 27, row);

        //
        // Print the item name
        //
        if (thing->weapon) {
            if (item->name == thing->weapon->name) {
                gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
            } else {
                gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
            }
        } else {
            gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        }

        static char tmp_str[100];

        tmp_str[0] = '\0';

        sprintf(tmp_str, "%25s", item->name.c_str());
        gh_term_putf(tmp_str);

        //
        // Is this line selected?
        //
        if (item->item_no == item_last_selected) {
            gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
            gh_term_putc('<');
        } else {
            gh_term_putc(' ');
        }

        //
        // Print the short cut key for this item along with its name.
        //
        gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);

        if (item->shortcut_key != -1) {
            switch (item->shortcut_key) {
                case 0: gh_term_putc('0'); break;
                case 1: gh_term_putc('1'); break;
                case 2: gh_term_putc('2'); break;
                case 3: gh_term_putc('3'); break;
                case 4: gh_term_putc('4'); break;
                case 5: gh_term_putc('5'); break;
                case 6: gh_term_putc('6'); break;
                case 7: gh_term_putc('7'); break;
                case 8: gh_term_putc('8'); break;
                case 9: gh_term_putc('9'); break;
                case 10: gh_term_putc(')'); break;
                case 11: gh_term_putc('!'); break;
                case 12: gh_term_putc('"'); break;
                case 13: gh_term_putc('#'); break;
                case 14: gh_term_putc('$'); break;
                case 15: gh_term_putc('%'); break;
                case 16: gh_term_putc('^'); break;
                case 17: gh_term_putc('&'); break;
                case 18: gh_term_putc('*'); break;
                case 19: gh_term_putc('('); break;
                default: gh_term_putc(' '); break;
            }
        }

        //
        // Print any other item intrinsics
        //
        gh_term_goto(GH_TERM_WIDTH - 30, row);

        gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);
        
        tmp_str[0] = '\0';

        if (item->is_vheavy) {
            sprintf(tmp_str, "%%%%fg=purpleV.Heavy");
        } else if (item->is_heavy) {
            sprintf(tmp_str, "%%%%fg=purpleHeavy");
        }

        if ((item->is_chargeable) || (item->how_many > 1)) {
            sprintf(tmp_str + strlen(tmp_str),
                    " %%%%fg=purple[%d]", item->how_many);
        }

        if (item->is_cursed && item->curse_noticed) {
            sprintf(tmp_str + strlen(tmp_str),
                    " %%%%fg=orangeCursed");
        }

        if (item->is_damaged) {
            sprintf(tmp_str + strlen(tmp_str),
                    " %%%%fg=redDAMAGED");
        }

        if (item->is_stolen) {
            sprintf(tmp_str + strlen(tmp_str),
                    " %%%%fg1=red%%%%bg1=white"
                    "%%%%fg2=white%%%%bg2=red"
                    "STOLEN!");

        } else if (item->is_unpaid_for) {
            sprintf(tmp_str + strlen(tmp_str),
                    " %%%%fg=yellowUNPAID");
        }

        if (tmp_str[0]) {
            gh_term_goto(GH_TERM_WIDTH - 20 -
                         gh_term_length(tmp_str), row);
            gh_term_putf(tmp_str);
        }
    }

    //
    // As we print the list backwards, this appears at the top of
    // the screen. i.e. the first item.
    //
    gh_term_goto(GH_TERM_WIDTH - 26, row-1);
    gh_term_fgbg(GH_TERM_COLOR_GRAY_7, GH_TERM_COLOR_BLACK);

    switch (sort) {
    case 0:
        gh_term_puts("Backpack");
        break;
    case 1:
        gh_term_puts("Wands");
        break;
    case 2:
        gh_term_puts("Axes");
        break;
    case 3:
        gh_term_puts("Warhammers");
        break;
    case 4:
        gh_term_puts("Bows");
        break;
    case 5:
        gh_term_puts("Swords");
        break;
    case 6:
        gh_term_puts("Staffs");
        break;
    case 7:
        gh_term_puts("Guns");
        break;
    case 8:
        gh_term_puts("Potions");
        break;
    case 9:
        gh_term_puts("Spells");
        break;
    case 10:
        gh_term_puts("Armour");
        break;
    case 11:
        gh_term_puts("Amulets");
        break;
    }
}

//
// Map a shortcut key to an item on the inventory. 0-9 and shift 0-9
//
bool gh_player::shortcut_key_use_item (const SDL_keysym & sym)
{
    int key = -1;

    GH_FUNCTION()

    switch (sym.mod) {
        case KMOD_SHIFT:
        case KMOD_LSHIFT:
        case KMOD_RSHIFT:
        case KMOD_ALT:
        case KMOD_LALT:
        case KMOD_RALT:
        case KMOD_CTRL:
        case KMOD_LCTRL:
        case KMOD_RCTRL:
            switch (sym.sym) {
                case SDLK_0: key = 10; break;
                case SDLK_1: key = 11; break;
                case SDLK_2: key = 12; break;
                case SDLK_3: key = 13; break;
                case SDLK_4: key = 14; break;
                case SDLK_5: key = 15; break;
                case SDLK_6: key = 16; break;
                case SDLK_7: key = 17; break;
                case SDLK_8: key = 18; break;
                case SDLK_9: key = 19; break;

            default:
                break;
            }

        default:
            break;
    }

    if (key == -1) {
        switch (sym.sym) {
            case SDLK_0: key = 0; break;
            case SDLK_1: key = 1; break;
            case SDLK_2: key = 2; break;
            case SDLK_3: key = 3; break;
            case SDLK_4: key = 4; break;
            case SDLK_5: key = 5; break;
            case SDLK_6: key = 6; break;
            case SDLK_7: key = 7; break;
            case SDLK_8: key = 8; break;
            case SDLK_9: key = 9; break;
            default: return (false);
        }
    }

    if (key == -1) {
        return (false);
    }

    list<gh_item>::iterator i;
    for (i = items.begin(); i != items.end(); i++) {
        gh_item *item = &(*i);

        if (item->shortcut_key == key) {
            thing->use_item_name(item->name);

            make_item_list();

            return (true);
        }
    }

    return (false);
}

bool gh_player::drop_item (void)
{
    GH_FUNCTION()

    if (!items.size()) {
        game->new_message("Thine pockets are empty noble sire");
        return (false);
    }

    GH_THING_LOG(thing, "wants to drop something ");

    list<gh_item>::iterator i;
    for (i = items.begin(); i != items.end(); i++) {
        gh_item *item = &(*i);

        if ((items.size() == 1) || (item->item_no == item_last_selected)) {
            GH_THING_LOG(thing, "wants to drop the " + item->name);

            return (thing->drop_item_name(item->name));
        }
    }

    return (false);
}

