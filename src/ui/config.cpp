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

    $Id: config.cpp 1228 2009-06-29 02:08:02Z Ghack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_ui_event.h>
#include <gh_figlet.h>
#include <gh_level.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_music.h>
#include <gh_screen.h>
#include <gh_sdl.h>
#include <gh_audio.h>
#include <gh_templates.h>

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

    typedef bool (*gh_myscreen_callback)(class gh_myscreen *, void *arg);

    int gh_ui_display_configs (gh_game *game,
                               size_t base,
                               size_t at);
    gh_screen myscreen;
    gh_game *game;
    gh_event_ret ret_code;

    vector <string> options;
    vector <string> values;
    vector <gh_myscreen_callback> callbacks;

    size_t base;
    size_t at;
    int chose;

    gh_size screensize;
    bool fullscreen;
    bool noframe;
};

bool gh_myscreen_change_resolution (gh_myscreen *myscreen,
                                    void *arg)
{
    vector<gh_size> arr;

    arr.push_back(gh_global::default_screensize);

    push_back_if_unique(arr, gh_size(640,480));
    push_back_if_unique(arr, gh_size(800,600));
    push_back_if_unique(arr, gh_size(1024,800));
    push_back_if_unique(arr, gh_size(1280,800));
    push_back_if_unique(arr, gh_size(1280,1024));

    static int index;

    myscreen->screensize = arr[++index % arr.size()];

    return (true);
}

bool gh_myscreen_change_fullscreen (gh_myscreen *myscreen, void *arg)
{
    myscreen->fullscreen = !myscreen->fullscreen;

    return (true);
}

bool gh_myscreen_change_noframe (gh_myscreen *myscreen, void *arg)
{
    myscreen->noframe = !myscreen->noframe;

    return (true);
}

bool gh_myscreen_change_music (gh_myscreen *myscreen, void *arg)
{
    gh_global::music = !gh_global::music;

    if (gh_global::music) {
        myscreen->game->new_message("Music on", true /* unique */);
        gh_music::play("HackYourWay");
    } else {
        myscreen->game->new_message("Music off", true /* unique */);
        gh_music::halt();
    }

    return (true);
}

bool gh_myscreen_change_sound (gh_myscreen *myscreen, void *arg)
{
    gh_global::sound = !gh_global::sound;

    return (true);
}

static gh_myscreen screen;

int gh_myscreen::gh_ui_display_configs (gh_game *game,
                                        size_t base,
                                        size_t at)
{
    GH_FUNCTION()

    gh_term_goto(0, 0);
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_fill_line(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_puts("Configure Ghack:");

    gh_term_goto(0, GH_TERM_HEIGHT - 1);
    gh_term_fgbg(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_fill_line(GH_TERM_COLOR_YELLOW, GH_TERM_COLOR_BLUE);
    gh_term_puts("Use space/enter to select");

    gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);

    int y = 3;
    int ret = -1;

    vector<string>::iterator f = options.begin();
    vector<string>::iterator g = values.begin();

    while (f != options.end()) {
        char line[40];

        memset(line, 0, sizeof(line));

        gh_term_goto(0, y++);

        snprintf(line, sizeof(line) - 1, "%-*s %*s",
                 (int)(sizeof(line)/2 - 2),
                 options[base].c_str(),
                 (int)(sizeof(line)/2 - 2),
                 values[base].c_str());

        if (base == at) {
            ret = base;

            gh_term_fgbg(GH_TERM_COLOR_WHITE, GH_TERM_COLOR_RED);
        } else {
            gh_term_fgbg(GH_TERM_COLOR_CYAN, GH_TERM_COLOR_BLACK);
        }

        gh_term_putfmx(line);

        f++;

        if (y >= GH_TERM_HEIGHT/2 + (int)options.size()) {
            if (f != options.end()) {
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

    gh_ui_intro_border(game);

    erase(options);
    erase(values);

    options.push_back("Screen resolution");
    values.push_back(tostring(screensize.w) + "x" +
                     tostring(screensize.h));

    callbacks.push_back(gh_myscreen_change_resolution);

    options.push_back("Fullscreen");
    if (fullscreen) {
        values.push_back("On");
    } else {
        values.push_back("Off");
    }
    callbacks.push_back(gh_myscreen_change_fullscreen);

    options.push_back("Window border");
    if (!fullscreen) {
        if (noframe) {
            values.push_back("Off");
        } else {
            values.push_back("On");
        }
        callbacks.push_back(gh_myscreen_change_noframe);
    } else {
        values.push_back("N/A");
    }

    options.push_back("Music");
    if (gh_global::music) {
        values.push_back("On");
    } else {
        values.push_back("Off");
    }
    callbacks.push_back(gh_myscreen_change_music);

    options.push_back("Sound");
    if (gh_global::sound) {
        values.push_back("On");
    } else {
        values.push_back("Off");
    }
    callbacks.push_back(gh_myscreen_change_sound);

    at = GH_MIN(at, options.size() - 1);

    chose = gh_ui_display_configs(game, base, at);
}

int gh_myscreen::key_held (const SDL_keysym & sym)
{
    switch (sym.sym) {
    case ' ':
    case SDLK_RETURN:
    case SDLK_ESCAPE:
        return (true);

    default:
        return (key_down(sym));
    }
}

int gh_myscreen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    gh_audio::play("Keyboard_Hit_001");

    switch (sym.sym) {
    case ' ':
    case SDLK_RETURN:
        if (chose == -1) {
            ret_code = GH_UI_EVENT_CONTINUE;
            gh_global::done = true;
            return (true);
        }

        (void) (*callbacks[chose])(this, 0);
        break;

    case SDLK_ESCAPE:
        gh_audio::play("Button_Off_001");

        ret_code = GH_UI_EVENT_CONTINUE;
        gh_global::done = true;

        gh_global::config_changed = (fullscreen != gh_global::fullscreen);

        if (noframe != gh_global::noframe) {
            gh_global::config_changed = true;
        }
                        
        if (screensize != gh_global::screensize) {
            gh_global::config_changed = true;
        }

        if (gh_global::config_changed) {
            gh_global::fullscreen = fullscreen;
            gh_global::noframe = noframe;
            gh_global::screensize = screensize;
        }

        return (true);

    case SDLK_DOWN:
    case 'j':
        if (at < options.size() - 1) {
            at++;
            if (at > base + options.size() - 1) {
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
        for (uint32 i = 0; i < options.size(); i++) {
            if (at < options.size() - 1) {
                at++;
                if (at > base + options.size() - 1) {
                    base++;
                }
            }
        }
        break;

    case SDLK_PAGEUP:
        for (uint32 i = 0; i < options.size(); i++) {
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

gh_event_ret gh_ui_new_config_loop (gh_game *game)
{
    GH_FUNCTION()

    screen.game = game;
    screen.ret_code = GH_UI_EVENT_NONE;

    screen.base = 0;
    screen.at = 0;
    screen.chose = -1;

    screen.myscreen.name = __FILE__;
    screen.myscreen.classbase = &screen;
    screen.myscreen.priority = 100;
    screen.fullscreen = gh_global::fullscreen;
    screen.noframe = gh_global::noframe;
    screen.screensize = gh_global::screensize;

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
