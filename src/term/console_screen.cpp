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

    $Id: console_screen.cpp 1070 2008-08-11 23:30:42Z Goblinhack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_console.h>
#include <gh_console_screen.h>
#include <gh_sdl.h>
#include <gh_screen.h>

gh_console_screen * console_screen;

gh_console_screen::gh_console_screen (const string gh_cfg_file)
{
    GH_FUNCTION()

    font[0] = new gh_collage(string("fonts/console"));

    console.init(font[0]);

    gh_term_init();

    myscreen.name = __FILE__;
    myscreen.classbase = this;
    myscreen.priority = 200;

    myscreen.init =
	(gh_screen::gh_screen_init)&gh_console_screen::init;
    myscreen.fini =
	(gh_screen::gh_screen_fini)&gh_console_screen::fini;
    myscreen.tick =
	(gh_screen::gh_screen_tick)&gh_console_screen::tick;
    myscreen.display =
	(gh_screen::gh_screen_display)&gh_console_screen::display;
    myscreen.reset =
	(gh_screen::gh_screen_reset)&gh_console_screen::reset;
    myscreen.key_down =
	(gh_screen::gh_screen_key_down)&gh_console_screen::key_down;
    myscreen.key_held =
	(gh_screen::gh_screen_key_held)&gh_console_screen::key_held;
    myscreen.mouse_up =
	(gh_screen::gh_screen_mouse_up)&gh_console_screen::mouse_up;
    myscreen.mouse_down =
	(gh_screen::gh_screen_mouse_down)&gh_console_screen::mouse_down;
    myscreen.mouse_held =
	(gh_screen::gh_screen_mouse_held)&gh_console_screen::mouse_held;
    myscreen.mouse_movement =
	(gh_screen::gh_screen_mouse_movement)&gh_console_screen::mouse_movement;
}

gh_console_screen::~gh_console_screen (void)
{
    GH_FUNCTION()

    delete font[0];

    gh_term_exit();
}

void gh_console_screen::init (void)
{
    GH_FUNCTION()

}

void gh_console_screen::fini (void)
{
    GH_FUNCTION()

}

void gh_console_screen::tick (void)
{
    GH_FUNCTION()

}

void gh_console_screen::display (void)
{
    GH_FUNCTION()

    static int x;

    if (!(x++ % 2)) {
	gh_term_refresh();
	x = 0;
    }
}

int gh_console_screen::key_held (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (false);
}

int gh_console_screen::key_down (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (false);
}

int gh_console_screen::key_up (const SDL_keysym & sym)
{
    GH_FUNCTION()

    return (false);
}

int gh_console_screen::mouse_up (const size_t button)
{
    GH_FUNCTION()

    return (false);
}

int gh_console_screen::mouse_down (const size_t button)
{
    GH_FUNCTION()

    return (false);
}

int gh_console_screen::mouse_held (const size_t button)
{
    GH_FUNCTION()

    return (false);
}

int gh_console_screen::mouse_movement (const gh_point & rel, const gh_point &at)
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

void gh_console_screen::reset (void)
{
    GH_FUNCTION()
}
