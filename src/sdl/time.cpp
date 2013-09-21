/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

    Neil McGill

    $Id: time.cpp 1191 2009-06-03 12:39:17Z Ghack $
*/

#include <ghack.h>
#include <gh_sdl.h>

uint32 gh_sdl_get_time_milli (void)
{
    GH_FUNCTION()

    uint32 t = SDL_GetTicks();

    return (t);
}

char *gh_sdl_get_time_string (void)
{
    GH_FUNCTION()

    static char date[30];

    //
    // micro - one millionth
    // millisec - one thousandth
    //
    uint32 msec;
    uint32 secs;
    uint32 mins;
    uint32 hrs;

    if (gh_global::sdl_initted) {
        msec = SDL_GetTicks();
        secs = msec / 1000;
        mins = secs / 60;
        hrs = mins / 60;

        msec = msec % 1000;
        secs = secs % 60;
        mins = mins % 60;
    } else {
        msec = 0;
        secs = 0;
        mins = 0;
        hrs = 0;
    }

    if (hrs) {
        snprintf(date, sizeof(date)-1, "%u:%02u:%02u.%03u",
                 hrs, mins, secs, msec);
    } else {
        snprintf(date, sizeof(date)-1, "%02u:%02u.%03u",
                 mins, secs, msec);
    }

    return (date);
}
