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

    $Id: gh_getput_pixel_macros.h 513 2007-05-07 01:46:54Z goblinhack $
*/ 

#ifndef _GH_GETPUT_PIXEL_MACROS_H_
#define _GH_GETPUT_PIXEL_MACROS_H_

/*
 * Quick row-pixel bit-shift lookups for certain common pixel screen.size.widths:
 *
 * <<1  2
 * <<2  4
 * <<3  8
 * <<4  16
 * <<5  32
 * <<6  64
 * <<7  128
 * <<8  256
 * <<9  512
 * <<10 1024
 */

#define gh_pixel_16w(y)     ((y)<<4)
#define gh_pixel_24w(y)     ((y)<<4)+((y)<<3)
#define gh_pixel_32w(y)     ((y)<<5)
#define gh_pixel_48w(y)     ((y)<<5)+((y)<<4)
#define gh_pixel_64w(y)     ((y)<<6)
#define gh_pixel_128w(y)        ((y)<<7)
#define gh_pixel_256w(y)        ((y)<<8)
#define gh_pixel_320w(y)        (((y)<<8)+((y)<<6))
#define gh_pixel_512w(y)        ((y)<<9)
#define gh_pixel_640w(y)        (((y)<<9)+((y)<<7))
#define gh_pixel_800w(y)        (((y)<<9)+((y)<<8)+((y)<<5))
#define gh_pixel_1024w(y)       (((y)<<10))
#define gh_pixel_ANYw(s,y)      ((y)*(s->sdlSurface->pitch/s->sdlSurface->format->BytesPerPixel))

/*
 * 8 bpp macros:
 */
#define gh_pixel_16x8bpp(s, p)      ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_16w(p.y))
#define gh_pixel_24x8bpp(s, p)      ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_24w(p.y))
#define gh_pixel_32x8bpp(s, p)      ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_32w(p.y))
#define gh_pixel_48x8bpp(s, p)      ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_48w(p.y))
#define gh_pixel_64x8bpp(s, p)      ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_64w(p.y))
#define gh_pixel_128x8bpp(s, p) ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_128w(p.y))
#define gh_pixel_256x8bpp(s, p) ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_256w(p.y))
#define gh_pixel_320x8bpp(s, p) ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_320w(p.y))
#define gh_pixel_640x8bpp(s, p) ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_640w(p.y))
#define gh_pixel_800x8bpp(s, p) ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_800w(p.y))
#define gh_pixel_1024x8bpp(s, p)    ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_1024w(p.y))
#define gh_pixel_8bpp(s, p)     ((uint8*)s->sdlSurface->pixels+(p.x)+gh_pixel_ANYw(s,p.y))

#define gh_putPixel_16x8bpp(s, p, rgb)  *gh_pixel_16x8bpp(s, p) = rgb
#define gh_putPixel_24x8bpp(s, p, rgb)  *gh_pixel_24x8bpp(s, p) = rgb
#define gh_putPixel_32x8bpp(s, p, rgb)  *gh_pixel_32x8bpp(s, p) = rgb
#define gh_putPixel_48x8bpp(s, p, rgb)  *gh_pixel_48x8bpp(s, p) = rgb
#define gh_putPixel_64x8bpp(s, p, rgb)  *gh_pixel_64x8bpp(s, p) = rgb
#define gh_putPixel_128x8bpp(s, p, rgb) *gh_pixel_128x8bpp(s, p) = rgb
#define gh_putPixel_256x8bpp(s, p, rgb) *gh_pixel_256x8bpp(s, p) = rgb
#define gh_putPixel_320x8bpp(s, p, rgb) *gh_pixel_320x8bpp(s, p) = rgb
#define gh_putPixel_640x8bpp(s, p, rgb) *gh_pixel_640x8bpp(s, p) = rgb
#define gh_putPixel_800x8bpp(s, p, rgb) *gh_pixel_800x8bpp(s, p) = rgb
#define gh_putPixel_1024x8bpp(s, p, rgb) *gh_pixel_1024x8bpp(s, p) = rgb
#define gh_putPixel_8bpp(s, p, rgb)     *gh_pixel_8bpp(s, p) = rgb

#define gh_getPixel_16x8bpp(s, p, rgb)  *(rgb) = *gh_pixel_16x8bpp(s, p)
#define gh_getPixel_24x8bpp(s, p, rgb)  *(rgb) = *gh_pixel_24x8bpp(s, p)
#define gh_getPixel_32x8bpp(s, p, rgb)  *(rgb) = *gh_pixel_32x8bpp(s, p)
#define gh_getPixel_48x8bpp(s, p, rgb)  *(rgb) = *gh_pixel_48x8bpp(s, p)
#define gh_getPixel_64x8bpp(s, p, rgb)  *(rgb) = *gh_pixel_64x8bpp(s, p)
#define gh_getPixel_128x8bpp(s, p, rgb) *(rgb) = *gh_pixel_128x8bpp(s, p)
#define gh_getPixel_256x8bpp(s, p, rgb) *(rgb) = *gh_pixel_256x8bpp(s, p)
#define gh_getPixel_320x8bpp(s, p, rgb) *(rgb) = *gh_pixel_320x8bpp(s, p)
#define gh_getPixel_640x8bpp(s, p, rgb) *(rgb) = *gh_pixel_640x8bpp(s, p)
#define gh_getPixel_800x8bpp(s, p, rgb) *(rgb) = *gh_pixel_800x8bpp(s, p)
#define gh_getPixel_1024x8bpp(s, p, rgb) *(rgb) = *gh_pixel_1024x8bpp(s, p)
#define gh_getPixel_8bpp(s, p, rgb)     *(rgb) = *gh_pixel_8bpp(s, p)

/*
 * 16 bpp macros:
 */
#define gh_pixel_16x16bpp(s, p) ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_16w(p.y))
#define gh_pixel_24x16bpp(s, p) ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_24w(p.y))
#define gh_pixel_32x16bpp(s, p) ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_32w(p.y))
#define gh_pixel_48x16bpp(s, p) ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_48w(p.y))
#define gh_pixel_64x16bpp(s, p) ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_64w(p.y))
#define gh_pixel_128x16bpp(s, p)    ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_128w(p.y))
#define gh_pixel_256x16bpp(s, p)    ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_256w(p.y))
#define gh_pixel_320x16bpp(s, p)    ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_320w(p.y))
#define gh_pixel_640x16bpp(s, p)    ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_640w(p.y))
#define gh_pixel_800x16bpp(s, p)    ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_800w(p.y))
#define gh_pixel_1024x16bpp(s, p)   ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_1024w(p.y))
#define gh_pixel_16bpp(s, p)        ((uint16*)s->sdlSurface->pixels+(p.x)+gh_pixel_ANYw(s,p.y))

#define gh_putPixel_16x16bpp(s, p, rgb)     *gh_pixel_16x16bpp(s, p) = rgb
#define gh_putPixel_24x16bpp(s, p, rgb)     *gh_pixel_24x16bpp(s, p) = rgb
#define gh_putPixel_32x16bpp(s, p, rgb)     *gh_pixel_32x16bpp(s, p) = rgb
#define gh_putPixel_48x16bpp(s, p, rgb)     *gh_pixel_48x16bpp(s, p) = rgb
#define gh_putPixel_64x16bpp(s, p, rgb)     *gh_pixel_64x16bpp(s, p) = rgb
#define gh_putPixel_128x16bpp(s, p, rgb)    *gh_pixel_128x16bpp(s, p) = rgb
#define gh_putPixel_256x16bpp(s, p, rgb)    *gh_pixel_256x16bpp(s, p) = rgb
#define gh_putPixel_320x16bpp(s, p, rgb)    *gh_pixel_320x16bpp(s, p) = rgb
#define gh_putPixel_640x16bpp(s, p, rgb)    *gh_pixel_640x16bpp(s, p) = rgb
#define gh_putPixel_800x16bpp(s, p, rgb)    *gh_pixel_800x16bpp(s, p) = rgb
#define gh_putPixel_1024x16bpp(s, p, rgb)   *gh_pixel_1024x16bpp(s, p) = rgb
#define gh_putPixel_16bpp(s, p, rgb)        *gh_pixel_16bpp(s, p) = rgb

#define gh_getPixel_16x16bpp(s, p, rgb)     *(rgb) = *gh_pixel_16x16bpp(s, p)
#define gh_getPixel_24x16bpp(s, p, rgb)     *(rgb) = *gh_pixel_24x16bpp(s, p)
#define gh_getPixel_32x16bpp(s, p, rgb)     *(rgb) = *gh_pixel_32x16bpp(s, p)
#define gh_getPixel_48x16bpp(s, p, rgb)     *(rgb) = *gh_pixel_48x16bpp(s, p)
#define gh_getPixel_64x16bpp(s, p, rgb)     *(rgb) = *gh_pixel_64x16bpp(s, p)
#define gh_getPixel_128x16bpp(s, p, rgb)    *(rgb) = *gh_pixel_128x16bpp(s, p)
#define gh_getPixel_256x16bpp(s, p, rgb)    *(rgb) = *gh_pixel_256x16bpp(s, p)
#define gh_getPixel_320x16bpp(s, p, rgb)    *(rgb) = *gh_pixel_320x16bpp(s, p)
#define gh_getPixel_640x16bpp(s, p, rgb)    *(rgb) = *gh_pixel_640x16bpp(s, p)
#define gh_getPixel_800x16bpp(s, p, rgb)    *(rgb) = *gh_pixel_800x16bpp(s, p)
#define gh_getPixel_1024x16bpp(s, p, rgb)   *(rgb) = *gh_pixel_1024x16bpp(s, p)
#define gh_getPixel_16bpp(s, p, rgb)        *(rgb) = *gh_pixel_16bpp(s, p)

/*
 * 24 bpp (sucks for optimisations... Use 32 bpp if possible):
 */
#define gh_pixel_16x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_16w(p.y))*3))
#define gh_pixel_24x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_24w(p.y))*3))
#define gh_pixel_32x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_32w(p.y))*3))
#define gh_pixel_48x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_48w(p.y))*3))
#define gh_pixel_64x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_64w(p.y))*3))
#define gh_pixel_128x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_128w(p.y))*3))
#define gh_pixel_256x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_256w(p.y))*3))
#define gh_pixel_320x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_320w(p.y))*3))
#define gh_pixel_640x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_640w(p.y))*3))
#define gh_pixel_800x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_800w(p.y))*3))
#define gh_pixel_1024x24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_1024w(p.y))*3))
#define gh_pixel_24bpp(s, p)\
        ((uint8*)s->sdlSurface->pixels+(((p.x)+gh_pixel_ANYw(s,p.y))*3))

#define gh_putPixel_16x24bpp(s, p, rgb)\
        memcpy(gh_pixel_16x24bpp(s, p), &rgb, 3)
#define gh_putPixel_24x24bpp(s, p, rgb)\
        memcpy(gh_pixel_24x24bpp(s, p), &rgb, 3)
#define gh_putPixel_32x24bpp(s, p, rgb)\
        memcpy(gh_pixel_32x24bpp(s, p), &rgb, 3)
#define gh_putPixel_48x24bpp(s, p, rgb)\
        memcpy(gh_pixel_48x24bpp(s, p), &rgb, 3)
#define gh_putPixel_64x24bpp(s, p, rgb)\
        memcpy(gh_pixel_64x24bpp(s, p), &rgb, 3)
#define gh_putPixel_128x24bpp(s, p, rgb)\
        memcpy(gh_pixel_128x24bpp(s, p), &rgb, 3)
#define gh_putPixel_256x24bpp(s, p, rgb)\
        memcpy(gh_pixel_256x24bpp(s, p), &rgb, 3)
#define gh_putPixel_320x24bpp(s, p, rgb)\
        memcpy(gh_pixel_320x24bpp(s, p), &rgb, 3)
#define gh_putPixel_640x24bpp(s, p, rgb)\
        memcpy(gh_pixel_640x24bpp(s, p), &rgb, 3)
#define gh_putPixel_800x24bpp(s, p, rgb)\
        memcpy(gh_pixel_800x24bpp(s, p), &rgb, 3)
#define gh_putPixel_1024x24bpp(s, p, rgb)\
        memcpy(gh_pixel_1024x24bpp(s, p), &rgb, 3)
#define gh_putPixel_24bpp(s, p, rgb)\
        memcpy(gh_pixel_24bpp(s, p), &rgb, 3)

#define gh_getPixel_16x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_16x24bpp(s, p), 3)
#define gh_getPixel_24x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_24x24bpp(s, p), 3)
#define gh_getPixel_32x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_32x24bpp(s, p), 3)
#define gh_getPixel_48x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_48x24bpp(s, p), 3)
#define gh_getPixel_64x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_64x24bpp(s, p), 3)
#define gh_getPixel_128x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_128x24bpp(s, p), 3)
#define gh_getPixel_256x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_256x24bpp(s, p), 3)
#define gh_getPixel_320x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_320x24bpp(s, p), 3)
#define gh_getPixel_640x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_640x24bpp(s, p), 3)
#define gh_getPixel_800x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_800x24bpp(s, p), 3)
#define gh_getPixel_1024x24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_1024x24bpp(s, p), 3)
#define gh_getPixel_24bpp(s, p, rgb)\
        memcpy(rgb, gh_pixel_24bpp(s, p), 3)

/*
 * 32 bpp macros:
 */
#define gh_pixel_16x32bpp(s, p) ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_16w(p.y))
#define gh_pixel_24x32bpp(s, p) ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_24w(p.y))
#define gh_pixel_32x32bpp(s, p) ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_32w(p.y))
#define gh_pixel_48x32bpp(s, p) ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_48w(p.y))
#define gh_pixel_64x32bpp(s, p) ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_64w(p.y))
#define gh_pixel_128x32bpp(s, p)    ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_128w(p.y))
#define gh_pixel_256x32bpp(s, p)    ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_256w(p.y))
#define gh_pixel_320x32bpp(s, p)    ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_320w(p.y))
#define gh_pixel_640x32bpp(s, p)    ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_640w(p.y))
#define gh_pixel_800x32bpp(s, p)    ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_800w(p.y))
#define gh_pixel_1024x32bpp(s, p)   ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_1024w(p.y))
#define gh_pixel_32bpp(s, p)        ((uint32*)s->sdlSurface->pixels+(p.x)+gh_pixel_ANYw(s,p.y))

#define gh_putPixel_16x32bpp(s, p, rgb)     *gh_pixel_16x32bpp(s, p) = rgb
#define gh_putPixel_24x32bpp(s, p, rgb)     *gh_pixel_24x32bpp(s, p) = rgb
#define gh_putPixel_32x32bpp(s, p, rgb)     *gh_pixel_32x32bpp(s, p) = rgb
#define gh_putPixel_48x32bpp(s, p, rgb)     *gh_pixel_48x32bpp(s, p) = rgb
#define gh_putPixel_64x32bpp(s, p, rgb)     *gh_pixel_64x32bpp(s, p) = rgb
#define gh_putPixel_128x32bpp(s, p, rgb)    *gh_pixel_128x32bpp(s, p) = rgb
#define gh_putPixel_256x32bpp(s, p, rgb)    *gh_pixel_256x32bpp(s, p) = rgb
#define gh_putPixel_320x32bpp(s, p, rgb)    *gh_pixel_320x32bpp(s, p) = rgb
#define gh_putPixel_640x32bpp(s, p, rgb)    *gh_pixel_640x32bpp(s, p) = rgb
#define gh_putPixel_800x32bpp(s, p, rgb)    *gh_pixel_800x32bpp(s, p) = rgb
#define gh_putPixel_1024x32bpp(s, p, rgb)   *gh_pixel_1024x32bpp(s, p) = rgb
#define gh_putPixel_32bpp(s, p, rgb)        *gh_pixel_32bpp(s, p) = rgb

#define gh_getPixel_16x32bpp(s, p, rgb)     *(rgb) = *gh_pixel_16x32bpp(s, p)
#define gh_getPixel_24x32bpp(s, p, rgb)     *(rgb) = *gh_pixel_24x32bpp(s, p)
#define gh_getPixel_32x32bpp(s, p, rgb)     *(rgb) = *gh_pixel_32x32bpp(s, p)
#define gh_getPixel_48x32bpp(s, p, rgb)     *(rgb) = *gh_pixel_48x32bpp(s, p)
#define gh_getPixel_64x32bpp(s, p, rgb)     *(rgb) = *gh_pixel_64x32bpp(s, p)
#define gh_getPixel_128x32bpp(s, p, rgb)    *(rgb) = *gh_pixel_128x32bpp(s, p)
#define gh_getPixel_256x32bpp(s, p, rgb)    *(rgb) = *gh_pixel_256x32bpp(s, p)
#define gh_getPixel_320x32bpp(s, p, rgb)    *(rgb) = *gh_pixel_320x32bpp(s, p)
#define gh_getPixel_640x32bpp(s, p, rgb)    *(rgb) = *gh_pixel_640x32bpp(s, p)
#define gh_getPixel_800x32bpp(s, p, rgb)    *(rgb) = *gh_pixel_800x32bpp(s, p)
#define gh_getPixel_1024x32bpp(s, p, rgb)   *(rgb) = *gh_pixel_1024x32bpp(s, p)
#define gh_getPixel_32bpp(s, p, rgb)        *(rgb) = *gh_pixel_32bpp(s, p)

#endif /* _GH_GETPUT_PIXEL_MACROS_H_ */
