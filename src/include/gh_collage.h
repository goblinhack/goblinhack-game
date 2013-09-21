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

    $Id: gh_collage.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_COLLAGE_H_
#define _GH_COLLAGE_H_

#include <gh_surface.h>

class gh_tex {
public:
    float x1;
    float y1;
    float x2;
    float y2;

    gh_tex(float x1, float y1, float x2, float y2) :
	    x1(x1), y1(y1), x2(x2), y2(y2) {}
};

class gh_collage : public gh_stream
{
public:
    gh_size		pixel_size;	// Sz of each tile
    gh_size		tiles;	    // # of tiles in BMP

    gh_surface		* surface;
    gh_surface		* mask;

    vector<gh_tex>	images;	    // Images that compose collage
    vector<gh_point>	at;
    vector<gh_point>	bt;
    vector<gh_point>	tl;
    vector<gh_point>	br;

    gh_collage(const string gh_cfg_file);
    gh_collage(void)
    {
	surface = NULL;
	mask = NULL;
    }

    //
    // Process the bitmap into textures
    //
    void process(void);

    friend void operator>> (gh_stream_in& is, gh_collage *me);
    friend void operator<< (gh_stream_out& os, const gh_collage *me);
};
#endif /* _GH_COLLAGE_H_ */
