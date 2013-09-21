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

    $Id: collage.cpp 1186 2009-05-31 23:57:24Z Goblinhack $
*/

#include <ghack.h>
#include <gh_collage.h>
#include <gh_surface.h>
#include <gh_pixel.h>

gh_collage::gh_collage (const string gh_cfg_file)
{
    GH_FUNCTION()

    GH_LOG("Load collage");
//    GH_LOG("  " + gh_cfg_file);
    gh_read_file(this, gh_cfg_file);
}

void gh_collage::process (void)
{
    GH_FUNCTION()

    if (!surface->size.w) {
	GH_THROW("no width");
    }

    if (!surface->size.h) {
	GH_THROW("no height");
    }

    if (!surface->size.d) {
	GH_THROW("no depth");
    }

    this->pixel_size = pixel_size;
    this->tiles = tiles;

    if ((pixel_size.w == 0) && (pixel_size.h == 0)) {
	pixel_size.w = surface->size.w / tiles.w;
	pixel_size.h = surface->size.h / tiles.h;

	this->pixel_size = pixel_size;
    } else {
	tiles.w = surface->orig_size.w / pixel_size.w;
	tiles.h = surface->orig_size.h / pixel_size.h;

	if (tiles.w > surface->size.w) {
	    GH_THROW("overflow on collage width,"
		     " tiles=" + tostring(tiles.w) + "x" + tostring(tiles.h) +
		     " size=" + tostring(pixel_size.w) + "x" + tostring(pixel_size.h) +
		     " only have " +
		     tostring(surface->orig_size.w) + "x" + tostring(surface->orig_size.h) +
		     " pixels")
	}

	if (tiles.h > surface->size.h) {
	    GH_THROW("overflow on collage height,"
		     " tiles=" + tostring(tiles.w) + "x" + tostring(tiles.h) +
		     " size=" + tostring(pixel_size.w) + "x" + tostring(pixel_size.h) +
		     " only have " +
		     tostring(surface->orig_size.w) + "x" + tostring(surface->orig_size.h) +
		     " pixels")
	}
    }

//    GH_LOG("    tiles       " + tostring(tiles.w) + "x" + 
//    tostring(tiles.h));
//    GH_LOG("    size        " + tostring(pixel_size.w) + "x" + 
//    tostring(pixel_size.h));
//    GH_LOG("    orig pixels " + tostring(surface->orig_size.w) + "x" + 
//    tostring(surface->orig_size.h));
//    GH_LOG("    pixels      " + tostring(surface->size.w) + "x" + 
//    tostring(surface->size.h));

    if ((pixel_size.w * tiles.w > surface->orig_size.w) ||
	(pixel_size.h * tiles.h > surface->orig_size.h)) {

	GH_THROW("overflow on collage,"
		 " tiles=" + tostring(tiles.w) + "x" + tostring(tiles.h) +
		 " size=" + tostring(pixel_size.w) + "x" + tostring(pixel_size.h) +
		 " only have " +
		 tostring(surface->orig_size.w) + "x" + tostring(surface->orig_size.h) +
		 " pixels")
    }

    //
    // Work out how large the original surface is inside the opengl texture
    // which we had to round up to the next power of 2 pixels
    //
    const float tex_w = (float)surface->orig_size.w / (float)surface->size.w;
    const float tex_h = (float)surface->orig_size.h / (float)surface->size.h;

    //
    // How big in floating point texture units is each tile within the original
    // surface? The tex points live within the bigger surface.
    //
    const float sx = tex_w / (float)(surface->orig_size.w / pixel_size.w);
    const float sy = tex_h / (float)(surface->orig_size.h / pixel_size.h);

    for (int y=0; y<tiles.h; y++) {
	for (int x=0; x<tiles.w; x++) {
	    //
	    // What are the bounds of each tile?
	    //
	    gh_tex t(sx * (float)x,
		     1.0 - (sy * (float)(y+1)),
		     sx * (float)(x+1),
		     1.0 - (sy * (float)(y)));

	    images.push_back(t);
	    int yu = tiles.h - 1 - y;

	    gh_point AT((pixel_size.w * x) + 0,
			 (pixel_size.h * yu) + 0);
	    gh_point BT((pixel_size.w * x) + pixel_size.w,
			 (pixel_size.h * yu) + pixel_size.h);
	    gh_point MAX((pixel_size.w * x) + 0,
			 (pixel_size.h * yu) + 0);
	    gh_point MIN((pixel_size.w * x) + pixel_size.w - 1,
			 (pixel_size.h * yu) + pixel_size.h - 1);

	    for (int y1=pixel_size.h-1; y1>=0; y1--) {
		for (int x1=0; x1<pixel_size.w; x1++) {

		    gh_point at((pixel_size.w * x) + x1,
				(pixel_size.h * yu) + y1);

		    gh_pixel p = surface->getPixel(at);

		    if (p.rgb & 0x00ffffff) {
			MIN.x = min(at.x, MIN.x);
			MIN.y = min(at.y, MIN.y);
			MAX.x = max(at.x, MAX.x);
			MAX.y = max(at.y, MAX.y);
		    }
		}
	    }

	    tl.push_back(MIN);
	    br.push_back(MAX);
	    at.push_back(AT);
	    bt.push_back(BT);
	}
    }

#define nDEBUG_MIN_MAX
#ifdef DEBUG_MIN_MAX
    int v=0;
    for (int y=0; y<tiles.h; y++) {
	for (int x=0; x<tiles.w; x++) {
	    for (int y1=pixel_size.h-1; y1>=0; y1--) {
		for (int x1=0; x1<pixel_size.w; x1++) {

		    int yu = tiles.h - 1 - y;

		    gh_point at((pixel_size.w * x) + x1,
				(pixel_size.h * yu) + y1);

		    if (at == tl[v]) {
			cout << "<";
			continue;
		    }
		    if (at == br[v]) {
			cout << ">";
			continue;
		    }

		    gh_pixel p = surface->getPixel(at);

		    if (p.rgb != 0xff000000) {
			cout << ".";
		    } else  {
			cout << " ";
		    }
		}
		cout << endl;
	    }
	    v++;
	}
    }
#endif
}

void operator>> (gh_stream_in& is, gh_collage *me)
{
    GH_FUNCTION()

    string tmp;

    me->get_unnamed(is, &tmp);
    me->surface = gh_surface::find(tmp);

    me->get_unnamed(is, &tmp);
    if (tmp != "") {
	me->mask = gh_surface::find(tmp);
    }

    me->get_class(is, &me->tiles, "tile");
    me->get_class(is, &me->pixel_size, "pixels");

    me->process();
}

void operator<< (gh_stream_out& os, const gh_collage *me)
{
    GH_FUNCTION()
}
