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

    $Id: screendump.cpp 1266 2009-08-02 16:02:37Z Goblinhack $
*/

#if defined WIN32 || defined __CYGWIN__
#include <windows.h>
#endif

#include <ghack.h>
#include <gh_surface.h>
#include <gh_game.h>
#include <zlib.h>

void gh_surface::screen_dump_raw (const gh_size s, const string file)
{
    GH_FUNCTION()

    uint8 *image;
    gzFile fptr;

    if ((fptr = gzopen(file.c_str(),"wb9")) == NULL) {
	GH_THROW("write to " + file);
	return;
    }

    image = new uint8[s.w * s.h * 3];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadBuffer(GL_BACK_LEFT);

    glReadPixels(0, 0, s.w, s.h, GL_RGB, GL_UNSIGNED_BYTE, image);

    /* Write the raw file */
    /* fprintf(fptr,"P6\n%d %d\n255\n",width,height); for ppm */
    for (int j=s.h-1;j>=0;j--) {
	for (int i=0;i<s.w;i++) {
	    gzputc(fptr, image[3*j*s.w+3*i+0]);
	    gzputc(fptr, image[3*j*s.w+3*i+1]);
	    gzputc(fptr, image[3*j*s.w+3*i+2]);
	}
    }

    gzclose(fptr);

    if (gh_global::game) {
	gh_global::game->new_message("Screen dump saved to " + file);
    }

    delete image;
}

void gh_surface::screen_dump_ppm (const gh_size s, const string file,
    const string comp_type)
{
    GH_FUNCTION()

    uint8 *image;
    gzFile fptr;
    string comp = "wb9";

    if (comp_type != "") {
	comp = comp_type;
    }

    if ((fptr = gzopen(file.c_str(), comp.c_str())) == NULL) {
	GH_THROW("write to " + file);
	return;
    }

    image = new uint8[s.w * s.h * 3];

    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    glReadBuffer(GL_BACK_LEFT);

    glReadPixels(0, 0, s.w, s.h, GL_RGB, GL_UNSIGNED_BYTE, image);

    gzprintf(fptr,"P6\n%d %d\n255\n",s.w,s.h);

    for (int j=s.h-1;j>=0;j--) {
	for (int i=0;i<s.w;i++) {
	    gzputc(fptr, image[3*j*s.w+3*i+0]);
	    gzputc(fptr, image[3*j*s.w+3*i+1]);
	    gzputc(fptr, image[3*j*s.w+3*i+2]);
	}
    }

    gzclose(fptr);

    if (gh_global::game) {
	gh_global::game->new_message("Screen dump saved to " + file);
    }

    delete image;
}

uint8 * gh_surface::screen_read_raw (const gh_size s, const string file)
{
    GH_FUNCTION()

    uint8 *image;
    image=0;
    gzFile fptr;

    if ((fptr = gzopen(file.c_str(),"rb")) == NULL) {
	GH_THROW("read from " + file);
	return (NULL);
    }

    const uint32 line = s.w * 3;
    const uint32 size = line * s.h;
    uint8 *save;

    save = new uint8[line];

    image = new uint8[size];

    //
    // BMPs are upside down, need to fix
    //
    gzread(fptr, image, size);

    for (int j=0; j<s.h/2; j++) {
	uint8 * a = &image[j*line];
	uint8 * b = &image[(s.h-1-j)*line];
	memcpy(save, a, line);
	memcpy(a, b, line);
	memcpy(b, save, line);
    }

    gzclose(fptr);

    delete save;

    return (image);
}
