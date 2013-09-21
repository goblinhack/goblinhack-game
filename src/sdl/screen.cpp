/*
    retreat.
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

    $Id: screen.cpp 1240 2009-07-10 02:20:51Z Ghack $
*/

#include <ghack.h>
#include <gh_global.h>
#include <gh_screen.h>
#include <gh_templates.h>

gh_size gh_screen::vid;
vector<class gh_screen *> gh_screen::screens;
vector<class gh_screen *> gh_screen::all_screens;

#define MY_CALLBACK(CLASS, FN)                              \
        (((gh_screen*)CLASS->classbase)->*(CLASS->FN))      \

typedef struct {
    SDL_keysym sym;
    int held:1;
    uint32 count;
} SDL_keysym_count;

static vector<SDL_keysym> key_history_down;
static vector<SDL_keysym> key_history_up;
static vector<SDL_keysym_count *> key_history_held;
static vector<SDL_Event> mouse_history;

static SDL_keysym_count heldkey[4];
static uint32 mouse_held_down[10];
static uint32 mouse_held_count;
static const SDL_keysym_count * eo_heldkey = heldkey + GH_ARRAY_SIZE(heldkey);

static void key_down(SDL_keysym const * const key);
static void key_up(SDL_keysym const * const key);

//
// Lower is faster for both
//
const uint32 gh_screen::auto_repeat_rate_default = 1;
const uint32 gh_screen::auto_repeat_delay_default = 5;

gh_screen::gh_screen (void)
{
    GH_FUNCTION()

    name = "unnamed";
    ticks = 0;
    active = true;
    init_done = fini_done = false;
    visible = false;

    auto_repeat_rate = gh_screen::auto_repeat_rate_default;
    auto_repeat_delay = gh_screen::auto_repeat_delay_default;

    classbase = 0;
    init = 0;
    shown = 0;
    hidden = 0;
    fini = 0;
    tick = 0;
    display = 0;
    reset = 0;
    key_down = 0;
    key_up = 0;
    key_held = 0;
    mouse_up = 0;
    mouse_down = 0;
    mouse_held = 0;
    mouse_movement = 0;
    priority = 0;

    wake();
}

gh_screen::~gh_screen (void)
{
    GH_FUNCTION()

    kill();

    erase_all(key_history_down);
    erase_all(key_history_up);
    erase_all(key_history_held);
    erase_all(mouse_history);

    memset(heldkey, 0, sizeof(heldkey));
    memset(mouse_held_down, 0, sizeof(mouse_held_down));
    mouse_held_count = 0;

    //
    // Some odd crash at exit
    //
    if (gh_global::done) {
        return;
    }

    if (visible) {
        hide();
    }

    if (fini_done) {
        return;
    }

    if (!fini) {
        return;
    }

//    GH_LOG("Screen finished " + name);

    fini_done = true;

    MY_CALLBACK(this, fini)();
}

static void init_screen (gh_screen * screen)
{
    GH_FUNCTION()

    if (screen->init_done) {
        return;
    }

    if (!screen->init) {
        return;
    }

//    GH_LOG("Screen init " + screen->name);

    screen->init_done = true;

    MY_CALLBACK(screen, init)();
}

static int FilterEvents (const SDL_Event *event)
{
    GH_FUNCTION()

    switch (event->type) {
        /* This is important!  Queue it if we want to quit. */
        case SDL_QUIT:
            return(1);

        /* Mouse and keyboard events go to threads */
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            return(1);

        /* Drop all other events */
        default:
            return(0);
    }
}

static void read_events (void)
{
    GH_FUNCTION()

    SDL_keysym_count * hkey;
    static SDL_Event events[10];
    int found;
    SDL_Event * e;

        SDL_Delay(6);

    found = SDL_PeepEvents(events, GH_ARRAY_SIZE(events), SDL_GETEVENT,
                    SDL_MOUSEMOTIONMASK|
                    SDL_MOUSEBUTTONDOWNMASK|
                    SDL_MOUSEBUTTONUPMASK|
                    SDL_KEYDOWNMASK|
                    SDL_KEYUPMASK);

    //
    // Update the count of held mouse buttons, and for how long
    //
    mouse_held_count = 0;

    for (uint32 i=0;i<GH_ARRAY_SIZE(mouse_held_down);i++) {
        if (mouse_held_down[i]) {
            mouse_held_down[i]++;
            mouse_held_count++;
        }
    }

    //
    // Copy all currently held keys to the history queue if the queue
    // has been drained.
    //
    if (key_history_held.empty()) {
        hkey = heldkey;

        while (hkey < eo_heldkey) {
            if (hkey->sym.sym) {
                key_history_held.push_back(hkey);
            }
            hkey++;
        }
    }

    //
    // Update held count of keys
    //
    vector<SDL_keysym_count *>::iterator i;

    for (i = key_history_held.begin(); i != key_history_held.end(); i++) {
        if ((*i)->held) {
            (*i)->count++;
        }
    }

    e = events;

    while (found-- > 0) {

        switch (e->type) {
            default: {

                //
                // Copy all currently held mouse to the history queue
                //
                mouse_history.push_back(*e);

                uint32 sdl_button = e->button.button;

                if (sdl_button >= GH_ARRAY_SIZE(mouse_held_down)) {
                    GH_WARN("unhandled SDL button " + tostring(sdl_button));
                    break;
                }

                if (e->type == SDL_MOUSEBUTTONDOWN) {
                    mouse_held_down[sdl_button] = true;
                } else if (e->type ==  SDL_MOUSEBUTTONUP) {
                    mouse_held_down[sdl_button] = false;
                }
                break;
            }

            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_keysym * key = &e->key.keysym;
                SDLMod mod = key->mod;

                //
                // If the numpad is screwing with us picking up events
                // like shift key presses, filter it out
                //
                if (mod & KMOD_NUM) {
                    mod = key->mod = (SDLMod)
                        ((uint32)mod & (uint32)~KMOD_NUM);
                }

                if (key->sym == SDLK_LALT) {
                    continue;
                }

                if (key->sym == SDLK_RALT) {
                    continue;
                }

                if (e->key.state == SDL_PRESSED) {
                    if (mod & KMOD_SHIFT) {
                        if (key->sym == SDLK_F12) {
                            gh_global::done = true;
                            GH_LOG("");
                            GH_LOG("*** Hard Quit ***");
                            GH_LOG("");
                            e++;
                            SDL_Quit();
                            exit(1);
                        } else if (key->sym == SDLK_F11) {
                            gh_global::dump = true;
                            e++;
                            continue;
                        } else if (key->sym == SDLK_F10) {
                            gh_global::hack = !gh_global::hack;
                            cout << "hack " << gh_global::hack << endl;
                            e++;
                            continue;
                        }
                    } else if (mod & KMOD_ALT) {
                        if (key->sym == SDLK_f) {
                            gh_global::fps = !gh_global::fps;
                            e++;
                            continue;
                        } else if (key->sym == SDLK_d) {
                            gh_global::debug = !gh_global::debug;
                            e++;
                            continue;
                        } else if (key->sym == SDLK_t) {
                            gh_global::trace_mode = !gh_global::trace_mode;
                            e++;
                            continue;
                        } else if (key->sym == SDLK_TAB) {
                            //
                            // Don't allow alt tab to break pause!
                            //
                            e++;
                            continue;
                        }
                    } else if (mod & KMOD_CTRL) {
                        if (key->sym == SDLK_TAB) {
                            gh_global::mouse_visible =
                                    !gh_global::mouse_visible;
                            SDL_ShowCursor(gh_global::mouse_visible);

                            if (gh_global::mouse_visible == false) {
                                SDL_WM_GrabInput(SDL_GRAB_ON);
                            } else {
                                SDL_WM_GrabInput(SDL_GRAB_OFF);
                            }
                            e++;
                            continue;
                        }
                    }
                    key_down(key);
                } else {
                    key_up(key);
                }
                break;
            }
        }
        e++;
    }
}

void gh_screen::start_threads (void)
{
    GH_FUNCTION()

    SDL_SetEventFilter(FilterEvents);
}

void gh_screen::exit (void)
{
    GH_FUNCTION()
}

static bool compare_screens (const gh_screen *a, const gh_screen *b)
{
    GH_FUNCTION()

    return (a->priority < b->priority);
}

void gh_screen::hide (void)
{
    GH_FUNCTION()

//    GH_LOG("Screen hide " + name);

    visible = false;

    vector<gh_screen *>::iterator i =
        find(screens.begin(), screens.end(), this);

    if (i == screens.end()) {
        return;
    }

    screens.erase(i);

    if (hidden) {
        MY_CALLBACK(this, hidden)();
    }
}

void gh_screen::show (void)
{
    GH_FUNCTION()

//    GH_LOG("Screen show " + name);

    visible = true;

    vector<gh_screen *>::iterator i =
        find(screens.begin(), screens.end(), this);

    if (i != screens.end()) {
        return;
    }

    screens.push_back(this);

    if (screens.size() > 1) {
        sort(screens.begin(), screens.end(), compare_screens);
    }

    init_screen(this);

    if (shown) {
        MY_CALLBACK(this, shown)();
    }
}

void gh_screen::kill (void)
{
    GH_FUNCTION()

    vector<gh_screen *>::iterator i =
        find(all_screens.begin(), all_screens.end(), this);

    if (i == all_screens.end()) {
        return;
    }

    all_screens.erase(i);
}

void gh_screen::wake (void)
{
    GH_FUNCTION()

    vector<gh_screen *>::iterator i =
        find(all_screens.begin(), all_screens.end(), this);

    if (i != all_screens.end()) {
        GH_THROW("already awake: " + name);
        return;
    }

    all_screens.push_back(this);

    if (all_screens.size() > 1) {
        sort(all_screens.begin(), all_screens.end(), compare_screens);
    }
}

void gh_screen::init_all (void)
{
    GH_FUNCTION()

    vector<class gh_screen *>::iterator i = screens.begin();

    while (i != screens.end()) {
        gh_screen *s = *i++;

        if (s->init) {
            MY_CALLBACK(s, init)();
        }
    }
}

void gh_screen::tick_all (void)
{
    GH_FUNCTION()

    vector<class gh_screen *>::iterator i = screens.begin();

    while (i != screens.end()) {
        gh_screen *s = *i++;

        if (s->tick) {
            MY_CALLBACK(s, tick)();
        }
    }
}

void gh_screen::display_all (void)
{
    GH_FUNCTION()

    gh_screen::vid.w = SDL_GetVideoSurface()->w;
    gh_screen::vid.h = SDL_GetVideoSurface()->h;

    vector<class gh_screen *>::iterator i = screens.begin();

//cout << "display all " << endl;
    while (i != screens.end()) {
        gh_screen *s = *i++;

        if (s->display) {
            MY_CALLBACK(s, display)();
        }
    }
}

void gh_screen::reset_all (void)
{
    GH_FUNCTION()

    gh_surface::reload_all();

    vector<class gh_screen *>::iterator i = screens.begin();

    while (i != screens.end()) {
        gh_screen *s = *i++;

        if (s->reset) {
            GH_LOG("Screen reset " + s->name);

            s->ticks = 0;

            MY_CALLBACK(s, reset)();
        }
    }
}

static void key_down (SDL_keysym const * const key)
{
    GH_FUNCTION()

    SDL_keysym_count * hkey;

    hkey = heldkey;

    key_history_down.push_back(*key);

//cout << "key_down " << SDL_GetKeyName(key->sym) << " mod " << key->mod 
//<<endl;

    while (hkey < eo_heldkey) {
        if (!hkey->sym.sym) {
            hkey->count = 0;
            hkey->held = true;
            hkey->sym = *key;
            break;
        }
        hkey++;
    }
}

static void key_up (SDL_keysym const * const key)
{
    GH_FUNCTION()

    SDL_keysym_count * hkey = heldkey;

    key_history_up.push_back(*key);

    while (hkey < eo_heldkey) {
        if (hkey->sym.sym == key->sym) {

            vector<SDL_keysym_count *>::iterator i;

            for (i = key_history_held.begin(); i != key_history_held.end(); i++) {
                if ((*i)->sym.sym == key->sym) {
                    (*i)->held = false;
                    break;
                }
            }

            hkey->count = 0;
            hkey->held = false;
            memset(&hkey->sym, 0, sizeof(hkey->sym));
        }
        hkey++;
    }
}

void gh_screen::key_all (void)
{
    GH_FUNCTION()

    if (key_history_down.empty() && key_history_up.empty() &&
        key_history_held.empty()) {
        return;
    }

    vector<SDL_keysym_count *>::iterator i;

    for (i = key_history_held.begin(); i != key_history_held.end(); i++) {
        SDL_keysym_count * k = *i;

//cout << "held : \"" << SDL_GetKeyName(k->sym.sym) << "\" " << k->count << 
//endl;
        for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
            s != screens.rend(); s++) {

            gh_screen * sc = *s;

            if (!sc->active) {
                continue;
            }

            if (!sc->key_held) {
                continue;
            }

//cout << "screen " << sc->name << endl;
//cout << "delay " << sc->auto_repeat_delay << " rate " <<sc->auto_repeat_rate 
//<< endl;
            if (k->count > sc->auto_repeat_delay + sc->auto_repeat_rate) {

                if (MY_CALLBACK(sc, key_held)(k->sym) == false) {
                    continue;
                }
//cout << "grabbed by " << sc->name << endl;

                k->count -= sc->auto_repeat_rate;
                break;

//cout << "count now " <<  k->count << endl;
            } else if (k->count == sc->auto_repeat_delay) {

//cout << "first grabbed by " << sc->name << endl;

                if (MY_CALLBACK(sc, key_held)(k->sym) == false) {
                    continue;
                }
                break;
            }
        }
    }

    key_history_held.erase(key_history_held.begin(), key_history_held.end());

    vector<SDL_keysym>::iterator j;

    for (j = key_history_down.begin(); j != key_history_down.end(); j++) {
        SDL_keysym k = *j;

//cout << "down \"" << SDL_GetKeyName(k.sym) << "\" " << " mod " << k.mod << 
//endl;
        for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
            s != screens.rend(); s++) {

            gh_screen * sc = *s;

            if (!sc->active) {
                continue;
            }

            if (!sc->key_down) {
                continue;
            }

//cout << "  asked " << (*s)->name << endl;
            if (MY_CALLBACK(sc, key_down)(k) == false) {
                continue;
            }

//cout << "grabbed by " << (*s)->name << endl;
            break;
        }
    }

    key_history_down.erase(key_history_down.begin(),
        key_history_down.end());

    for (j = key_history_up.begin(); j != key_history_up.end(); j++) {
        SDL_keysym k = *j;

//cout << "down \"" << SDL_GetKeyName(k.sym) << "\" " << " mod " << k.mod << 
//endl;
        for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
            s != screens.rend(); s++) {

            gh_screen * sc = *s;

            if (!sc->active) {
                continue;
            }

            if (!sc->key_up) {
                continue;
            }

//cout << "  asked " << (*s)->name << endl;
            if (MY_CALLBACK(sc, key_up)(k) == false) {
                continue;
            }

            break;
//cout << "grabbed by " << (*s)->name << endl;
        }
    }

    key_history_up.erase(key_history_up.begin(),
        key_history_up.end());
}

void gh_screen::mouse_all (void)
{
    GH_FUNCTION()

    if (!mouse_held_count && mouse_history.empty()) {
        return;
    }

    if (mouse_held_count) {
        for (uint32 i=0; i<GH_ARRAY_SIZE(mouse_held_down); i++) {

            uint32 d = mouse_held_down[i];

            if (!d) {
                continue;
            }

            for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
                s != screens.rend(); s++) {

                gh_screen * sc = *s;

                if (!sc->active) {
                    continue;
                }

                if (!sc->mouse_held) {
                    continue;
                }

                if (d > sc->auto_repeat_delay + sc->auto_repeat_rate) {

                    if (MY_CALLBACK(sc, mouse_held)(i) == false) {
                        continue;
                    }

                    mouse_held_down[i] -= sc->auto_repeat_rate;

                    break;
                } else if (d == sc->auto_repeat_delay) {

                    if (MY_CALLBACK(sc, mouse_held)(i) == false) {
                        continue;
                    }
                    break;
                }
            }
        }
    }

    vector<SDL_Event>::iterator i;

    for (i = mouse_history.begin(); i != mouse_history.end(); i++) {
        SDL_Event k = *i;

        switch (k.type) {
            case SDL_MOUSEBUTTONDOWN:
                for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
                    s != screens.rend(); s++) {

                    gh_screen * sc = *s;

                    if (!sc->active) {
                        continue;
                    }

                    if (!sc->mouse_down) {
                        continue;
                    }

//cout << "mouse down " << (*s) <<
//      " " << (*s)->name <<
//      " " << (int)k.button.button << endl;

//                  if (double_click_screen[k.button.button] == s) {
//                  }
//                  double_click_timer[k.button.button] = 0;
//                  double_click_timer[k.button.button] = NULL;

                    if (MY_CALLBACK(sc, mouse_down)(k.button.button) == false) {
                        continue;
                    }
                    break;
                }
            break;
            case SDL_MOUSEBUTTONUP:
                for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
                    s != screens.rend(); s++) {
                    gh_screen * sc = *s;

//cout << "mouse move " << (*s) << " " << (*s)->name << endl;
                    if (!sc->active) {
                        continue;
                    }

                    if (!sc->mouse_up) {
                        continue;
                    }

//cout << "mouse up " << (*s) <<
//      " " << (*s)->name <<
//      " " << (int)k.button.button << endl;

                    if (MY_CALLBACK(sc, mouse_up)(k.button.button) == false) {
                        continue;
                    }

//                  double_click_timer[k.button.button] = s->tick;
//                  double_click_screen[k.button.button] = s;

//cout << "grabbed" << endl;
                    break;
                }
            break;
            case SDL_MOUSEMOTION:
                gh_point rel(k.motion.xrel, k.motion.yrel);
                gh_point abs(k.motion.x, k.motion.y);

                for (vector<gh_screen *>::reverse_iterator s = screens.rbegin();
                    s != screens.rend(); s++) {

//cout << "mouse move " << (*s) << " " << (*s)->name << endl;
                    gh_screen * sc = *s;

                    if (!sc->active) {
                        continue;
                    }

                    if (!sc->mouse_movement) {
                        continue;
                    }

                    if (MY_CALLBACK(sc, mouse_movement)(rel, abs) == false) {
                        continue;
                    }
//cout << "grabbed" << endl;
                    break;
                }
            break;
        }
    }

    mouse_history.erase(mouse_history.begin(), mouse_history.end());
}

void gh_screen::event_all (void)
{
    GH_FUNCTION()

    read_events();
    key_all();
    mouse_all();
}
