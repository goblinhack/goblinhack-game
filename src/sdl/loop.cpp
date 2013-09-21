/*
    ghack.
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

    $Id: loop.cpp 1265 2009-08-02 15:59:12Z Goblinhack $
*/

#include <ghack.h>
#include <gh_global.h>
#include <gh_term.h>
#include <gh_subsys.h>
#include <gh_console_screen.h>
#include <gh_sdl.h>
#include <gh_surface.h>
#include <gh_math.h>
#include <gh_audio.h>
#include <gh_music.h>

gh_surface *gh_sdl_screen;

//
// Hacks to see how slow the rendering is going
//
extern uint32 gh_map_gfx_cached;
extern uint32 gh_map_gfx_slow;

void sdl_loop_init (void)
{
    GH_FUNCTION()

    //
    // Initialize SDL
    //
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE) < 0) {
	cout << "Couldn't initialize SDL: " << SDL_GetError() << endl;

	exit(EXIT_FAILURE);
    }

    gh_surface::screensize(gh_global::default_screensize);

    if (gh_global::screensize == gh_size(0,0)) {
	gh_global::screensize = gh_global::default_screensize;
    }

    gh_global::sdl_initted = true;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    gh_sdl_screen = new gh_surface(
			      gh_global::screensize,
			      SDL_OPENGL |
			      (gh_global::fullscreen ? SDL_FULLSCREEN : 0) |
			      (gh_global::noframe ? SDL_NOFRAME : 0),
			      gh_surface::isVideo
			      );
    //
    // Name our window, if it's visible.
    //
    SDL_WM_SetCaption("Goblin Hack", NULL);

    gh_surface::load_all();

    gh_audio::init();
    gh_audio::load_all();

    gh_music::init();
    gh_music::load_all();

    //
    // Initialise screens
    //
    gh_screen::init_all();

    console_screen = new gh_console_screen("config/console.gz");

    //
    // Show screens
    //
    console_screen->myscreen.show();
}

void sdl_loop_fini (void)
{
    GH_FUNCTION()

    delete console_screen;
}

void sdl_loop (void)
{
    GH_FUNCTION()

    static const uint32 framerate_min = 30;
    static const uint32 framerate_max = 100;
    static uint32 frame = 0;
    static uint32 now = 0;
    static uint32 then = 0;
    static uint32 framerate = 0;
    static const uint32 max_fps_delay_index = 5;
    static uint32 which_fps_delay = 0;
    static uint32 total_fps_delay = 1;
    static uint32 fps_delay[max_fps_delay_index] = { 1, 1, 1, 1, 1 };
    static uint32 slow_penalty;

    //
    // Only generate relative motion
    //
    gh_global::mouse_visible = false;
    SDL_ShowCursor(gh_global::mouse_visible);

    //
    // The main inner loop, hopefully fast!
    //
    gh_global::done = false;

    while (!gh_global::done) {

	//
	// Clear The Screen And The Depth Buffer
	//
	glClearColor(gh_global::flash_color.r,
		     gh_global::flash_color.g,
		     gh_global::flash_color.b,
		     1.0);

	if ((gh_global::flash_color.r != 0.0) ||
	    (gh_global::flash_color.g != 0.0) ||
	    (gh_global::flash_color.b != 0.0)) {

	    gh_global::flash_color.r *= 0.95;
	    gh_global::flash_color.g *= 0.95;
	    gh_global::flash_color.b *= 0.95;
	}

	//
	// A bit faster to avoid clearing the depth buffer.
	//
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	// Process all input events
	//
	gh_screen * current = *(gh_screen::screens.begin());
	gh_set_time_milli();
	current->event_all();
	current->tick_all();
	frame++;

	current->display_all();

	//
	// Occasionally, do slow things.
	//
	if (gh_global::fps) {
	    gh_term_fgbg(GH_TERM_COLOR_GREEN, GH_TERM_COLOR_BLACK);
	    gh_term_goto(0, 5);

	    if (gh_global::really_slow) {
		gh_term_putf((tostring(framerate) + 
			     " REALLY slow, pen " + 
			     tostring(slow_penalty)).c_str());
	    } else if (gh_global::too_slow) {
		gh_term_putf((tostring(framerate) + 
			     " slow, pen " + 
			     tostring(slow_penalty)).c_str());
	    } else {
		gh_term_putf(tostring(framerate).c_str());
	    }

	    static char tmp_str[40];

	    gh_term_goto(0, 6);
	    sprintf(tmp_str, "cache %d slow %d",
		    gh_map_gfx_cached,
		    gh_map_gfx_slow);
	    gh_term_puts(tmp_str);
	}

	if (frame == 32) {
	    random_fg = (gh_term_color)((gh_math::rand100() & 15) + 2);
	    random_bg = (gh_term_color)((gh_math::rand100() & 15) + 2);

	    if (gh_global::dump) {
		static int cnt;

		cnt++;

		gh_surface::screen_dump_ppm(
		    gh_size(gh_sdl_screen->size.w,
			    gh_sdl_screen->size.h),
		    "dump" + tostring(cnt) + ".ppm.gz");

		gh_global::dump = false;
	    }

	    now = SDL_GetTicks();
	    framerate = (uint32)(((float)frame*1000.0)/(float)(now-then));
	    then = now;
	    frame = 0;

	    bool fps_changed = false;

	    //
	    // Has the framerate has dipped below expectations.
	    //
	    if (framerate < framerate_min) {
		gh_global::too_slow = true;

		//
		// Gradual decreases are better to avoid ping-ponging of FPS
		//
		if (total_fps_delay) {
		    total_fps_delay--;
		}

		fps_changed = true;

		slow_penalty ++;
		slow_penalty ++;
		slow_penalty ++;
		slow_penalty ++;
		slow_penalty ++;
		slow_penalty ++;

		if (slow_penalty > 5000) {
		    slow_penalty = 5000;
		}

	    } else {
		if (framerate > framerate_max) {
		    //
		    // Gradual increases are better to avoid ping-ponging of FPS
		    //
		    total_fps_delay++;
		    fps_changed = true;
		}

		if (slow_penalty > 0) {
		    slow_penalty--;
		}
	    }

	    gh_global::too_slow = (slow_penalty > 1000);
	    gh_global::really_slow = (slow_penalty > 2500);

	    //
	    // We spread out increases in the delay over each frame to
	    // smooth it out - and as SDL handles jumps per frame really
	    // badly.
	    //
	    if (fps_changed) {
		float df = (float)total_fps_delay / (float)max_fps_delay_index;
		float af = 0.0;

		for (uint8 f = 0; f < which_fps_delay; f++) {

		    af += df;

		    if (af < 1.0) {
			continue;
		    }

		    fps_delay[f] = (int)af;

		    af -= floor(af);
		}
	    }
	}

	SDL_GL_SwapBuffers();

	//
	// Step through the list of delays that we have
	//
	which_fps_delay++;
	if (which_fps_delay >= max_fps_delay_index) {
	    which_fps_delay = 0;
	}

	SDL_Delay(fps_delay[which_fps_delay]);

	SDL_PumpEvents();
    }
}
