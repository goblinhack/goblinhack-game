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

    $Id: level_chooser.cpp 1241 2009-07-10 21:32:09Z Ghack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_ui_event.h>
#include <gh_figlet.h>
#include <gh_level.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_screen.h>
#include <gh_sdl.h>
#include <gh_audio.h>

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

    string gh_ui_display_level_choosers(gh_game *game,
                                        vector<string> listing,
                                        size_t base,
                                        size_t at,
                                        const size_t how_many);
    gh_screen myscreen;
    gh_game *game;
    gh_event_ret ret_code;

    vector <string> listing;
    string dir;
    size_t base;
    size_t at;
    size_t how_many;
    string chose;
    bool bottom_level;
    bool read_input;
};

static gh_myscreen screen;

string gh_myscreen::gh_ui_display_level_choosers (gh_game *game,
                                                  vector<string> listing,
                                                  size_t base,
                                                  size_t at,
                                                  const size_t how_many)
{
    GH_FUNCTION()

    gh_term_goto(0, 0);
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_fill_line(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_puts(("Current directory: " + dir).c_str());

    gh_term_goto(0, GH_TERM_HEIGHT - 1);
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_fill_line(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);

    if (bottom_level) {
        gh_term_puts("Select a level of press 'n' to create a new file.");
    } else {
        gh_term_puts("Select a level directory.");
    }

    gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);

    vector<string>::iterator f = listing.begin();

    //
    // First see if we are at the bottom level where we are allowed to
    // make levels.
    //
    f = listing.begin();
    screen.bottom_level = true;
    while (f != listing.end()) {
        string file_or_dir = *f;

        if (file_or_dir[file_or_dir.size() -1] == '/') {
            screen.bottom_level = false;
        }

        f++;
    }

    //
    // Now just list what we can see in this section of the listing.
    //
    f = listing.begin();
    f += base;

    int y = 3;
    string ret = "";

    f = listing.begin();
    while (f != listing.end()) {
        char line[40];

        memset(line, 0, sizeof(line));

        gh_term_goto(0, y++);

        snprintf(line, sizeof(line) - 1, "%*s", (int)(sizeof(line) - 2),
                 listing[base].c_str());

        if (base == at) {
            ret = listing[base];
            gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
        } else {
            gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        }

        gh_term_putfmx(line);

        f++;

        if (y >= GH_TERM_HEIGHT/2 + (int)how_many) {
            if (f != listing.end()) {
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

static void gh_myscreen_finished_reading_input (void *context, string result)
{
    gh_myscreen *screen = (gh_myscreen*)context;

    screen->read_input = false;
    screen->chose = screen->dir + result;

    vector<string> level_name = string_split(screen->chose, '/');
    int tuples = level_name.size();

    if (tuples >= 3) {
        screen->game->level_set_name = level_name[tuples - 3];
        screen->game->level_no = toint(level_name[tuples - 2]);
        screen->game->which_level = level_name[tuples - 1];

        string_replace(screen->game->which_level, ".gz", "");

        screen->ret_code = GH_UI_EVENT_CONTINUE;

        gh_global::done = true;
    }
}

void gh_myscreen::display (void)
{
    GH_FUNCTION()

    gh_term_cls();

    at = GH_MIN(at, listing.size() - 1);

    gh_ui_intro_border(game);

    chose = gh_ui_display_level_choosers(game, listing, base, at, how_many);

    if (read_input) {
        game->display_messages();
    }
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    switch (sym.sym) {
    case ' ':
    case SDLK_RETURN:
    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_ESCAPE:
        return (true);

    default:
        return (key_down(sym));
    }
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    switch (sym.sym) {
    case 'n':
        if (bottom_level) {
            gh_audio::play("Keyboard_Hit_001");

            if (!read_input) {
                read_input = true;

                game->replace_last_message("Enter a file name: ");

                gh_ui_player_new_read_small_text_loop(
                            game,
                            this,
                            gh_myscreen_finished_reading_input);
            }
        }
        return (true);

    case ' ':
    case SDLK_RETURN:
        gh_audio::play("Keyboard_Hit_001");

        if (chose == "") {
            ret_code = GH_UI_EVENT_CONTINUE;
            gh_global::done = true;
            return (true);
        }

        dir += chose;

        if (gh_global::file_exists(dir)) {
            vector<string> level_name = string_split(dir, '/');
            int tuples = level_name.size();

            if (tuples >= 3) {
                game->level_set_name = level_name[tuples - 3];
                game->level_no = toint(level_name[tuples - 2]);
                game->which_level = level_name[tuples - 1];

                string_replace(game->which_level, ".gz", "");

                ret_code = GH_UI_EVENT_CONTINUE;
                gh_global::done = true;
                return (true);
            }
        }

        listing = gh_global::listdir(dir);

        base = 0;
        at = 0;
        break;


    case SDLK_LEFT:
    case SDLK_ESCAPE:
        //
        // Don't allow the user to leave the resources dir
        //
        if (!bottom_level) {
            ret_code = GH_UI_EVENT_ESCAPE;
            gh_global::done = true;
            return (true);
        }

        listing = gh_global::listdir(dir);

        //
        // Remove the trailing / and then the last dir name
        //
        string_rremove_to_end(dir, '/');
        string_rremove_to_end(dir, '/');
        dir += "/";

        listing = gh_global::listdir(dir);

        if (!listing.size()) {
            ret_code = GH_UI_EVENT_CONTINUE;
            gh_global::done = true;
            return (true);
        }

        base = 0;
        at = 0;
        break;

    case SDLK_DOWN:
    case 'j':
        gh_audio::play("Keyboard_Hit_001");

        if (at < listing.size() - 1) {
            at++;
            if (at > base + how_many - 1) {
                base++;
            }
        }
        break;

    case SDLK_UP:
    case 'k':
        gh_audio::play("Keyboard_Hit_001");

        if (at) {
            at--;
            if (at < base) {
                base--;
            }
        }
        break;

    case SDLK_PAGEDOWN:
        gh_audio::play("Keyboard_Hit_001");

        for (uint32 i = 0; i < how_many; i++) {
            if (at < listing.size() - 1) {
                at++;
                if (at > base + how_many - 1) {
                    base++;
                }
            }
        }
        break;

    case SDLK_PAGEUP:
        gh_audio::play("Keyboard_Hit_001");

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

    return (false);
}

void gh_myscreen::reset (void)
{
    GH_FUNCTION()
}
};

gh_event_ret gh_ui_new_level_chooser_loop (gh_game *game)
{
    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;
    screen.dir = gh_global::locatedir("levels") + "random/";
    screen.listing = gh_global::listdir(screen.dir);
    screen.base = 0;
    screen.at = 0;
    screen.how_many = 15;
    screen.chose = "";
    screen.bottom_level = false;
    screen.read_input = false;

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

    return (screen.ret_code);
}
