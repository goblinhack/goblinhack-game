/*
    retreat.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This game is distributed in the hoE that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

    Neil McGill

*/

#include <ghack.h>
#include <gh_model.h>

//
// Draw a shaded square
//
uint32 gh_model::gl_list_square (gh_fpoint tl,
			       gh_fpoint br,
			       gh_fsize uv,
			       gh_pixel hi,
			       gh_pixel med,
			       gh_pixel lo)
{
    GH_FUNCTION()

    uint32 l;

    l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE);

	const gh_fpoint a(tl.x, tl.y);
	const gh_fpoint b(br.x, tl.y);
	const gh_fpoint c(br.x, br.y);
	const gh_fpoint d(tl.x, br.y);

	glEnable(GL_TEXTURE_2D);

	//
	// Draw the central flat square
	//
	med.gl();

	glBegin(GL_TRIANGLE_STRIP);

	    gh_model::gl_vertex2uf_uv(a.x, a.y, uv);
	    gh_model::gl_vertex2uf_uv(b.x, b.y, uv);
	    hi.gl();
	    gh_model::gl_vertex2uf_uv(d.x, d.y, uv);
	    med.gl();
	    gh_model::gl_vertex2uf_uv(c.x, c.y, uv);

	glEnd();

    glEndList();

    return (l);
}

//
// Draw a square with 3D relief
//
uint32 gh_model::gl_list_square_bevelled (gh_fpoint tl,
					gh_fpoint br,
					gh_fsize uv,
					gh_pixel hi,
					gh_pixel med,
					gh_pixel lo,
					gh_fsize bevel)
{
    GH_FUNCTION()

    uint32 l;

    l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE);

	const gh_fpoint a(tl.x, tl.y);
	const gh_fpoint b(br.x, tl.y);
	const gh_fpoint c(br.x, br.y);
	const gh_fpoint d(tl.x, br.y);
	const gh_fpoint a1(tl.x + bevel.w, tl.y + bevel.h);
	const gh_fpoint b1(br.x - bevel.w, tl.y + bevel.h);
	const gh_fpoint c1(br.x - bevel.w, br.y - bevel.h);
	const gh_fpoint d1(tl.x + bevel.w, br.y - bevel.h);

	glEnable(GL_TEXTURE_2D);

	//
	// Draw the central flat square
	//
	med.gl();

	glBegin(GL_TRIANGLE_STRIP);

	    gh_model::gl_vertex2uf_uv(a1.x, a1.y, uv);
	    gh_model::gl_vertex2uf_uv(b1.x, b1.y, uv);
	    hi.gl();
	    gh_model::gl_vertex2uf_uv(d1.x, d1.y, uv);
	    med.gl();
	    gh_model::gl_vertex2uf_uv(c1.x, c1.y, uv);

	glEnd();


	//
	// Now draw the bevelled border
	//
	hi.gl();
	gl_quad_uv(c1, c, d, d1, uv);
	gl_quad_uv(a, a1, d1, d, uv);

	lo.gl();
	gl_quad_uv(a, b, b1, a1, uv);
	gl_quad_uv(b, c, c1, b1, uv);

    glEndList();

    return (l);
}

//
// Draw a square with 3D relief
//
uint32 gh_model::gl_list_square_bevelled_plain (gh_fpoint tl,
					      gh_fpoint br,
					      gh_fsize uv,
					      gh_pixel hi,
					      gh_pixel med,
					      gh_pixel lo,
					      gh_fsize bevel)
{
    GH_FUNCTION()

    uint32 l;

    l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE);

	const gh_fpoint a(tl.x, tl.y);
	const gh_fpoint b(br.x, tl.y);
	const gh_fpoint c(br.x, br.y);
	const gh_fpoint d(tl.x, br.y);
	const gh_fpoint a1(tl.x + bevel.w, tl.y + bevel.h);
	const gh_fpoint b1(br.x - bevel.w, tl.y + bevel.h);
	const gh_fpoint c1(br.x - bevel.w, br.y - bevel.h);
	const gh_fpoint d1(tl.x + bevel.w, br.y - bevel.h);

	glEnable(GL_TEXTURE_2D);

	//
	// Draw the central flat square
	//
	med.gl();

	glBegin(GL_TRIANGLE_STRIP);

	    gh_model::gl_vertex2uf_uv(a1.x, a1.y, uv);
	    gh_model::gl_vertex2uf_uv(b1.x, b1.y, uv);
	    gh_model::gl_vertex2uf_uv(d1.x, d1.y, uv);
	    gh_model::gl_vertex2uf_uv(c1.x, c1.y, uv);

	glEnd();


	//
	// Now draw the bevelled border
	//
	hi.gl();
	gl_quad_uv(c1, c, d, d1, uv);
	gl_quad_uv(a, a1, d1, d, uv);

	lo.gl();
	gl_quad_uv(a, b, b1, a1, uv);
	gl_quad_uv(b, c, c1, b1, uv);

    glEndList();

    return (l);
}

//
// Draw a rounded square
//
uint32 gh_model::gl_list_rounded_square (gh_fpoint tl,
				       gh_fpoint br,
				       gh_fsize uv,
				       gh_pixel hi,
				       gh_pixel med,
				       gh_pixel lo,
				       gh_fsize radius,
				       int sides)
{
    GH_FUNCTION()

    uint32 l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE); {

	med.gl();

	glEnable(GL_TEXTURE_2D);

	{
	    const gh_fpoint a( tl.x, tl.y + radius.h);
	    const gh_fpoint b( br.x, tl.y + radius.h);
	    const gh_fpoint c( br.x, br.y - radius.h);
	    const gh_fpoint d( tl.x, br.y - radius.h);

	    const gh_fpoint a1(tl.x + radius.w, tl.y);
	    const gh_fpoint b1(br.x - radius.w, tl.y);
	    const gh_fpoint c1(br.x - radius.w, br.y);
	    const gh_fpoint d1(tl.x + radius.w, br.y);

	    const gh_fpoint a2(tl.x + radius.w, tl.y + radius.h);
	    const gh_fpoint b2(br.x - radius.w, tl.y + radius.h);
	    const gh_fpoint c2(br.x - radius.w, br.y - radius.h);
	    const gh_fpoint d2(tl.x + radius.w, br.y - radius.h);

	    //
	    // Draw the central flat square
	    //
	    gl_quad_uv(a1, b1, c1, d1, uv);

	    //
	    // Now side squares
	    //
	    gl_quad_uv(c2, c, b, b2, uv);
	    gl_quad_uv(a, a2, d2, d, uv);
	}

	const float step = M_360 / sides;

	float a;
	float x;
	float y;
	int i;

	//
	// Now draw the rounded filled edges, smaller, plain
	//

	//
	// Top right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = br.x - radius.w;
	    y = br.y - radius.h;
	    a = 0;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = br.x - radius.w;
	    y = tl.y + radius.h;
	    a = M_90;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom left
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = tl.x + radius.w;
	    y = tl.y + radius.h;
	    a = M_180;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = tl.x + radius.w;
	    y = br.y - radius.h;
	    a = M_270;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

    } glEndList();

    return (l);
}

//
// Draw a rounded square with 3D relief
//
uint32 gh_model::gl_list_rounded_square_bevelled (gh_fpoint tl,
						gh_fpoint br,
						gh_fsize uv,
						gh_pixel hi,
						gh_pixel med,
						gh_pixel lo,
						gh_fsize bevel,
						gh_fsize radius,
						int sides)
{
    GH_FUNCTION()

    uint32 l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE); {

	//
	//
	//
	//        x-----A-----------------------B----x
	//        |\               2                /|
	//        | \                              / |
	//        |  x-A1----------------------B1-x  |
	//        |  |                            |  |
	//        |  |                            |  |
	//        |  |             1              |  |
	//        |  |                            |  |
	//        |  |                            |  |
	//        |  |                            |  |
	//        |  |                            |  |
	//        |  x-C1----------------------D1-x  |
	//        | /              3               \ |
	//        |/                                \|
	//        x-----C-----------------------D----x
	//
	//
	//
	glEnable(GL_TEXTURE_2D);
	{
	    const gh_fpoint a( tl.x + bevel.w + radius.w, tl.y);
	    const gh_fpoint b( br.x - bevel.w - radius.w, tl.y);
	    const gh_fpoint c( br.x - bevel.w - radius.w, br.y);
	    const gh_fpoint d( tl.x + bevel.w + radius.w, br.y);
	    const gh_fpoint a1(tl.x + bevel.w + radius.w, tl.y + bevel.h);
	    const gh_fpoint b1(br.x - bevel.w - radius.w, tl.y + bevel.h);
	    const gh_fpoint c1(br.x - bevel.w - radius.w, br.y - bevel.h);
	    const gh_fpoint d1(tl.x + bevel.w + radius.w, br.y - bevel.h);

	    //
	    // Draw the central flat square (1)
	    //
	    med.gl();
	    gl_quad_uv(a1, b1, c1, d1, uv);

	    //
	    // Now draw the bevelled border (2)
	    //
	    hi.gl();
	    gl_quad_uv(c1, c, d, d1, uv);
	    gl_quad_uv(a, a1, d1, d, uv);

	    //
	    // (3)
	    //
	    lo.gl();
	    gl_quad_uv(a, b, b1, a1, uv);
	    gl_quad_uv(b, c, c1, b1, uv);
	}

	//
	//
	//
	//        x----------------------------------x
	//        |\                                /|
	//        | \                              / |
	//        |  x----------------------------x  |
	//        |  |                            |  |
	//        A  A1                          B1  B
	//        |  |                            |  |
	//        | 3|1            1             2|4 |
	//        |  |                            |  |
	//        C  C1                          D1  D
	//        |  |                            |  |
	//        |  x----------------------------x  |
	//        | /                              \ |
	//        |/                                \|
	//        x----------------------------------x
	//
	//
	//
	{
	    const gh_fpoint a(tl.x,            tl.y + bevel.h + radius.h);
	    const gh_fpoint b(br.x,            tl.y + bevel.h + radius.h);
	    const gh_fpoint c(br.x,            br.y - bevel.h - radius.h);
	    const gh_fpoint d(tl.x,            br.y - bevel.h - radius.h);

	    const gh_fpoint a1(tl.x + bevel.w, tl.y + bevel.h + radius.h);
	    const gh_fpoint b1(br.x - bevel.w, tl.y + bevel.h + radius.h);
	    const gh_fpoint c1(br.x - bevel.w, br.y - bevel.h - radius.h);
	    const gh_fpoint d1(tl.x + bevel.w, br.y - bevel.h - radius.h);

	    const gh_fpoint a2(tl.x + bevel.w + radius.w,
			       tl.y + bevel.h + radius.h);
	    const gh_fpoint b2(br.x - bevel.w - radius.w,
			       tl.y + bevel.h + radius.h);
	    const gh_fpoint c2(br.x - bevel.w - radius.w,
			       br.y - bevel.h - radius.h);
	    const gh_fpoint d2(tl.x + bevel.w + radius.w,
			       br.y - bevel.h - radius.h);

	    //
	    // Draw the central flat squares (1)
	    //
	    med.gl();
	    gl_quad_uv(a1, a2, d2, d1, uv);
	    gl_quad_uv(b2, b1, c1, c2, uv);

	    //
	    // Now draw the bevelled border
	    //
	
	    //
	    // (3)
	    //
	    hi.gl();
	    gl_quad_uv(a, a1, d1, d, uv);

	    //
	    // (4)
	    //
	    lo.gl();
	    gl_quad_uv(b, c, c1, b1, uv);
	}

	float step = M_360 / sides;

	float a;
	float x;
	float y;
	int i;

	//
	// Now draw the rounded filled edges, large first, with highlights
	//
	radius.w += bevel.w;
	radius.h += bevel.h;

	//
	// Top right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = br.x - radius.w;
	    y = br.y - radius.h;
	    a = 0;
	    i = 0;

	    hi.gl();
	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;

		if (i == sides / 8) {
		    lo.gl();
		}
	    }

	} glEnd();

	//
	// Bottom right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = br.x - radius.w;
	    y = tl.y + radius.h;
	    a = M_90;
	    i = 0;

	    lo.gl();
	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom left
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = tl.x + radius.w;
	    y = tl.y + radius.h;
	    a = M_180;
	    i = 0;

	    lo.gl();
	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;

		if (i == sides / 8) {
		    hi.gl();
		}
	    }

	} glEnd();

	//
	// Top right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = tl.x + radius.w;
	    y = br.y - radius.h;
	    a = M_270;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    hi.gl();

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	med.gl();

	//
	// Now draw the rounded filled edges, smaller, plain
	//
	radius.w -= bevel.w;
	radius.h -= bevel.h;

	//
	// Top right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = br.x - radius.w - bevel.w;
	    y = br.y - radius.h - bevel.h;
	    a = 0;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = br.x - radius.w - bevel.w;
	    y = tl.y + radius.h + bevel.h;
	    a = M_90;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom left
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = tl.x + radius.w + bevel.w;
	    y = tl.y + radius.h + bevel.h;
	    a = M_180;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();

	//
	// Bottom right
	//
	glBegin(GL_TRIANGLE_FAN); {

	    x = tl.x + radius.w + bevel.w;
	    y = br.y - radius.h - bevel.h;
	    a = M_270;
	    i = 0;

	    gl_vertex2uf_uv(x, y, uv);

	    while (i++ <= sides / 4) {

		gl_vertex2uf_uv(x + radius.w * sin(a),
				y + radius.h * cos(a), uv);

		a += step;
	    }

	} glEnd();
    } glEndList();

    return (l);
}

//
// Draw three slanted lines used for resize widgets
//
void gh_model::gl_list_shape (const char *shape[16], const gh_fsize uv,
			      const char start, const char end)
{
    GH_FUNCTION()

    char c;
    int x, y;

    for (c = start; c <= end; c++) {
	for (x=0; x<16; x++) {
	    for (y=0; y<16; y++) {
		if (shape[x][y] == c) {
		    gh_model::gl_vertex2uf_uv((1.0 / 16.0) * (float)x,
					      1.0 - (1.0 / 16.0) * (float)y, uv);
		    goto done;
		}
	    }
	}
	break;
done:
	continue;
    }
}

//
// Draw a resize icon
//
uint32 gh_model::gl_list_resize_icon (gh_fpoint tl,
				    gh_fpoint br,
				    gh_fsize uv,
				    gh_pixel hi,
				    gh_pixel med,
				    gh_pixel lo)
{
    GH_FUNCTION()

    static const char *shape[16] = {
	"             1  ",
	"                ",
	"               2",
	"                ",
	"                ",
	"                ",
	"                ",
	"                ",
	"              5 ",
	"                ",
	"                ",
	"                ",
	"               6",
	"0               ",
	"        4       ",
	"  3         7   ",
    };

    uint32 l;

    l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE);

	glBegin(GL_QUADS);
	    med.gl();
	    gl_list_shape(shape, uv, '0', '3');
	    gl_list_shape(shape, uv, '4', '7');
	glEnd();

    glEndList();

    return (l);
}

//
// Draw a close icon
//
uint32 gh_model::gl_list_close_icon (gh_fpoint tl,
				   gh_fpoint br,
				   gh_fsize uv,
				   gh_pixel hi,
				   gh_pixel med,
				   gh_pixel lo)
{
    GH_FUNCTION()

    static const char *shape[16] = {
	"   b        f   ",
	"                ",
	"                ",
	"a              g",
	"                ",
	"                ",
	"                ",
	"                ",
	"                ",
	"                ",
	"                ",
	"                ",
	"e              c",
	"                ",
	"                ",
	"   h        d   ",
    };

    uint32 l;

    l = glGenLists(1);

    if (!l) {
	GH_THROW("no list");
	return (0);
    }

    glNewList(l, GL_COMPILE);

	glBegin(GL_QUADS);
	    med.gl();
	    gl_list_shape(shape, uv, 'a', 'q');
	glEnd();

    glEndList();

    return (l);
}
