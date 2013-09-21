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

    $Id: player_powerup_chooser.cpp 276 2006-11-20 21:16:05Z goblinhack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_ui_event.h>
#include <gh_figlet.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_screen.h>
#include <gh_sdl.h>
#include <gh_string.h>
#include <gh_templates.h>
#include <gh_audio.h>
#include <gh_thing.h>

namespace {
class gh_myscreen {
public:
    gh_myscreen (void) { }
    ~gh_myscreen (void) { }

    void init(void);
    void fini(void);
    void tick(void);
    void display(void);
    int key_down(const SDL_keysym & key);
    int key_up(const SDL_keysym & key);
    int key_held(const SDL_keysym & key);
    int mouse_up(const size_t button);
    int mouse_down(const size_t button);
    int mouse_held(const size_t button);
    int mouse_movement(const gh_point & p, const gh_point & at);
    void reset(void);

    gh_screen myscreen;
    gh_game *game;
    gh_event_ret ret_code;

    vector <string> files;
    vector <string> names;
    string playerdir;
    size_t base;
    size_t at;
    size_t how_many;
    string chose;
};

static gh_myscreen screen;

static string gh_ui_display_player_powerup_choosers (gh_game *game,
                                             vector<string> files,
                                             size_t base,
                                             size_t at,
                                             const size_t how_many)
{
    gh_thing *player_thing;

    player_thing = game->player->thing;

    GH_FUNCTION()

    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    if (!files.size()) {
        font1->printat(2, "No Upgrades!");
        return ("");
    }

    string title = tostring(player_thing->bravery_curr) + " points";
    font1->printat(2, title.c_str());

    vector<string>::iterator f = files.begin();

    f += base;

    int y = GH_TERM_HEIGHT/2 - (int)how_many;
    string ret = "";

    if (base) {
        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_putfmx("^^^");
    }

    while (f != files.end()) {
        char line[200];

        memset(line, 0, sizeof(line));

        string a_filename(files[base]);

        string_replace(a_filename, ".gz", "");
        string_rremove_to_start(a_filename, "things/");

        //
        // Map the filename to a readable name.
        //
        gh_thing *powerup = gh_thing::find(a_filename, true /* error */);

        if (!powerup) {
            GH_THROW("unknown powerup " + a_filename);
        }

        //
        // Bad show to allow purchase of cursed spells...
        //
        if (powerup->is_spell) {
            if (powerup->is_cursed) {
                f++;
                base++;
                continue;
            }
        }

        gh_term_goto(0, y++);

        snprintf(line, sizeof(line) - 1, "%-25s cost %5u",
                 powerup->named().c_str(), powerup->cost);

        if (base == at) {
            ret = a_filename;
            gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLUE);
        } else {
            if (powerup->cost > player_thing->bravery_curr) {
                gh_term_fgbg(GH_TERM_COLOR_GRAY_2, GH_TERM_COLOR_BLACK);
            } else if (powerup->intelligence_req > player_thing->intelligence) {
                gh_term_fgbg(GH_TERM_COLOR_GRAY_2, GH_TERM_COLOR_BLACK);
            } else if (powerup->strength_req > player_thing->_strength) {
                gh_term_fgbg(GH_TERM_COLOR_GRAY_2, GH_TERM_COLOR_BLACK);
            } else if (powerup->dex_req > player_thing->_dex) {
                gh_term_fgbg(GH_TERM_COLOR_GRAY_2, GH_TERM_COLOR_BLACK);
            } else {
                gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
            }
        }

        gh_term_putfmx(line);

        f++;

        if (y >= GH_TERM_HEIGHT/2 + (int)how_many) {
            if (f != files.end()) {
                gh_term_goto(0, y++);
                gh_term_goto(0, y++);

                gh_term_putfmx("%%fg=red%%bg=black--more--");
            }

            return (ret);
        }

        base++;
    }

    if (f != files.end()) {
        gh_term_goto(0, y++);
        gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
        gh_term_putfmx("VVV");
    }

    return (ret);
}

void gh_myscreen::init (void)
{
    GH_FUNCTION()

    game->frame = 0;
}

void gh_myscreen::fini (void)
{
    GH_FUNCTION()
}

void gh_myscreen::tick (void)
{
    GH_FUNCTION()

    game->frame++;
}

void gh_myscreen::display (void)
{
    GH_FUNCTION()

    gh_term_cls();

    at = GH_MIN(at, files.size() - 1);

    gh_ui_intro_border(game);

    chose = gh_ui_display_player_powerup_choosers(game, files, base,
                                                  at, how_many);

    gh_term_goto(0, GH_TERM_HEIGHT - 9);

    gh_term_putfmx(
        "%%fg=yellow%%bg=black"
        "Press "
        "%%fg1=black%%bg1=white"
        "%%fg2=white%%bg2=black"
        "SPACE"
        "%%fg=yellow%%bg=black"
        " to spend bravery points or "
        "%%fg1=black%%bg1=white"
        "%%fg2=white%%bg2=black"
        "ESCAPE");
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    if (sym.sym == ' ') {
        return (true);
    }

    return (key_down(sym));
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    switch (sym.sym) {
    case ' ': {
        if (chose == "") {
            return (true);
        }

        gh_thing *powerup = gh_thing::find(chose, true /* error */);
        gh_thing *player_thing;

        player_thing = game->player->thing;

        if (powerup->cost > player_thing->bravery_curr) {
            gh_audio::play("Button_Off_001");
            return (true);
        }

        player_thing->intelligence += powerup->intelligence;
        player_thing->_metabolism += powerup->metabolism();
        player_thing->_can_see_distance += powerup->can_see_distance();
        player_thing->_stamina += powerup->stamina();
        player_thing->karma += powerup->karma;
        player_thing->health += powerup->health;
        player_thing->orig_health += powerup->orig_health;
        player_thing->keys += powerup->keys;
        player_thing->fate += powerup->fate;
        player_thing->armour += powerup->armour;
        player_thing->_strength += powerup->strength();
        player_thing->_dex += powerup->dex();

        if (powerup->_poisoned) {
            player_thing->_poisoned = 0;
            player_thing->poisoned_boost = 0;
        }

        if (powerup->_stoned) {
            player_thing->_stoned = 0;
            player_thing->stoned_boost = 0;
        }

        if (powerup->is_cursed) {
            player_thing->spell_remove_curse(NULL);
            player_thing->is_cursed = false;
        }

        if (powerup->can_see_cursed_items) {
            player_thing->can_see_cursed_items = false;
        }

        if (powerup->is_thief) {
            player_thing->spell_remove_curse(NULL);
            player_thing->is_thief = false;
        }

        if (powerup->bite_damage) {
            if (player_thing->weapon) {
                player_thing->weapon->bite_damage += powerup->bite_damage;
            } else {
                gh_audio::play("magic_tinkle_cursed");
                return (true);
            }
        }

        if (powerup->charges) {
            if (player_thing->weapon) {
                player_thing->weapon->charges += powerup->charges;
            } else {
                gh_audio::play("magic_tinkle_cursed");
                return (true);
            }
        }

        if (powerup->bite_delay_in_tenths) {
            if (player_thing->bite_delay_in_tenths <= 1) {
                gh_audio::play("magic_tinkle_cursed");
                return (true);
            }

            player_thing->bite_delay_in_tenths += powerup->bite_delay_in_tenths;
        }

        if (powerup->_acid_proof) {
            player_thing->_acid_proof = powerup->_acid_proof;
        }

        if (powerup->_fire_proof) {
            player_thing->_fire_proof = powerup->_fire_proof;
        }

        if (powerup->_water_proof) {
            player_thing->_water_proof = powerup->_water_proof;
        }

        if (powerup->_cold_proof) {
            player_thing->_cold_proof = powerup->_cold_proof;
        }

        if (powerup->_suffocation_proof) {
            player_thing->_suffocation_proof = powerup->_suffocation_proof;
        }

        if (powerup->_teleport_proof) {
            player_thing->_teleport_proof = powerup->_teleport_proof;
        }

        if (powerup->_poison_proof) {
            player_thing->_poison_proof = powerup->_poison_proof;
        }

        if (powerup->_stone_proof) {
            player_thing->_stone_proof = powerup->_stone_proof;
        }

        if (powerup->tireless) {
            player_thing->tireless = powerup->tireless;
        }

        if (powerup->tireless_swing) {
            player_thing->tireless_swing = powerup->tireless_swing;
        }

        if (powerup->ability_can_regenerate1) {
            player_thing->ability_can_regenerate1 = powerup->ability_can_regenerate1;
        }

        if (powerup->ability_can_regenerate2) {
            player_thing->ability_can_regenerate1 = powerup->ability_can_regenerate2;
            player_thing->ability_can_regenerate2 = powerup->ability_can_regenerate2;
        }

        if (powerup->ability_can_regenerate3) {
            player_thing->ability_can_regenerate1 = powerup->ability_can_regenerate3;
            player_thing->ability_can_regenerate2 = powerup->ability_can_regenerate3;
            player_thing->ability_can_regenerate3 = powerup->ability_can_regenerate3;
        }

        if (powerup->can_self_teleport) {
            player_thing->can_self_teleport = powerup->can_self_teleport;
            player_thing->can_self_teleport_once_per_level = false;
        }

        if (powerup->ability_can_rage) {
            player_thing->ability_can_rage = powerup->ability_can_rage;
        }

        if (powerup->ability_can_auto_rage) {
            player_thing->ability_can_auto_rage = powerup->ability_can_auto_rage;
        }

        if (powerup->can_self_teleport_once_per_level) {
            player_thing->can_self_teleport_once_per_level =
                            powerup->can_self_teleport_once_per_level;
        }

        if (powerup->is_spell) {
            gh_thing *item;

            item = new gh_thing(player_thing->level,
                                powerup->short_name,
                                player_thing->at);

            if (!player_thing->collect_item(item)) {
                player_thing->bravery_curr += powerup->cost;
                item->decref(GH_THING_REF_FIRST);
                item->dead("failed to buy");
            } else {
                item->decref(GH_THING_REF_FIRST);
            }
        }

        player_thing->bravery_curr -= powerup->cost;
        gh_audio::play("magic_tinkle", 10);

        if (player_thing->bravery_curr < 500) {
            gh_global::done = true;
            return (true);
        }

        return (true);
    }

    case SDLK_LEFT:
    case SDLK_ESCAPE:
        gh_audio::play("Button_Off_001");

        gh_global::done = true;
        return (true);

    case SDLK_DOWN:
    case 'j':
        if (at < files.size() - 1) {
            at++;
            if (at > base + how_many - 1) {
                base++;
            }
        }
        break;

    case SDLK_UP:
    case 'k':
        if (at) {
            at--;
            if (at < base) {
                base--;
            }
        }
        break;

    case SDLK_PAGEDOWN:
        for (uint32 i = 0; i < how_many; i++) {
            if (at < files.size() - 1) {
                at++;
                if (at > base + how_many - 1) {
                    base++;
                }
            }
        }
        break;

    case SDLK_PAGEUP:
        for (uint32 i = 0; i < how_many; i++) {
            if (at) {
                at--;
                if (at < base) {
                    base--;
                }
            }
        }
        break;

    default:
        break;
    }

    return (true);
}

int gh_myscreen::key_up (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_up (const size_t button)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_down (const size_t button)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_held (const size_t button)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::mouse_movement (const gh_point & rel, const gh_point &at)
{
    GH_FUNCTION()

    gh_fpoint f_at(
                  ((float)at.x / (float)gh_screen::vid.w),
            1.0 - ((float)at.y / (float)gh_screen::vid.h));

    gh_fpoint pixel_at(
            (int)(f_at.x * gh_screen::vid.w),
            (int)(f_at.y * gh_screen::vid.h));

    return (true);
}

void gh_myscreen::reset (void)
{
    GH_FUNCTION()
}
};

gh_event_ret gh_ui_new_player_powerup_chooser_loop (gh_game *game)
{
    static bool first = true;

    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;

    if (first) {
        first = false;
        screen.base = 0;
        screen.at = 0;
    }

    screen.how_many = 12;
    screen.chose = "";

    //
    // Exclude .map files
    //
    screen.playerdir = gh_global::locatedir("things/powerups");
    screen.files = gh_global::listdir_recursive(screen.playerdir, "", ".map");

    screen.playerdir = gh_global::locatedir("things/magic/spell");
    vector<string> files = gh_global::listdir_recursive(screen.playerdir, "", ".map");

    copy(files.begin(), files.end(),
        back_insert_iterator<vector <string> > (screen.files));

    screen.myscreen.name = __FILE__;
    screen.myscreen.classbase = &screen;
    screen.myscreen.priority = 210;

    screen.myscreen.init =
        (gh_screen::gh_screen_init)&gh_myscreen::init;
    screen.myscreen.fini =
        (gh_screen::gh_screen_fini)&gh_myscreen::fini;
    screen.myscreen.tick =
        (gh_screen::gh_screen_tick)&gh_myscreen::tick;
    screen.myscreen.display =
        (gh_screen::gh_screen_display)&gh_myscreen::display;
    screen.myscreen.reset =
        (gh_screen::gh_screen_reset)&gh_myscreen::reset;
    screen.myscreen.key_down =
        (gh_screen::gh_screen_key_down)&gh_myscreen::key_down;
    screen.myscreen.key_held =
        (gh_screen::gh_screen_key_held)&gh_myscreen::key_held;
    screen.myscreen.mouse_up =
        (gh_screen::gh_screen_mouse_up)&gh_myscreen::mouse_up;
    screen.myscreen.mouse_down =
        (gh_screen::gh_screen_mouse_down)&gh_myscreen::mouse_down;
    screen.myscreen.mouse_held =
        (gh_screen::gh_screen_mouse_held)&gh_myscreen::mouse_held;
    screen.myscreen.mouse_movement =
        (gh_screen::gh_screen_mouse_movement)&gh_myscreen::mouse_movement;

    screen.myscreen.show();

    screen.myscreen.auto_repeat_rate = 1;
    screen.myscreen.auto_repeat_delay = 20;

    gh_term_cls();

    gh_global::done = false;

    sdl_loop();

    gh_global::done = false;

    screen.myscreen.hide();

    return (screen.ret_code);
}
