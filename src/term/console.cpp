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

    $Id: console.cpp 1288 2009-08-12 15:44:29Z Goblinhack $
*/

#include <ghack.h>
#include <gh_surface.h>
#include <gh_console.h>
#include <gh_term.h>
#include <gh_subsys.h>

gh_console console;

//
// Fonts to display console with
//
void gh_console::init (gh_collage * c1)
{
    font[0] = c1;
    cursor = gh_point(0,0);
    pixel_fg = &GH_WHITE;
    pixel_bg = &GH_BLACK;
    random_color.r = rand() % 255;
    random_color.g = rand() % 255;
    random_color.b = rand() % 255;
    GH_STRUCT_INIT(console_tex);
}

//
// OpenGL is slow at blending onto a different colored background, so
// we cache all the char/color combos we'll need
//
int gh_console::char_cache (const char c,
			    const char fg,
			    const char bg)
{
    int *tex;

    tex = &console_tex[(int)c][(int)fg][(int)bg];
    if (*tex) {
	return (*tex);
    }

    glGenTextures(1, (GLuint*) tex);

    gh_fpoint tl(0,0);
    gh_fpoint br(32,32);

    GH_WHITE.gl();

    char_at(c, tl, br, color_to_pixel(fg), color_to_pixel(bg));

    glBindTexture(GL_TEXTURE_2D, *tex);

    //
    // NEAREST is quicker than LINEAR
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glCopyTexImage2D(GL_TEXTURE_2D, 0, // level of detail
		     GL_RGB,
		     (int)tl.x,
		     gh_global::screensize.h - (int)br.y,
		     (int)br.x,
		     (int)br.y,
		     0 // border
		     );
    return (*tex);
}

void gh_console::char_at (const char c,
			  const gh_fpoint &tl,
			  const gh_fpoint &br,
			  const char fg,
			  char bg,
			  bool blend,
			  gh_pixel shade)
{
    /*
     * We can draw black backgrounds quickly with the same
     * texture. Random colors, we just give up on caching.
     */
    if (blend ||
	(bg == GH_TERM_COLOR_BLACK)  ||
	(fg == GH_TERM_COLOR_RANDOM) ||
	(bg == GH_TERM_COLOR_RANDOM)) {

	//
	// No caching, slow mode
	//
	char_at(c, tl, br,
		color_to_pixel(fg),
		color_to_pixel(bg),
		blend,
		shade);
	return;
    }

    GH_WHITE.gl();

    /*
     * Make a tex cache for this color combo.
     */
    int tex = console_tex[(int)c][(int)fg][(int)bg];
    if (!tex) {
	tex = char_cache(c, fg, bg);
    }

    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(tl.x, br.y);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(br.x, br.y);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(tl.x, tl.y);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(br.x, tl.y);
    glEnd();
}

gh_inline void
gh_console::my_gl_quad (const char ch,
			const gh_fpoint &tl, const gh_fpoint &br)
{
    const gh_tex * t = &font[0]->images[ch];
    //
    // We trim the letter textures so they look closer together - a hack but
    // a nice looking hack
    //
    static const float dx1 = 0.012;
    static const float dx2 = 0.013;
    static const float dy1 = 0.019;
    static const float dy2 = 0.024;

    glBegin(GL_TRIANGLE_STRIP);				
	//						
	// X .						
	// . .						
	//						
	glTexCoord2f(t->x1 + dx1, t->y1 + dy1);		
	glVertex2f(tl.x, br.y);				
	//						
	// . X						
	// . .						
	//						
	glTexCoord2f(t->x2 - dx2, t->y1 + dy1);		
	glVertex2f(br.x, br.y);				
	//						
	// . .						
	// X .						
	//						
	glTexCoord2f(t->x1 + dx1, t->y2 - dy2);		
	glVertex2f(tl.x, tl.y);				
	//						
	// . .						
	// . X						
	//						
	glTexCoord2f(t->x2 - dx2, t->y2 - dy2);		
	glVertex2f(br.x, tl.y);				
    glEnd();						
}

gh_inline void gh_console::char_at (const char c,
				    const gh_fpoint &tl,
				    const gh_fpoint &br,
				    const gh_pixel *fg,
				    const gh_pixel *bg,
				    bool blend,
				    gh_pixel shade)
{
    const char ch = c - ' ';

    if (!blend) {
	if (bg == &GH_BLACK) {
	    //
	    // Opengl sucks at blending; speed wise. So, limit the number
	    // of things that we need to blend with. Black backgrounds can
	    // be ignored.
	    //
	    if (c == ' ') {
		return;
	    }

	    //
	    // If a color has been given to shade this thing, merge
	    // it in with the char color.
	    //
	    if ((shade.r != 255) || (shade.g != 255) || (shade.b != 255)) {
		gh_pixel color = *fg;

		if (shade.r != 255) {
		    color.r = shade.r;
		}

		if (shade.g != 255) {
		    color.g = shade.g;
		}

		if (shade.b != 255) {
		    color.b = shade.b;
		}

		color.gl();

	    } else {
		fg->gl();
	    }

	    glBindTexture(GL_TEXTURE_2D, font[0]->surface->tex);
	    my_gl_quad(ch, tl, br);
	    return;
	}

	//
	// If a color has been given to shade this thing, merge
	// it in with the char color.
	//
	if ((shade.r != 255) || (shade.g != 255) || (shade.b != 255)) {
	    gh_pixel color = *bg;

	    if (shade.r != 255) {
		color.r = shade.r;
	    }

	    if (shade.g != 255) {
		color.g = shade.g;
	    }

	    if (shade.b != 255) {
		color.b = shade.b;
	    }

	    color.gl();
	} else {
	    bg->gl();
	}

	glBindTexture(GL_TEXTURE_2D, font[0]->mask->tex);
	my_gl_quad(ch, tl, br);
    }

    //
    // If we don't need to blend in a foreground, then skip it.
    //
    if (c == ' ') {
	if (fg == &GH_BLACK) {
	    return;
	}
    }

    if ((shade.r != 255) || (shade.g != 255) || (shade.b != 255)) {
	gh_pixel color = *fg;

	if (shade.r != 255) {
	    color.r = shade.r;
	}

	if (shade.g != 255) {
	    color.g = shade.g;
	}

	if (shade.b != 255) {
	    color.b = shade.b;
	}

	color.gl();
    } else {
	fg->gl();
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    glBindTexture(GL_TEXTURE_2D, font[0]->surface->tex);
	my_gl_quad(ch, tl, br);
    glDisable(GL_BLEND);
}

gh_inline void gh_console::fixed_text_at (const char c,
					  const gh_fpoint3d v,
					  const gh_fsize &scale,
					  const gh_pixel *fg,
					  const gh_pixel *bg)
{
    const char ch = c - ' ';
    const gh_point *at = &font[0]->at[ch];
    const gh_point *bt = &font[0]->bt[ch];

    float width;
    float height;

    if (gh_global::screensize.w >= 1280) {
	//
	// 1280x1024
	//
	if (gh_global::screensize.h >= 1024) {
	    width = ((float) (bt->x - at->x)) / 5.0;
	    height = ((float) (bt->y - at->y)) / 3.0;
	} else {
	    //
	    // Fancy laptop, 1280x800
	    //
	    width = ((float) (bt->x - at->x)) / 5.0;
	    height = ((float) (bt->y - at->y)) / 3.5;
	}
    } else if (gh_global::screensize.w >= 1024) {
	//
	// 1024x800
	//
	width = ((float) (bt->x - at->x)) / 6.0;
	height = ((float) (bt->y - at->y)) / 4.0;
    } else if (gh_global::screensize.w >= 800) {
	//
	// 800x600
	//
	width = ((float) (bt->x - at->x)) / 7.0;
	height = ((float) (bt->y - at->y)) / 5.0;
    } else {
	//
	// 640x480
	//
	width = ((float) (bt->x - at->x)) / 9.0;
	height = ((float) (bt->y - at->y)) / 7.0;
    }

    gh_fpoint tl(v.x, v.y);
    gh_fpoint br(tl.x + width, tl.y + height);

    char_at(c, tl, br, fg, bg);

    cursor.x++;
}

gh_inline void gh_console::myputc (char c)
{
    const float dx = (float)gh_global::screensize.w / (float)GH_TERM_WIDTH;
    const float dy = (float)gh_global::screensize.h / (float)GH_TERM_HEIGHT;

    console.fixed_text_at(
		c,
		gh_fpoint3d(dx * cursor.x, dy * cursor.y, 0.0),
		gh_fsize(dx, dy),
		pixel_fg, pixel_bg);
}

void gh_console::puts_at (string s, gh_point3d at)
{
    const float dx = (float)gh_global::screensize.w / (float)GH_TERM_WIDTH;
    const float dy = (float)gh_global::screensize.h / (float)GH_TERM_HEIGHT;

    gh_fpoint3d fat(at.x - (s.size() / 2) * dx, at.y, at.z);

    for (size_t x = 0; x < s.size(); x++) {
	console.fixed_text_at(
		    s[x],
		    fat,
		    gh_fsize(dx, dy),
		    pixel_fg, pixel_bg);

	fat.x += dx;
    }
}

void gh_console::putf_at (string text, gh_point3d at)
{
    GH_FUNCTION()

    static uint32 last_boost_tick_when;
    static bool which;

    //
    // This is to make the colors flash
    // 
    if (!last_boost_tick_when) {
	last_boost_tick_when = gh_get_time_milli();
    }

    if (gh_have_x_tenths_passed_since(5, last_boost_tick_when)) {

	last_boost_tick_when = gh_get_time_milli();
	which = !which;
    }

    char c;
    const char *s = text.c_str();
    bool looking_for_start = false;

    const float dx = (float)gh_global::screensize.w / (float)GH_TERM_WIDTH;
    const float dy = (float)gh_global::screensize.h / (float)GH_TERM_HEIGHT;

    gh_fpoint3d fat(at.x - (gh_term_length(s) / 2) * dx, at.y, at.z);

    while ((c = *s++) != '\0') {

	if (!looking_for_start) {
	    if (c == '%') {
		looking_for_start = true;
		continue;
	    }
	} else if (looking_for_start) {
	    if (c == '%') {
		//
		// Got a format string.
		//
		if (!strncmp(s, "fg=random", 9)) {
		    s += 9;
		    console.fg(GH_TERM_COLOR_RANDOM);
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg=random", 9)) {
		    s += 9;
		    console.bg(GH_TERM_COLOR_RANDOM);
		    looking_for_start = false;
		    continue;
		}

		//
		// fg1/fg2 are colors we alternate between
		//
		if (!strncmp(s, "fg1=", 4)) {
		    s += 4;
		    if (which) {
			console.fg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "fg2=", 4)) {
		    s += 4;
		    if (!which) {
			console.fg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg1=", 4)) {
		    s += 4;
		    if (which) {
			console.bg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg2=", 4)) {
		    s += 4;
		    if (!which) {
			console.bg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "fg=", 3)) {
		    s += 3;
		    console.fg(gh_term_color_string_to_index(&s));
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg=", 3)) {
		    s += 3;
		    console.bg(gh_term_color_string_to_index(&s));
		    looking_for_start = false;
		    continue;
		}
	    }

	    console.fixed_text_at(
			c,
			fat,
			gh_fsize(dx, dy),
			pixel_fg, pixel_bg);

	    fat.x += dx;

	}

	looking_for_start = false;

	console.fixed_text_at(
		    c,
		    fat,
		    gh_fsize(dx, dy),
		    pixel_fg, pixel_bg);

	fat.x += dx;
    }
}

void gh_term_refresh (void)
{
    GH_FUNCTION()

    gh_point p;

    for (p.x = 0; p.x < GH_TERM_WIDTH; p.x++) {

	gh_term_cell *c = &gh_term_cells[p.x][0];

	for (p.y = 0; p.y < GH_TERM_HEIGHT; p.y++, c++) {

	    char ch = c->c;

	    if (ch) {
		if (ch == ' ') {
		    if (c->bg == GH_TERM_COLOR_BLACK) {
			continue;
		    }
		}
		console.fg(c->fg);
		console.bg(c->bg);

		console.mv(p);
		console.myputc(ch);
	    }
	}
    }

    //
    // We've cleared the screen. Next loop, we need to draw again.
    //
    gh_term_cls();
}

const gh_pixel *gh_console::color_to_pixel (uint8 a)
{
    static gh_pixel random_color;

    switch (a) {
    case GH_TERM_COLOR_BLACK:
	return (&GH_BLACK);

    case GH_TERM_COLOR_RED:
	return (&GH_RED);

    case GH_TERM_COLOR_DARK_RED:
	return (&GH_DARK_RED);

    case GH_TERM_COLOR_GREEN:
	return (&GH_GREEN);

    case GH_TERM_COLOR_DARK_GREEN:
	return (&GH_FORESTGREEN);

    case GH_TERM_COLOR_YELLOW:
	return (&GH_YELLOW);

    case GH_TERM_COLOR_DARK_BLUE:
	return (&GH_SLATEBLUE4);

    case GH_TERM_COLOR_BLUE:
	return (&GH_SLATEBLUE3);

    case GH_TERM_COLOR_PINK:
	return (&GH_PINK);

    case GH_TERM_COLOR_CYAN:
	return (&GH_CYAN2);

    case GH_TERM_COLOR_WHITE:
	return (&GH_WHITE);

    case GH_TERM_COLOR_GRAY:
	return (&GH_SLATE_GRAY);

    case GH_TERM_COLOR_ORANGE:
	return (&GH_ORANGE);

    case GH_TERM_COLOR_LIGHT_ORANGE:
	return (&GH_LIGHT_ORANGE);

    case GH_TERM_COLOR_LIME:
	return (&GH_LIME_GREEN);

    case GH_TERM_COLOR_LIGHT_YELLOW:
	return (&GH_YELLOW2);

    case GH_TERM_COLOR_LIGHT_BLUE:
	return (&GH_SLATEBLUE1);

    case GH_TERM_COLOR_PURPLE:
	return (&GH_PURPLE);

    case GH_TERM_COLOR_LIGHT_CYAN:
	return (&GH_CYAN);

    case GH_TERM_COLOR_SNOW:
	return (&GH_SNOW);

    case GH_TERM_COLOR_MAX:
    case GH_TERM_COLOR_RANDOM:
	random_color.r = rand() % 255;
	random_color.g = rand() % 255;
	random_color.b = rand() % 255;
	return ((const gh_pixel*)&random_color);

    case GH_TERM_COLOR_GRAY_0:
	return (&GH_GRAY5);

    case GH_TERM_COLOR_GRAY_1:
	return (&GH_GRAY10);

    case GH_TERM_COLOR_GRAY_2:
	return (&GH_GRAY20);

    case GH_TERM_COLOR_GRAY_3:
	return (&GH_GRAY30);

    case GH_TERM_COLOR_GRAY_4:
	return (&GH_GRAY40);

    case GH_TERM_COLOR_GRAY_5:
	return (&GH_GRAY50);

    case GH_TERM_COLOR_GRAY_6:
	return (&GH_GRAY60);

    case GH_TERM_COLOR_GRAY_7:
	return (&GH_GRAY70);

    case GH_TERM_COLOR_GRAY_8:
	return (&GH_GRAY80);

    case GH_TERM_COLOR_GRAY_9:
	return (&GH_GRAY90);

    case GH_TERM_COLOR_BROWN:
	return (&GH_SADDLE_BROWN);
    }
    return (&GH_WHITE);
}
