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

    $Id: loop.cpp 134 2006-09-24 02:43:04Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_term.h>

class gh_monst_info {
public:
    string name;
    int count;
    bool dangerous;
    bool is_dead;
    gh_monst_info(void) { count = 0; dangerous = false; }
};

typedef map<string, gh_monst_info> hash_info;

void gh_player::display_info (void)
{
    GH_FUNCTION()

    gh_level *level = thing->level;
    gh_map *map = &thing->level->map;

    list<string> info;

    display_game();

    info.push_back("");
    info.push_back("%%fg=green");
    info.push_back("  Map Dimensions            : " +
                   tostring((map->br.x - map->tl.x) * GH_RES) + "x" +
                   tostring((map->br.y - map->tl.y) * GH_RES) + " feet  ");

    //
    // How many monsters?
    //
    int monst_total = 0;
    list<gh_thing*>::iterator c;

    hash_info monst_info;
    gh_monst_info i;

    for (c = level->all_things.begin(); c != level->all_things.end(); c++) {
        gh_thing *m = *c;

        if (!m->is_monst) {
            continue;
        }

        if (m->is_body_part) {
            continue;
        }

        monst_total++;

        i = monst_info[m->name];
        i.name = m->name;
        i.count++;

        if (m->health > 150) {
            i.dangerous = true;
        }

        i.is_dead = m->is_dead;

        monst_info[m->name] = i;
    }

    //
    // How much treasure?
    //
    int treasure_total = 0;
    int potion_total = 0;
    int spell_total = 0;

    for (c = level->all_things.begin(); c != level->all_things.end(); c++) {
        gh_thing *m = *c;

        if (m->is_potion) {
            potion_total ++;
        }

        if (m->is_spell) {
            spell_total ++;
        }

        if (m->is_hero) {
            continue;
        }

        treasure_total += m->treasure;
    }

    info.push_back("  Treasure on level         : " + tostring(treasure_total));
    info.push_back("  Potions  on level         : " + tostring(potion_total));
    info.push_back("  Spells   on level         : " + tostring(spell_total));
    info.push_back("  Monsters on level         : " + tostring(monst_total));

    //
    // List all the monsters
    //
    hash_info::iterator s = monst_info.begin();

    list<string> all_monst_info_sorted;

    while (s != monst_info.end()) {
        i = s->second;

        char out[80];

        if (i.is_dead) {
            snprintf(out, sizeof(out)-1, "%3d x %%%%fg=gray %30s%%%%fg=green",
                     i.count, i.name.c_str());
        } else if (i.dangerous) {
            snprintf(out, sizeof(out)-1, "%3d x %%%%fg=red %30s%%%%fg=green",
                     i.count, i.name.c_str());
        } else {
            snprintf(out, sizeof(out)-1, "%3d x %%%%fg=green %30s%%%%fg=green",
                     i.count, i.name.c_str());
        }

        string s_out(out);

        all_monst_info_sorted.push_back(s_out);

        s++;
    }

    all_monst_info_sorted.sort();
    all_monst_info_sorted.reverse();

    list <string>::iterator a_monst_info_string;

    info.push_back("");

    //
    // Print the strings, 2 at a time.
    //
    a_monst_info_string = all_monst_info_sorted.begin();
    while (a_monst_info_string != all_monst_info_sorted.end()) {
        string a = *a_monst_info_string++;
        string b;

        if (a_monst_info_string != all_monst_info_sorted.end()) {
            b = *a_monst_info_string++;
        }

        info.push_back(a + "   " + b);
    }

    info.push_back("");

    //
    // Display all our info
    //
    gh_term_text_box(info,
             GH_TERM_COLOR_WHITE, // middle bg/fg
             GH_TERM_COLOR_BLACK,
             GH_TERM_COLOR_BLACK, // border bg/fg
             GH_TERM_COLOR_BLUE,
             '!',
             -1 // left justify
                   );
}
