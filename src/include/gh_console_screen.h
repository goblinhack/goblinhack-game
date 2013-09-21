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

    $Id: gh_console_screen.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_CONSOLE_SCREEN_H_
#define _GH_CONSOLE_SCREEN_H_

#include <gh_collage.h>
#include <gh_screen.h>

class gh_console_screen : public gh_stream
{   
public:
    gh_console_screen(const string gh_cfg_file);
    ~gh_console_screen(void);

    gh_screen myscreen;
    gh_collage * font[1];

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
};

extern gh_console_screen * console_screen;
#else
#error reinclusion
#endif /* _GH_FPS_SCREEN_H_ */
