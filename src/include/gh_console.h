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

    $Id: gh_console.h 545 2007-08-06 17:47:55Z goblinhack $
*/

#ifndef _GH_CONSOLE_H_
#define _GH_CONSOLE_H_

#include <gh_collage.h>
#include <gh_term.h>

//
// Fixed-width console wrapper for collage class
//
class gh_console
{
public:
    gh_point cursor;
    const gh_pixel *pixel_fg;
    const gh_pixel *pixel_bg;
    gh_collage *font[1];
    gh_pixel random_color;
    int console_tex[128][GH_TERM_COLOR_MAX][GH_TERM_COLOR_MAX];

    gh_console() {}

    void init(gh_collage * c);

    void bg(uint8 c);
    void fg(uint8 c);
    void fgbg(uint8, uint8);
    void putc(char c);
    void putc_small(char c);
    void puts_at(string s, gh_point3d at);
    void putf_at(string s, gh_point3d at);
    void mv(gh_point cursor);

    int char_cache(const char c,
		   const char fg,
		   const char bg);

    gh_inline void my_gl_quad(const char ch,
			      const gh_fpoint &tl, const gh_fpoint &br);

    void char_at(const char c,
		 const gh_fpoint &tl,
		 const gh_fpoint &br,
		 const char fg,
		 char bg,
		 bool blend = false,
		 gh_pixel shade = GH_WHITE);

    void char_at(const char c,
		 const gh_fpoint &tl,
		 const gh_fpoint &br,
		 const gh_pixel *fg,
		 const gh_pixel *bg,
		 bool blend = false,
		 gh_pixel shade = GH_WHITE);

    const gh_pixel *color_to_pixel(uint8 a);

    void fixed_text_at(const char c,
		       const gh_fpoint3d v,
		       const gh_fsize &scale,
		       const gh_pixel *fg,
		       const gh_pixel *bg);
};

extern gh_console console;

extern gh_term_color gh_term_color_string_to_index(const char **s);

gh_inline void gh_console::bg (uint8 c)
{
    pixel_bg = color_to_pixel(c);
}

gh_inline void gh_console::fg (uint8 c)
{
    pixel_fg = color_to_pixel(c);
}

gh_inline void gh_console::fgbg (uint8 c, uint8 d)
{
    pixel_fg = color_to_pixel(c);
    pixel_bg = color_to_pixel(d);
}

gh_inline void gh_console::mv (gh_point c)
{
    cursor.x = c.x;
    cursor.y = c.y;
}

#else
#error reinclusion
#endif /* _GH_CONSOLE_H_ */
