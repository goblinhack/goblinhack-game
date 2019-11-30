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

    $Id: gh_surface.h 475 2007-03-10 03:29:22Z goblinhack $
*/

#ifndef _GH_SURFACE_H_
#define _GH_SURFACE_H_

#if defined WIN32 || defined __CYGWIN__
#include <windows.h>
#endif
#include <SDL.h>

#ifdef MACOSX
#include <SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

#include <gh_pixel.h>
#include <gh_getput_pixel.h>

class gh_surface : public gh_stream
{
public:
    enum gh_surfaceType {
        isNormal = 0,
        isVideo = 1,
    };

    SDL_Surface             * sdlSurface;   // SDL image
    int                     tex;            // OpenGL texture, if created
    gh_size3d               size;           // Surface size.
    gh_size3d               orig_size;          // Surface size befor cut().
    string                  short_name;
    string                  full_name;

    //
    // Create the primary video surface
    //
    gh_surface(gh_size3d size, uint32 flags=0,
        enum gh_surfaceType type = isNormal);

    //
    // Create detailed surface
    //
    gh_surface(gh_size3d size, uint32 flags,
        uint32 rmask, uint32 gmask, uint32 bmask, uint32 amask);

    //
    // Create surface from bitmap file
    //
    gh_surface(string file);

    ~gh_surface();

    static void smooth_scroll(bool on);

    //
    // Texture bank operations
    //
    typedef map<string, gh_surface*> hash_surfaces;

    static hash_surfaces surfaces;
    static void unload_all(void);
    static void load_all(void);
    static void reload_all(void);

    //
    // Find this texture in the texture bank
    //
    gh_inline static gh_surface* find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this surface in the surface bank
    //
    gh_inline static gh_surface* find (const char* s1,
                                       const bool err = true)
    {
        gh_surface::hash_surfaces::iterator m =
            surfaces.find(s1);

        if (m == surfaces.end()) {
            if (err) {
                GH_THROW("couldn't find " + string(s1) + " in loaded list");
            }
            return (NULL);
        }

        gh_surface *surface = m->second;

        if (surface == NULL) {
            GH_THROW("null surface " + string(s1) + " in loaded list");
        }

        return (surface);
    }

    //
    // Convert to an opengl texture.
    //
    void texture(uint32 type = 0);
    void reset(void);

    //
    // To resize the video mode.
    //
    static void resize(gh_size3d size, uint32 flags=0);

    void clear(void);
    void clear(const gh_fpoint &tl, const gh_fpoint &br);
    void clear(const gh_point &tl, const gh_point &br);

    static void openglinit(void);
    static bool openglprobe(const gh_size3d videoSz);
    static bool openglsetup(gh_size3d &videoSz, const bool fail);

    //
    // Cut the whitespace around an image. Good for getting a
    // quicker blit. 'key' is the color to use as the 'scissors' and
    // subsequent color key. This modifies the color key!
    //
    void cut(gh_pixel pix);

    ostream& put(ostream& os) const;

    static void center_window(void);
    static bool screensize(gh_size & s);

    static void screen_dump_raw(const gh_size s, const string file);
    static void screen_dump_ppm(const gh_size s, const string file,
        const string comp_type = "");
    static uint8* screen_read_raw(const gh_size s, const string file);

    //
    // Get/Set a pixel, throws if out of bounds
    // Also does surface locking
    //
    gh_pixel getPixel(gh_point p);
    void putPixel(gh_point p, gh_pixel pix);

    //
    // Fast get pixel, no boundary checks or surface locks
    //
    gh_pixel getPixelFast(gh_point p);

    //
    // Fast put pixel, no boundary checks or surface locks
    //
    void putPixelFast(gh_point p, gh_pixel pix);

    //
    // Lock a surface for safe pixel access
    //
    void lock(void) const;

    //
    // Unlock a surface, after lock()
    //
    void unlock(void) const;

private:
    gh_cacheGetPixel cacheGetPixel;     // Fast pixel get routine
    gh_cachePutPixel cachePutPixel;     // Fast pixel put routine

    gh_pixel lowerGetPixel(gh_point p);
    gh_cacheGetPixel getCacheLowerGetPixel(void);

    void lowerPutPixel(gh_point p, gh_pixel pix);
    gh_cachePutPixel getCacheLowerPutPixel(void);

    void common(gh_size3d size, uint32 flags,
        uint32 rmask, uint32 gmask, uint32 bmask, uint32 amask);
};

extern gh_surface::hash_surfaces surfaces;
extern gh_surface *gh_sdl_screen;

#endif /* _GH_SURFACE_H_ */
