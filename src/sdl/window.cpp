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

    $Id: window.cpp 1230 2009-07-04 02:47:29Z Ghack $
*/

#if defined WIN32 || defined __CYGWIN__
#include <windows.h>
#endif

#include <ghack.h>
#include <gh_surface.h>
#include <SDL.h>
#ifndef __APPLE__
#include <SDL_syswm.h>
#endif

void gh_surface::center_window (void)
{
    GH_FUNCTION()

#ifndef __APPLE__
    SDL_SysWMinfo info;

    SDL_VERSION(&info.version);
#endif

#if 0
    if (SDL_GetWMInfo(&info) > 0) {
#if defined WIN32 || defined __CYGWIN__
        HWND top;
        RECT top_rect;

        HWND current;
        RECT current_rect;

        current = info.window;
        top = GetDesktopWindow();

        GetClientRect(current, &current_rect);
        GetWindowRect(top, &top_rect);

        int x = ((top_rect.right - top_rect.left) -
            (current_rect.right - current_rect.left)) / 2;
        int y = ((top_rect.bottom - top_rect.top) -
            (current_rect.bottom - current_rect.top)) / 2;

        RECT        size;

        /* Get the current position of our window */
        GetWindowRect(current, &size);

        MoveWindow(current, (int)x, (int)y,
            (int)current_rect.right, (int)current_rect.bottom, TRUE);
#else
        SDL_Surface *screen = SDL_GetVideoSurface();
        int x, y;
        int w, h;
        if (info.subsystem == SDL_SYSWM_X11) {
            info.info.x11.lock_func();
            w = DisplayWidth(info.info.x11.display,
            DefaultScreen(info.info.x11.display));
            h = DisplayHeight(info.info.x11.display,
            DefaultScreen(info.info.x11.display));
            x = (w - screen->w)/2;
            y = (h - screen->h)/2;
            XMoveWindow(info.info.x11.display, info.info.x11.wmwindow, x, y);
            info.info.x11.unlock_func();
        }
#endif
    }
#endif
}

bool gh_surface::screensize (gh_size & s)
{
    GH_FUNCTION()

#ifdef __APPLE__
    s.w = 1024;
    s.h = 800;
    GH_LOG("Default screensize as SDL_GetWMInfo failed");
    return (true);
#else
    SDL_SysWMinfo info;

    SDL_VERSION(&info.version);

    if (SDL_GetWMInfo(&info) > 0) {
#if defined WIN32 || defined __CYGWIN__
        HWND top;
        RECT top_rect;

        HWND current;

        current = info.window;
        top = GetDesktopWindow();

        GetWindowRect(top, &top_rect);

        s.w = top_rect.right - top_rect.left;
        s.h = top_rect.bottom - top_rect.top;

        GH_LOG("Win32 window size " + tostring(s.w) + "x" + tostring(s.h));

        return (true);
#else
        if (info.subsystem == SDL_SYSWM_X11) {
            info.info.x11.lock_func();
            s.w = DisplayWidth(info.info.x11.display,
                DefaultScreen(info.info.x11.display));
            s.h = DisplayHeight(info.info.x11.display,
                DefaultScreen(info.info.x11.display));
            info.info.x11.unlock_func();

            if (gh_global::trace_mode) {
                s.w = 800;
                s.h = 600;
            }

            GH_LOG("Unix window size " + tostring(s.w) + "x" + tostring(s.h));
            return (true);
        } else {
            s.w = 1024;
            s.h = 800;
            GH_LOG("Default screensize for unix");
            return (true);
        }
#endif // unix
    } else {
        //
        // MacOS hits here - no idea why. Need help from nice MacOS folks.
        //
        s.w = 1024;
        s.h = 800;
        GH_LOG("Default screensize as SDL_GetWMInfo failed");
        return (true);
    }
#endif
}
