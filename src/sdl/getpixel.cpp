/*
    Retro.
    Copyright (C) 1999, 2000 Neil McGill

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

    $Id: getpixel.cpp 621 2008-01-04 15:25:24Z goblinhack $
*/

#include <ghack.h>
#include <gh_surface.h>
#include <gh_getput_pixel.h>
#include <gh_getput_pixel_macros.h>

/*
 * This function sets the specified pixel on a surface. Sanity checks are
 * performed on the co-ordinates and the surface is locked for you.
 * Safe, but slow. For more speed, try the lower level access function.
 */
gh_pixel gh_surface::getPixel(gh_point p)
{
    if ((p.x < 0) || (p.y < 0) || (p.x >= size.w) || (p.y >= size.h)) {
        GH_THROW("out of range ");
    }

    lock();

    gh_pixel r = lowerGetPixel(p);

    unlock();

    return (r);
}

void inline gh_getPixel_16x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_16x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_24x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_24x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_32x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_32x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_48x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_48x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_64x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_64x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_128x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_128x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_256x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_256x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_320x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_320x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_640x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_640x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_800x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_800x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_1024x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_1024x8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_8bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_8bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_16x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_16x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_24x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_24x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_32x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_32x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_48x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_48x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_64x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_64x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_128x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_128x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_256x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_256x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_320x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_320x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_640x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_640x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_800x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_800x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_1024x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_1024x16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_16bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_16bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_16x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_16x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_24x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_24x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_32x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_32x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_48x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_48x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_64x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_64x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_128x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_128x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_256x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_256x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_320x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_320x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_640x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_640x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_800x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_800x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_1024x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_1024x24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_24bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_24bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_16x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_16x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_24x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_24x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_32x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_32x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_48x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_48x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_64x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_64x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_128x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_128x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_256x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_256x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_320x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_320x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_640x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_640x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_800x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_800x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_1024x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_1024x32bpp(dst, p, &pix->rgb);
}

void inline gh_getPixel_32bpp_inline (gh_surface * dst, gh_point p, gh_pixel * pix)
{
     gh_getPixel_32bpp(dst, p, &pix->rgb);
}

/*
 * This function performs a 'low-level' pixel put, on a previously locked
 * surface. It is assumed that the pixel is within the bounds of the surface.
 * i.e. there are no sanity checks here to save you!
 */
gh_pixel gh_surface::lowerGetPixel(gh_point p)
{
    uint8   bpp = sdlSurface->format->BytesPerPixel;
    uint16  pitch = sdlSurface->pitch/bpp;
    gh_pixel pix;
    uint32  * rgb = &pix.rgb;

    if (bpp == 1) {
        switch (pitch) {
            case 16:
                gh_getPixel_16x8bpp(this, p, rgb);
                break;
            case 24:
                gh_getPixel_24x8bpp(this, p, rgb);
                break;
            case 32:
                gh_getPixel_32x8bpp(this, p, rgb);
                break;
            case 48:
                gh_getPixel_48x8bpp(this, p, rgb);
                break;
            case 64:
                gh_getPixel_64x8bpp(this, p, rgb);
                break;
            case 128:
                gh_getPixel_128x8bpp(this, p, rgb);
                break;
            case 256:
                gh_getPixel_256x8bpp(this, p, rgb);
                break;
            case 320:
                gh_getPixel_320x8bpp(this, p, rgb);
                break;
            case 640:
                gh_getPixel_640x8bpp(this, p, rgb);
                break;
            case 800:
                gh_getPixel_800x8bpp(this, p, rgb);
                break;
            case 1024:
                gh_getPixel_1024x8bpp(this, p, rgb);
                break;
            default:
                gh_getPixel_8bpp(this, p, rgb);
                break;
        }

    } else if (bpp == 2) {

        switch (pitch) {
            case 16:
                gh_getPixel_16x16bpp(this, p, rgb);
                break;
            case 24:
                gh_getPixel_24x16bpp(this, p, rgb);
                break;
            case 32:
                gh_getPixel_32x16bpp(this, p, rgb);
                break;
            case 48:
                gh_getPixel_48x16bpp(this, p, rgb);
                break;
            case 64:
                gh_getPixel_64x16bpp(this, p, rgb);
                break;
            case 128:
                gh_getPixel_128x16bpp(this, p, rgb);
                break;
            case 256:
                gh_getPixel_256x16bpp(this, p, rgb);
                break;
            case 320:
                gh_getPixel_320x16bpp(this, p, rgb);
                break;
            case 640:
                gh_getPixel_640x16bpp(this, p, rgb);
                break;
            case 800:
                gh_getPixel_800x16bpp(this, p, rgb);
                break;
            case 1024:
                gh_getPixel_1024x16bpp(this, p, rgb);
                break;
            default:
                gh_getPixel_16bpp(this, p, rgb);
                break;
        }

    } else if (bpp == 3) {

        switch (pitch) {
            case 16:
                gh_getPixel_16x24bpp(this, p, rgb);
                break;
            case 24:
                gh_getPixel_24x24bpp(this, p, rgb);
                break;
            case 32:
                gh_getPixel_32x24bpp(this, p, rgb);
                break;
            case 48:
                gh_getPixel_48x24bpp(this, p, rgb);
                break;
            case 64:
                gh_getPixel_64x24bpp(this, p, rgb);
                break;
            case 128:
                gh_getPixel_128x24bpp(this, p, rgb);
                break;
            case 256:
                gh_getPixel_256x24bpp(this, p, rgb);
                break;
            case 640:
                gh_getPixel_640x24bpp(this, p, rgb);
                break;
            case 320:
                gh_getPixel_320x24bpp(this, p, rgb);
                break;
            case 800:
                gh_getPixel_800x24bpp(this, p, rgb);
                break;
            case 1024:
                gh_getPixel_1024x24bpp(this, p, rgb);
                break;
            default:
                gh_getPixel_24bpp(this, p, rgb);
                break;
        }

    } else {

        switch (pitch) {
            case 16:
                gh_getPixel_16x32bpp(this, p, rgb);
                break;
            case 24:
                gh_getPixel_24x32bpp(this, p, rgb);
                break;
            case 32:
                gh_getPixel_32x32bpp(this, p, rgb);
                break;
            case 48:
                gh_getPixel_48x32bpp(this, p, rgb);
                break;
            case 64:
                gh_getPixel_64x32bpp(this, p, rgb);
                break;
            case 128:
                gh_getPixel_128x32bpp(this, p, rgb);
                break;
            case 256:
                gh_getPixel_256x32bpp(this, p, rgb);
                break;
            case 320:
                gh_getPixel_320x32bpp(this, p, rgb);
                break;
            case 640:
                gh_getPixel_640x32bpp(this, p, rgb);
                break;
            case 800:
                gh_getPixel_800x32bpp(this, p, rgb);
                break;
            case 1024:
                gh_getPixel_1024x32bpp(this, p, rgb);
                break;
            default:
                gh_getPixel_32bpp(this, p, rgb);
                break;
        }
    }

    return (pix);
}

/*
 * This function performs a 'low-level' pixel put, on a previously locked
 * surface. It is assumed that the pixel is within the bounds of the surface.
 * i.e. there are no sanity checks here to save you!
 */
gh_cacheGetPixel gh_surface::getCacheLowerGetPixel(void)
{
    uint8   bpp = sdlSurface->format->BytesPerPixel;
    uint16  pitch = sdlSurface->pitch/bpp;

    if (bpp == 1) {
        switch (pitch) {
            case 16:
                return (gh_getPixel_16x8bpp_inline);
            case 24:
                return (gh_getPixel_24x8bpp_inline);
            case 32:
                return (gh_getPixel_32x8bpp_inline);
            case 48:
                return (gh_getPixel_48x8bpp_inline);
            case 64:
                return (gh_getPixel_64x8bpp_inline);
            case 128:
                return (gh_getPixel_128x8bpp_inline);
            case 256:
                return (gh_getPixel_256x8bpp_inline);
            case 320:
                return (gh_getPixel_320x8bpp_inline);
            case 640:
                return (gh_getPixel_640x8bpp_inline);
            case 800:
                return (gh_getPixel_800x8bpp_inline);
            case 1024:
                return (gh_getPixel_1024x8bpp_inline);
            default:
                return (gh_getPixel_8bpp_inline);
        }

    } else if (bpp == 2) {

        switch (pitch) {
            case 16:
                return (gh_getPixel_16x16bpp_inline);
            case 24:
                return (gh_getPixel_24x16bpp_inline);
            case 32:
                return (gh_getPixel_32x16bpp_inline);
            case 48:
                return (gh_getPixel_48x16bpp_inline);
            case 64:
                return (gh_getPixel_64x16bpp_inline);
            case 128:
                return (gh_getPixel_128x16bpp_inline);
            case 256:
                return (gh_getPixel_256x16bpp_inline);
            case 320:
                return (gh_getPixel_320x16bpp_inline);
            case 640:
                return (gh_getPixel_640x16bpp_inline);
            case 800:
                return (gh_getPixel_800x16bpp_inline);
            case 1024:
                return (gh_getPixel_1024x16bpp_inline);
            default:
                return (gh_getPixel_16bpp_inline);
        }

    } else if (bpp == 3) {

        switch (pitch) {
            case 16:
                return (gh_getPixel_16x24bpp_inline);
            case 24:
                return (gh_getPixel_24x24bpp_inline);
            case 32:
                return (gh_getPixel_32x24bpp_inline);
            case 48:
                return (gh_getPixel_48x24bpp_inline);
            case 64:
                return (gh_getPixel_64x24bpp_inline);
            case 128:
                return (gh_getPixel_128x24bpp_inline);
            case 256:
                return (gh_getPixel_256x24bpp_inline);
            case 320:
                return (gh_getPixel_320x24bpp_inline);
            case 640:
                return (gh_getPixel_640x24bpp_inline);
            case 800:
                return (gh_getPixel_800x24bpp_inline);
            case 1024:
                return (gh_getPixel_1024x24bpp_inline);
            default:
                return (gh_getPixel_24bpp_inline);
        }

    } else {

        switch (pitch) {
            case 16:
                return (gh_getPixel_16x32bpp_inline);
            case 24:
                return (gh_getPixel_24x32bpp_inline);
            case 32:
                return (gh_getPixel_32x32bpp_inline);
            case 48:
                return (gh_getPixel_48x32bpp_inline);
            case 64:
                return (gh_getPixel_64x32bpp_inline);
            case 128:
                return (gh_getPixel_128x32bpp_inline);
            case 256:
                return (gh_getPixel_256x32bpp_inline);
            case 320:
                return (gh_getPixel_320x32bpp_inline);
            case 640:
                return (gh_getPixel_640x32bpp_inline);
            case 800:
                return (gh_getPixel_800x32bpp_inline);
            case 1024:
                return (gh_getPixel_1024x32bpp_inline);
            default:
                return (gh_getPixel_32bpp_inline);
        }
    }
}

//
// Lock a surface for safe pixel access
//
void gh_surface::lock (void) const
{
    if (SDL_MUSTLOCK(sdlSurface)) {
        SDL_LockSurface(sdlSurface);
    }
}

//
// Unlock a surface, after lock()
//
void gh_surface::unlock (void) const
{
    if (SDL_MUSTLOCK(sdlSurface)) {
        SDL_UnlockSurface(sdlSurface);
    }
}

//
// Fast get pixel, no boundary checks or surface locks
//
gh_pixel gh_surface::getPixelFast (gh_point p)
{
    gh_pixel pix;

    (*cacheGetPixel)(this, p, &pix);

    return (pix);
}

//
// Fast put pixel, no boundary checks or surface locks
//
void gh_surface::putPixelFast (gh_point p, gh_pixel pix)
{
    (*cachePutPixel)(this, p, pix);
}
