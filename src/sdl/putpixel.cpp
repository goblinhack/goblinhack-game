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

    $Id: putpixel.cpp 621 2008-01-04 15:25:24Z goblinhack $
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
void gh_surface::putPixel(gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

    if ((p.x < 0) || (p.y < 0) || (p.x >= size.w) || (p.y >= size.h)) {
        GH_THROW("out of range");
    }

    lock();

    lowerPutPixel(p, pix);

    unlock();
}

void inline gh_putPixel_16x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_16x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_24x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_24x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_32x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_32x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_48x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_48x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_64x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_64x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_128x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_128x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_256x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_256x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_320x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_320x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_640x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_640x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_800x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_800x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_1024x8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_1024x8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_8bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_8bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_16x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_16x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_24x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_24x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_32x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_32x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_48x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_48x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_64x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_64x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_128x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_128x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_256x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_256x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_320x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_320x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_640x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_640x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_800x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_800x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_1024x16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_1024x16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_16bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_16bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_16x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_16x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_24x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_24x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_32x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_32x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_48x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_48x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_64x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_64x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_128x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_128x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_256x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_256x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_320x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_320x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_640x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_640x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_800x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_800x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_1024x24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_1024x24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_24bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_24bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_16x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_16x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_24x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_24x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_32x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_32x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_48x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_48x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_64x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_64x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_128x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_128x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_256x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_256x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_320x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_320x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_640x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_640x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_800x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_800x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_1024x32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_1024x32bpp(dst, p, pix.rgb);
}

void inline gh_putPixel_32bpp_inline (gh_surface * dst, gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

     gh_putPixel_32bpp(dst, p, pix.rgb);
}

/*
 * This function performs a 'low-level' pixel put, on a previously locked
 * surface. It is assumed that the pixel is within the bounds of the surface.
 * i.e. there are no sanity checks here to save you!
 */
void gh_surface::lowerPutPixel(gh_point p, gh_pixel pix)
{
    GH_FUNCTION()

    uint8   bpp = sdlSurface->format->BytesPerPixel;
    uint16  pitch = sdlSurface->pitch/bpp;

    if (bpp == 1) {

    switch (pitch) {
        case 16:
            gh_putPixel_16x8bpp(this, p, pix.rgb);
            return;
        case 24:
            gh_putPixel_24x8bpp(this, p, pix.rgb);
            return;
        case 32:
            gh_putPixel_32x8bpp(this, p, pix.rgb);
            return;
        case 48:
            gh_putPixel_48x8bpp(this, p, pix.rgb);
            return;
        case 64:
            gh_putPixel_64x8bpp(this, p, pix.rgb);
            return;
        case 128:
            gh_putPixel_128x8bpp(this, p, pix.rgb);
            return;
        case 256:
            gh_putPixel_256x8bpp(this, p, pix.rgb);
            return;
        case 320:
            gh_putPixel_320x8bpp(this, p, pix.rgb);
            return;
        case 640:
            gh_putPixel_640x8bpp(this, p, pix.rgb);
            return;
        case 800:
            gh_putPixel_800x8bpp(this, p, pix.rgb);
            return;
        case 1024:
            gh_putPixel_1024x8bpp(this, p, pix.rgb);
            return;
        default:
            gh_putPixel_8bpp(this, p, pix.rgb);
            return;
    }

    } else if (bpp == 2) {

    switch (pitch) {
        case 16:
            gh_putPixel_16x16bpp(this, p, pix.rgb);
            return;
        case 24:
            gh_putPixel_24x16bpp(this, p, pix.rgb);
            return;
        case 32:
            gh_putPixel_32x16bpp(this, p, pix.rgb);
            return;
        case 48:
            gh_putPixel_48x16bpp(this, p, pix.rgb);
            return;
        case 64:
            gh_putPixel_64x16bpp(this, p, pix.rgb);
            return;
        case 128:
            gh_putPixel_128x16bpp(this, p, pix.rgb);
            return;
        case 256:
            gh_putPixel_256x16bpp(this, p, pix.rgb);
            return;
        case 320:
            gh_putPixel_320x16bpp(this, p, pix.rgb);
            return;
        case 640:
            gh_putPixel_640x16bpp(this, p, pix.rgb);
            return;
        case 800:
            gh_putPixel_800x16bpp(this, p, pix.rgb);
            return;
        case 1024:
            gh_putPixel_1024x16bpp(this, p, pix.rgb);
            return;
        default:
            gh_putPixel_16bpp(this, p, pix.rgb);
            return;
    }

    } else if (bpp == 3) {

    switch (pitch) {
        case 16:
            gh_putPixel_16x24bpp(this, p, pix.rgb);
            return;
        case 24:
            gh_putPixel_24x24bpp(this, p, pix.rgb);
            return;
        case 32:
            gh_putPixel_32x24bpp(this, p, pix.rgb);
            return;
        case 48:
            gh_putPixel_48x24bpp(this, p, pix.rgb);
            return;
        case 64:
            gh_putPixel_64x24bpp(this, p, pix.rgb);
            return;
        case 128:
            gh_putPixel_128x24bpp(this, p, pix.rgb);
            return;
        case 256:
            gh_putPixel_256x24bpp(this, p, pix.rgb);
            return;
        case 640:
            gh_putPixel_640x24bpp(this, p, pix.rgb);
            return;
        case 320:
            gh_putPixel_320x24bpp(this, p, pix.rgb);
            return;
        case 800:
            gh_putPixel_800x24bpp(this, p, pix.rgb);
            return;
        case 1024:
            gh_putPixel_1024x24bpp(this, p, pix.rgb);
            return;
        default:
            gh_putPixel_24bpp(this, p, pix.rgb);
            return;
    }

    } else {

    switch (pitch) {
        case 16:
            gh_putPixel_16x32bpp(this, p, pix.rgb);
            return;
        case 24:
            gh_putPixel_24x32bpp(this, p, pix.rgb);
            return;
        case 32:
            gh_putPixel_32x32bpp(this, p, pix.rgb);
            return;
        case 48:
            gh_putPixel_48x32bpp(this, p, pix.rgb);
            return;
        case 64:
            gh_putPixel_64x32bpp(this, p, pix.rgb);
            return;
        case 128:
            gh_putPixel_128x32bpp(this, p, pix.rgb);
            return;
        case 256:
            gh_putPixel_256x32bpp(this, p, pix.rgb);
            return;
        case 320:
            gh_putPixel_320x32bpp(this, p, pix.rgb);
            return;
        case 640:
            gh_putPixel_640x32bpp(this, p, pix.rgb);
            return;
        case 800:
            gh_putPixel_800x32bpp(this, p, pix.rgb);
            return;
        case 1024:
            gh_putPixel_1024x32bpp(this, p, pix.rgb);
            return;
        default:
            gh_putPixel_32bpp(this, p, pix.rgb);
            return;
        }
    }
}

/*
 * This function performs a 'low-level' pixel put, on a previously locked
 * surface. It is assumed that the pixel is within the bounds of the surface.
 * i.e. there are no sanity checks here to save you!
 */
gh_cachePutPixel gh_surface::getCacheLowerPutPixel(void)
{
    GH_FUNCTION()

    uint8   bpp = sdlSurface->format->BytesPerPixel;
    uint16  pitch = sdlSurface->pitch/bpp;

    if (bpp == 1) {

        switch (pitch) {
            case 16:
                return (gh_putPixel_16x8bpp_inline);
            case 24:
                return (gh_putPixel_24x8bpp_inline);
            case 32:
                return (gh_putPixel_32x8bpp_inline);
            case 48:
                return (gh_putPixel_48x8bpp_inline);
            case 64:
                return (gh_putPixel_64x8bpp_inline);
            case 128:
                return (gh_putPixel_128x8bpp_inline);
            case 256:
                return (gh_putPixel_256x8bpp_inline);
            case 320:
                return (gh_putPixel_320x8bpp_inline);
            case 640:
                return (gh_putPixel_640x8bpp_inline);
            case 800:
                return (gh_putPixel_800x8bpp_inline);
            case 1024:
                return (gh_putPixel_1024x8bpp_inline);
            default:
                return (gh_putPixel_8bpp_inline);
        }

    } else if (bpp == 2) {

        switch (pitch) {
            case 16:
                return (gh_putPixel_16x16bpp_inline);
            case 24:
                return (gh_putPixel_24x16bpp_inline);
            case 32:
                return (gh_putPixel_32x16bpp_inline);
            case 48:
                return (gh_putPixel_48x16bpp_inline);
            case 64:
                return (gh_putPixel_64x16bpp_inline);
            case 128:
                return (gh_putPixel_128x16bpp_inline);
            case 256:
                return (gh_putPixel_256x16bpp_inline);
            case 320:
                return (gh_putPixel_320x16bpp_inline);
            case 640:
                return (gh_putPixel_640x16bpp_inline);
            case 800:
                return (gh_putPixel_800x16bpp_inline);
            case 1024:
                return (gh_putPixel_1024x16bpp_inline);
            default:
                return (gh_putPixel_16bpp_inline);
        }

    } else if (bpp == 3) {

        switch (pitch) {
            case 16:
                return (gh_putPixel_16x24bpp_inline);
            case 24:
                return (gh_putPixel_24x24bpp_inline);
            case 32:
                return (gh_putPixel_32x24bpp_inline);
            case 48:
                return (gh_putPixel_48x24bpp_inline);
            case 64:
                return (gh_putPixel_64x24bpp_inline);
            case 128:
                return (gh_putPixel_128x24bpp_inline);
            case 256:
                return (gh_putPixel_256x24bpp_inline);
            case 320:
                return (gh_putPixel_320x24bpp_inline);
            case 640:
                return (gh_putPixel_640x24bpp_inline);
            case 800:
                return (gh_putPixel_800x24bpp_inline);
            case 1024:
                return (gh_putPixel_1024x24bpp_inline);
            default:
                return (gh_putPixel_24bpp_inline);
        }

    } else {

        switch (pitch) {
            case 16:
                return (gh_putPixel_16x32bpp_inline);
            case 24:
                return (gh_putPixel_24x32bpp_inline);
            case 32:
                return (gh_putPixel_32x32bpp_inline);
            case 48:
                return (gh_putPixel_48x32bpp_inline);
            case 64:
                return (gh_putPixel_64x32bpp_inline);
            case 128:
                return (gh_putPixel_128x32bpp_inline);
            case 256:
                return (gh_putPixel_256x32bpp_inline);
            case 320:
                return (gh_putPixel_320x32bpp_inline);
            case 640:
                return (gh_putPixel_640x32bpp_inline);
            case 800:
                return (gh_putPixel_800x32bpp_inline);
            case 1024:
                return (gh_putPixel_1024x32bpp_inline);
            default:
                return (gh_putPixel_32bpp_inline);
        }
    }
}
