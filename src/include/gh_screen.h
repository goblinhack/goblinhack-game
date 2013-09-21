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

    $Id: gh_screen.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_SCREEN_H_
#define _GH_SCREEN_H_

#include "gh_stream.h"
#include "gh_size.h"
#include "gh_point.h"
#include "gh_pixel.h"
#include "gh_getput_pixel.h"
#include "gh_surface.h"

class gh_screen
{
public:
    
    static vector<class gh_screen *> screens;
    static vector<class gh_screen *> all_screens;
    static gh_size vid;

    void *classbase;
    string name;
    bool active;
    bool init_done;
    bool fini_done;
    bool visible;
    uint32 ticks;
    uint32 priority;
    uint32 auto_repeat_delay;
    uint32 auto_repeat_rate;

    static const uint32 auto_repeat_delay_default;
    static const uint32 auto_repeat_rate_default;

    gh_screen(void);
    ~gh_screen(void);
    void kill(void);
    void wake(void);
    void show(void);
    void hide(void);

    typedef void (gh_screen::*gh_screen_init)(void);
    typedef void (gh_screen::*gh_screen_shown)(void);
    typedef void (gh_screen::*gh_screen_hidden)(void);
    typedef void (gh_screen::*gh_screen_fini)(void);
    typedef void (gh_screen::*gh_screen_tick)(void);
    typedef void (gh_screen::*gh_screen_display)(void);
    typedef void (gh_screen::*gh_screen_reset)(void);
    typedef int (gh_screen::*gh_screen_key_down)(const SDL_keysym & key);
    typedef int (gh_screen::*gh_screen_key_up)(const SDL_keysym & key);
    typedef int (gh_screen::*gh_screen_key_held)(const SDL_keysym & key);
    typedef int (gh_screen::*gh_screen_mouse_up)(const size_t button);
    typedef int (gh_screen::*gh_screen_mouse_down)(const size_t button);
    typedef int (gh_screen::*gh_screen_mouse_held)(const size_t button);
    typedef int (gh_screen::*gh_screen_mouse_movement)(const gh_point & rel,
                                                       const gh_point & at);

    gh_screen_init init;
    gh_screen_shown shown;
    gh_screen_hidden hidden;
    gh_screen_fini fini;
    gh_screen_tick tick;
    gh_screen_display display;
    gh_screen_reset reset;
    gh_screen_key_down key_down;
    gh_screen_key_up key_up;
    gh_screen_key_held key_held;
    gh_screen_mouse_up mouse_up;
    gh_screen_mouse_down mouse_down;
    gh_screen_mouse_held mouse_held;
    gh_screen_mouse_movement mouse_movement;

    static void start_threads(void);
    static void init_all(void);
    static void fini_all(void);
    static void tick_all(void);
    static void display_all(void);
    static void key_all(void);
    static void mouse_all(void);
    static void event_all(void);
    static void reset_all(void);
    static void exit(void);
};

bool operator==(const gh_screen &a, const gh_screen &b);
bool operator<(const gh_screen &a, const gh_screen &b);

#endif /* _GH_SCREEN_H_ */
