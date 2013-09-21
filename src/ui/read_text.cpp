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

    $Id: read_text.cpp 674 2008-01-23 02:57:47Z goblinhack $
*/

#include <ghack.h>
#include <gh_subsys.h>
#include <gh_term.h>
#include <gh_ui_event.h>
#include <gh_figlet.h>
#include <gh_level.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_sdl.h>
#include <gh_sdl_event.h>
#include <gh_ui_event.h>
#include <gh_screen.h>
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

    gh_screen myscreen;
    gh_game *game;
    gh_event_ret ret_code;
    string result;
    string what;
    size_t cx;
};

static gh_myscreen screen;

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

    int my = GH_TERM_HEIGHT / 2;

    gh_term_fgbg(GH_TERM_COLOR_RED, GH_TERM_COLOR_BLACK);
    font1->printat(my + 4, what.c_str());

    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLACK);

    string n = result;
    n.insert(cx, "_");

    font1->printat(my + 10, n.c_str());
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (true);
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_term_cls();

    gh_audio::play("Keyboard_Hit_001");

    switch (sym.sym) {
        case SDLK_BACKSPACE:
            if (cx) {
                result.erase(cx-1, 1);
                cx--;
            }
            break;

        case SDLK_RETURN:
            gh_global::done = true;
            break;

        case SDLK_LEFT:
            if (cx) {
                cx--;
            }
            break;

        case SDLK_RIGHT:
            if (cx < result.size()) {
                cx++;
            }
            break;

        case SDLK_HOME:
            cx = 0;
            break;

        case SDLK_END:
            cx = result.size();
            break;

        default: {
            char c = sdl_evt_to_char(sym);

            //
            // No space padding at the start of names.
            //
            if (c == ' ') {
                if (!result.size()) {
                    return (true);
                }
            }

            if (c != '\0') {
                if (result.size() < 20) {
                    string s = "";
                    s += (char) c;
                    result.insert(cx, s);
                    cx++;
                }
                return (true);
            }
            break;
        }
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

string gh_ui_player_new_read_text_loop (gh_game *game, string what)
{
    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;
    screen.result = "";
    screen.what = what;
    screen.cx = 0;

    screen.myscreen.name = __FILE__;
    screen.myscreen.classbase = &screen;
    screen.myscreen.priority = 200;

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

    gh_global::done = false;

    sdl_loop();

    gh_global::done = false;

    screen.myscreen.hide();

    return (screen.result);
}
