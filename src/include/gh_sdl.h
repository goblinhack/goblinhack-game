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

    $Id: gh_sdl.h 606 2007-12-15 22:01:13Z goblinhack $
*/ 

#ifndef _GH_SDL_H_

#include <SDL.h>

extern void sdl_loop(void);
extern void sdl_loop_init(void);
extern void sdl_loop_fini(void);

extern uint32 gh_sdl_get_time_milli(void);
extern char *gh_sdl_get_time_string(void);

#endif /* _GH_SDL_H_ */
