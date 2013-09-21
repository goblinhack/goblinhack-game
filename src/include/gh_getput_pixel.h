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

    $Id: gh_getput_pixel.h 513 2007-05-07 01:46:54Z goblinhack $
*/ 

#ifndef _GH_GETPUT_PIXEL_H_
#define _GH_GETPUT_PIXEL_H_

#include <gh_pixel.h>

/*
 * Public interface. Clips to surface and performs locking. Slow.
 */
extern int gh_putPixel(class gh_surface *dst, gh_point p, gh_pixel pix);

/*
 * Semi public interface. No clipping. Assumed locked surface.
 * Faster, but needs to look upp bpp depth and surface width.
 * To avoid lookups, use the cache routines retn'd from the first invocation.
 * (see following functions)
 */
extern void gh_lowerPutPixel(class gh_surface *dst, gh_point p, gh_pixel pix);

/*
 * Provided from gh_GetCacheLowerPutPixel(), as a quick pixel access method
 * to a hardwired function for a particular bpp and surface width. Fast!
 */
typedef void (*gh_cachePutPixel)
    (class gh_surface *dst, gh_point p, gh_pixel pix);

/*
 * Semi public interface. No clipping. Assumed locked surface.
 * Faster, but needs to look upp bpp depth and surface width.
 * To avoid lookups, use the cache routine retn'd from the first invocation.
 */
extern gh_cachePutPixel gh_GetCacheLowerPutPixel(class gh_surface *dst);

/*
 * Public interface. Clips to surface and performs locking. Slow.
 */
extern int gh_getPixel(class gh_surface *dst, gh_point p, gh_pixel * pix);

/*
 * Semi public interface. No clipping. Assumed locked surface.
 * Faster, but needs to look upp bpp depth and surface width.
 * To avoid lookups, use the cache routines retn'd from the first invocation.
 * (see following functions)
 */
extern void gh_lowerGetPixel
    (class gh_surface *dst, gh_point p, gh_pixel * pix);

/*
 * Provided from gh_GetCacheLowerGetPixel(), as a quick pixel access method
 * to a hardwired function for a particular bpp and surface width. Fast!
 */
typedef void (*gh_cacheGetPixel)
    (class gh_surface *dst, gh_point p, gh_pixel * ret);

/*
 * Semi public interface. No clipping. Assumed locked surface.
 * Faster, but needs to look upp bpp depth and surface width.
 * To avoid lookups, use the cache routine retn'd from the first invocation.
 */
extern gh_cacheGetPixel gh_GetCacheLowerGetPixel(class gh_surface *dst);

#endif /* _GH_GETPUT_PIXEL_H_ */
