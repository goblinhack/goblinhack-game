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

    $Id: level_type_chooser.cpp 276 2006-11-20 21:16:05Z goblinhack $
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

    vector <string> levels;
    vector <string> names;
    string leveldir;
    size_t base;
    size_t at;
    size_t how_many;
    string chose;
};

static gh_myscreen screen;

static string gh_ui_display_level_type_choosers (gh_game *game,
                                             vector<string> levels,
                                             size_t base,
                                             size_t at,
                                             const size_t how_many)
{
    GH_FUNCTION()

    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    if (!levels.size()) {
        font1->printat(2, "No Levels!");
        return ("");
    }

    font1->printat(2, "Choose Thy Dungeon!");

    levels = reverse(levels);

    vector<string>::iterator a_level = levels.begin();

    a_level += base;

    int y = GH_TERM_HEIGHT/2 - (int)how_many;
    string ret = "";

    while (a_level != levels.end()) {
        char line[80];

        memset(line, 0, sizeof(line));

        string a_filename(levels[base]);

        string_rremove_to_start(a_filename, "/");
        string_replace(a_filename, ".gz", "");

        gh_term_goto(0, y++);
        gh_term_goto(0, y++);

        string this_level = *a_level;
        string level_alias;

        if (this_level == "random/") {
            level_alias = "The Goblin Halls";
        } else if (this_level == "Tutorial/") {
            level_alias = "Tutorial Level (work in progress)";
        } else {
            level_alias = "Unnamed mystery level";
        }

        //
        // Map the filename to a readable name.
        //
        snprintf(line, sizeof(line) - 1, "%*s", (int)(sizeof(line) - 2),
                 level_alias.c_str());

        if (base == at) {
            ret = levels[base];
            gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_BLUE);
        } else {
            gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        }

        gh_term_putfmx(line);

        a_level++;

        if (y >= GH_TERM_HEIGHT/2 + (int)how_many) {
            if (a_level != levels.end()) {
                gh_term_goto(0, y++);
                gh_term_goto(0, y++);

                gh_term_putfmx("%%fg=red%%bg=black--more--");
            }

            return (ret);
        }

        base++;
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

    at = GH_MIN(at, levels.size() - 1);

    gh_ui_intro_border(game);

    chose = gh_ui_display_level_type_choosers(game, levels, base, at, how_many);

    gh_term_goto(0, GH_TERM_HEIGHT - 3);

    gh_term_putfmx(
        "%%fg=yellow%%bg=black"
        "Press "
        "%%fg1=black%%bg1=white"
        "%%fg2=white%%bg2=black"
        "SPACE"
        "%%fg=yellow%%bg=black"
        " to choose your method of demise");
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    if (sym.sym == ' ') {
        return (false);
    }

    return (key_down(sym));
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    switch (sym.sym) {
    case ' ':
    case SDLK_RETURN:
        if (chose == "") {
            ret_code = GH_UI_EVENT_CONTINUE;
            gh_global::done = true;
            return (true);
        }

        game->level_set_name = chose;
        ret_code = GH_UI_EVENT_CONTINUE;
        gh_global::done = true;
        return (true);

    case SDLK_LEFT:
    case SDLK_ESCAPE:
        gh_audio::play("Button_Off_001");

        ret_code = GH_UI_EVENT_ESCAPE;
        gh_global::done = true;
        return (true);

    case SDLK_DOWN:
    case 'j':
        if (at < levels.size() - 1) {
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

    return (false);
}

void gh_myscreen::reset (void)
{
    GH_FUNCTION()
}
};

gh_event_ret gh_ui_new_level_type_chooser_loop (gh_game *game)
{
    static bool first = true;

    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;
    screen.leveldir = gh_global::locatedir("resources/levels");

    if (first) {
        first = false;
        screen.base = 0;
        screen.at = 0;
    }

    screen.how_many = 14;
    screen.chose = "";

    //
    // Exclude .map levels
    //
    screen.levels = gh_global::list_dirs_only(screen.leveldir);

    screen.myscreen.name = __FILE__;
    screen.myscreen.classbase = &screen;
    screen.myscreen.priority = 100;

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

    //
    // If nothing chose, default is always the dungeons of doom
    //
    if (game->level_set_name == "") {
        game->level_set_name = "random";
    }

    return (screen.ret_code);
}
