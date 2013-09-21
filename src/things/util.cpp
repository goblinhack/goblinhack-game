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
#include <gh_thing.h>
#include <gh_level.h>
#include <gh_game.h>
#include <gh_player.h>
#include <gh_math.h>
#include <gh_thing_registration.h>

//
// If this thing is an item. monster or whatever; whom is using it?
//
gh_thing *gh_thing::get_owner (void)
{
    if (owner) {
        return (owner);
    }

    if (missile_thrower) {
        return (missile_thrower);
    }

    if (weapon_wielder) {
        return (weapon_wielder);
    }

    if (prev) {
        return (prev->get_owner());
    }

    if (carrier) {
        return (carrier);
    }

    return ((gh_thing*)this);
}

void gh_thing::flash (gh_pixel p)
{
    GH_FUNCTION()

    gh_thing *t;

    t = this;

    while (t) {
        t->flash_color.r = ((float)p.r) / 255.0;
        t->flash_color.g = ((float)p.g) / 255.0;
        t->flash_color.b = ((float)p.b) / 255.0;
        t->flash_color.a = ((float)p.a) / 255.0;

        t = t->next;
    }
}

string gh_thing::the_named (void)
{
    string n;

    if (text != "") {
        n = text + ", " + "%%fg=green the ";
    } else {
        n = "the ";
    }

    if (poisoned()) {
        n += "poisoned ";
    }

    if (stoned()) {
        n += "petrifying ";
    }

    if (is_monst || is_hero) {
        if (raging()) {
            n += "raging ";
        }

        if (is_dead) {
            n += "dead ";
        }
    }

    if (karma <= -700) {
        n += "diabolic ";
    } else if (karma <= -500) {
        n += "evil ";
    }

    if (karma > 500) {
        n += "holy ";
    }

    if (is_cursed && curse_noticed) {
        n += "cursed ";
    }

    if (invisible()) {
        n += "invisible ";
    }

    if (is_spell) {
        n += "spell of ";
    }

    if (is_wand) {
        n += "wand of ";
    }

    if (is_amulet) {
        n += "amulet of ";
    }

    n += name;

    if (is_potion) {
        n += " potion";
    }

    if (is_armour) {
        n += " armour";
    }

    if (is_sword) {
        n += " sword";
    }

    if (is_axe) {
        n += " axe";
    }

    if (is_warhammer) {
        n += " warhammer";
    }

    if (is_staff) {
        n += " staff";
    }

    if (is_bow) {
        n += " bow";
    }

    return (n);
}

string gh_thing::named (void)
{
    string n;

    if (text != "") {
        n = text + ", " + "%%fg=green";
    } else {
        n = "";
    }

    if (poisoned()) {
        n += "poisoned ";
    }

    if (stoned()) {
        n += "petrifying ";
    }

    if (is_monst || is_hero) {
        if (raging()) {
            n += "raging ";
        }

        if (is_dead) {
            n += "dead ";
        }
    }

    if (karma <= -700) {
        n += "diabolic ";
    } else if (karma <= -500) {
        n += "evil ";
    }

    if (karma > 500) {
        n += "holy ";
    }

    if (is_cursed && curse_noticed) {
        n += "cursed ";
    }

    if (is_invisible_anim() || invisible()) {
        n += "invisible ";
    }

    if (is_spell) {
        n += "spell of ";
    }

    if (is_wand) {
        n += "wand of ";
    }

    if (is_amulet) {
        n += "amulet of ";
    }

    n += name;

    if (is_potion) {
        n += " potion";
    }

    if (is_armour) {
        n += " armour";
    }

    if (is_sword) {
        n += " sword";
    }

    if (is_axe) {
        n += " axe";
    }

    if (is_warhammer) {
        n += " warhammer";
    }

    if (is_staff) {
        n += " staff";
    }

    if (is_bow) {
        n += " bow";
    }

    return (n);
}

string gh_thing::char_to_name (string char_or_name)
{
    if (char_or_name.size() != 1) {
        return (char_or_name);
    }

    int c = char_or_name[0];
    int how_many = level->map_char_how_many(c);

    return (level->map_char_nth(c, rand() % how_many));
}

string gh_thing::char_to_summon_name (string char_or_name)
{
    string summon_what;

    //
    // Find something to summon.
    //
    for (;;) {
        summon_what = gh_thing::char_to_name(char_or_name);

        gh_thing *t = gh_thing::find(summon_what);
        if (!t) {
            return ("<bug>");
        }

        if (t->cannot_summon) {
            continue;
        }

        //
        // Make some monsters less likely to be summoned.
        //
        if (gh_math::rand10000() < t->appearing_chance_on_level(level)) {
            return (summon_what);
        }

        continue;
    }
}

//
// Does a thing match a pattern string?
//
bool gh_thing::matches (const string &s)
{
    switch (s[0]) {
    case 'a':
        if (s == "all") {
            return (true);
        }
        break;

    case 'c':
        if (s == "cold") {
            return (is_cold);
        }

        if (s == "clever") {
            return (intelligence > 30);
        }
        break;

    case 'h':
        if (s == "hero") {
            return (is_hero);
        }

        if (s == "holy") {
            return (is_holy);
        }
        break;

    case 'e':
        if (s == "exit") {
            return (is_exit);
        }

        if (s == "evil") {
            return (karma < -600);
        }
        break;
        
    case 'f':
        if (s == "fire") {
            return (is_fire);
        }
        break;

    case 'g':
        if (s == "good") {
            return (karma > 100);
        }

        if (s == "generator") {
            return (is_generator);
        }
        break;

    case 'm':
        if (s == "monst") {
            return (is_monst);
        }

        if (s == "meat") {
            return (is_meat);
        }
        break;

    case 'r':
        if (s == "return exit") {
            if (is_return_exit) {
                return (true);
            }
        }

        if (s == "rock") {
            return (is_rock);
        }
        break;

    case 'u':
        if (s == "undead") {
            return (is_undead);
        }
        break;

    case 't':
        if (s == "thief") {
            return (is_thief);
        }
        break;

    case 'w':
        if (s == "wall") {
            return (is_wall);
        }
        if (s == "water") {
            return (is_water);
        }
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return (tostring(id) == s);

    default:
        break;
    }

    //
    // Could enhance for is_small is_spider etc...
    //
   
    // Do a simple pattern match on our file name and the pattern
    //
    if (string_contains(short_name, s)) {
        return (true);
    }

    return (false);
}

int gh_thing::hero_distance (void)
{
    gh_thing_registration_args args;

    if (!level || !level->game || !level->game->player) {
        return (9999);
    }

    gh_thing *hero = level->game->player->thing;

    if (!hero) {
        return (9999);
    }

    return ((int)(GH_DISTANCE(hero->at.x, hero->at.y, at.x, at.y) / GH_RES));
}

int gh_thing::appearing_chance_on_level (gh_level *l)
{
    int chance = 0;

    if (!l) {
        //
        // Some things like walls, just always appear.
        //
        chance = 1000;
    } else {
        //
        // Doth this thing want to exist on this level?
        //
        if ((int) appearing_chance.size() < l->level_no) {
            //
            // This monster is not specified at this depth.
            //
            if (!appearing_chance.size()) {
                //
                // Some things like walls, just always appear.
                //
                chance = 1000;
            } else {
                //
                // Keep using the last value specified at the last highest
                // level.
                //
                chance = appearing_chance[appearing_chance.size()-1];
            }
        } else if (appearing_chance.size()) {
            //
            // This monster is specified for a percentage at this level
            // depth.
            //
            chance = appearing_chance[l->level_no - 1];
        } else {
            //
            // Else just give it a 'sometimes appears' value.
            //
            chance = 100;
        }
    }

    return (chance);
}

int gh_thing::toughness (void)
{
    int score;

    if (template_thing) {
        score = (max(health, orig_health) / 100) * (template_thing->fate+1);
    } else {
        score = (max(health, orig_health) / 100) * (fate+1);
    }

    score += cold_proof();
    score += suffocation_proof();
    score += acid_proof();
    score += fire_proof();
    score += poison_proof();
    score += stone_proof();
    score += teleport_proof();
    score += water_proof();

    if (loves_acid) {
        score += 5;
    }
    if (loves_cold) {
        score += 5;
    }
    if (loves_death) {
        score += 5;
    }
    if (loves_fire) {
        score += 5;
    }
    if (can_cast_spells) {
        score += 5;
    }
    if (bite_armour) {
        score += 1;
    }
    if (bite_intelligence) {
        score += 1;
    }
    if (bite_poison) {
        score += 1;
    }
    if (bite_fate) {
        score += 10;
    }

//cerr << score << " " << name << endl;
    return (score);
}
