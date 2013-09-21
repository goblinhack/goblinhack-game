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
#include <gh_game.h>

//
// Flash on spell use!
//
void gh_thing::spell_effect (void)
{
    GH_FUNCTION()

    if (is_hero) {
        gh_global::flash_color.r = 4.7;
        gh_global::flash_color.g = 4.5;
        gh_global::flash_color.b = 4.0;
    } else {
        gh_global::flash_color.r = 4.0;
        gh_global::flash_color.g = 0.0;
        gh_global::flash_color.b = 0.0;
    }
}

//
// Speed self back up
//
void gh_thing::spell_fast (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    if (item->is_cursed) {
        metabolism_boost_start(-metabolism() / 2);
        stamina_boost_start(-stamina() / 2);

        if (is_hero) {
            level->game->new_message(
             "%%fg=yellowYou stagger slowly%%fg=green");
        }

        return;
    }

    metabolism_boost_start(metabolism() / 2);
    metabolism_boost_start(metabolism() / 4);
    stamina_boost_start(stamina() / 2);

    if (is_hero) {
        level->game->new_message(
         "%%fg=yellowYou feel great!%%fg=green");

        sleeping_boost_stop();
    }
}

//
// Throw something
//
void gh_thing::spell_throw (gh_thing *item, string what)
{
    GH_FUNCTION()

    spell_effect();

    (void) try_to_throw_something(NULL, /* target */
                                  this, /* owner */
                                  what,
                                  false /* want_thrown_list */,
                                  true /* always_throw */);
}

//
// Teleport thyself
//
void gh_thing::spell_teleport_self (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    item->teleport_attack(this);
}

//
// Remove all curses on carried objects
//
void gh_thing::spell_remove_curse (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    if (item) {
        is_cursed = item->is_cursed;
    } else {
        is_cursed = false;
    }

    curse_noticed = true;

    list<gh_thing *>::iterator i  = carrying.begin();
    while (i != carrying.end()) {
        if (item) {
            (*i)->is_cursed = item->is_cursed;
        } else {
            (*i)->is_cursed = false;
        }

        (*i)->curse_noticed = true;

        i++;
    }
}

//
// Remove poison etc...
//
void gh_thing::spell_heal (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    _poisoned = 0;
    poisoned_boost = 0;

    _stoned = 0;
    stoned_boost = 0;

    health += orig_health;

    sleeping_boost_stop();
}

void gh_thing::spell_raise_dead (gh_thing *item)
{
    GH_FUNCTION()

    int res = GH_RES;
    int sx = -2;
    int sy = -2;
    int ex = +2;
    int ey = +2;

    GH_LOG("raise dead");

    for (int i = sx; i <= ex; i++) {
        for (int j = sy; j <= ey; j++) {
            vector <gh_thing*> **vp =
                level->map.real_isatp(at + gh_point3d(i*res, j*res, 0));

            if (vp == NULL) {
                continue;
            }

            vector <gh_thing*> *v = *vp;

            if (v == NULL) {
                continue;
            }

            vector<gh_thing*>::iterator i;

            for (i = v->begin(); i != v->end(); i++) {
                gh_thing *t = *i;

                if (!t->is_monst) {
                    continue;
                }

                //
                // How likely is it to succeed?
                //
                int score = t->want_to_attack(this); 

                bool ok;

                if (score > 500) {
                    ok = (gh_math::rand100() < 5);
                } else if (score > 200) {
                    ok = (gh_math::rand100() < 10);
                } else if (score > 100) {
                    ok = (gh_math::rand100() < 20);
                } else if (score < 0) {
                    ok = (gh_math::rand100() < 50);
                } else if (score < -100) {
                    ok = (gh_math::rand100() < 70);
                } else if (score < -200) {
                    ok = (gh_math::rand100() < 90);
                } else {
                    ok = (gh_math::rand100() < 95);
                }

                if (ok) {
                    if (is_hero) {
                        level->game->new_message(
                         "%%fg=yellow" + t->the_named() + 
                         "gasps and breathes%%fg=green");
                    }

                    t->rm_dead_things();

                    t->is_dead = false;
                    t->karma -= 50;

                    karma -= 50;
                } else {
                    if (is_hero) {
                        level->game->new_message(
                         "%%fg=yellow" + t->the_named() + 
                         "shudders then lies still%%fg=green");
                    }

                    karma -= 20;
                }
            }
        }
    }
}

//
// Shield!
//
void gh_thing::spell_shield (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    shielded_boost_start(40); // 20 secs
}

//
// Invisible!
//
void gh_thing::spell_invisible (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    invisible_boost_start(40); // 20 secs
}

//
// Vision boost
//
void gh_thing::spell_vision (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    if (item->is_cursed) {
        can_see_distance_boost_start(-can_see_distance() / 2);

        if (is_hero) {
            level->game->new_message(
             "%%fg=yellowYou can't seem to see as well%%fg=green");
        }

        return;
    }

    can_see_distance_boost_start(can_see_distance() / 2);

    if (is_hero) {
        level->game->new_message(
         "%%fg=yellowYou see like the eagle!%%fg=green");
    }
}

//
// Info on the dungeon
//
void gh_thing::spell_info (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    level->game->info_screen = !level->game->info_screen;
}

//
// Carrying weight boost
//
void gh_thing::spell_weightless (gh_thing *item)
{
    GH_FUNCTION()

    spell_effect();

    if (item->is_cursed) {
        heavy_all_items();

        if (is_hero) {
            level->game->new_message(
             "%%fg=yellowYour pack seems much heavier!%%fg=green");
        }
    } else {
        weightless_all_items();

        if (is_hero) {
            level->game->new_message(
             "%%fg=yellowYour pack seems featherweight!%%fg=green");
        }
    }
}
