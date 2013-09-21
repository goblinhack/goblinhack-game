/*
    ghack.
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

    $Id: gh_term.h 543 2007-08-04 18:19:19Z goblinhack $
*/

#ifndef _GH_TERM_H_
#define _GH_TERM_H_

#include "gh_macro.h"

typedef enum gh_term_color_ {
    GH_TERM_COLOR_BLACK,
    GH_TERM_COLOR_RED,
    GH_TERM_COLOR_DARK_RED,
    GH_TERM_COLOR_GREEN,
    GH_TERM_COLOR_DARK_GREEN,
    GH_TERM_COLOR_YELLOW,
    GH_TERM_COLOR_BLUE,
    GH_TERM_COLOR_PINK,
    GH_TERM_COLOR_CYAN,
    GH_TERM_COLOR_WHITE,
    GH_TERM_COLOR_GRAY,
    GH_TERM_COLOR_ORANGE,
    GH_TERM_COLOR_LIGHT_ORANGE,
    GH_TERM_COLOR_LIME,
    GH_TERM_COLOR_LIGHT_YELLOW,
    GH_TERM_COLOR_LIGHT_BLUE,
    GH_TERM_COLOR_PURPLE,
    GH_TERM_COLOR_LIGHT_CYAN,
    GH_TERM_COLOR_SNOW,
    GH_TERM_COLOR_GRAY_0,
    GH_TERM_COLOR_GRAY_1,
    GH_TERM_COLOR_GRAY_2,
    GH_TERM_COLOR_GRAY_3,
    GH_TERM_COLOR_GRAY_4,
    GH_TERM_COLOR_GRAY_5,
    GH_TERM_COLOR_GRAY_6,
    GH_TERM_COLOR_GRAY_7,
    GH_TERM_COLOR_GRAY_8,
    GH_TERM_COLOR_GRAY_9,
    GH_TERM_COLOR_BROWN,
    GH_TERM_COLOR_DARK_BLUE,
    GH_TERM_COLOR_RANDOM,

    GH_TERM_COLOR_MAX,

} gh_term_color;

#define GH_TERM_MAX_SIZE 100
#define GH_TERM_HEIGHT gh_term_screen_height
#define GH_TERM_WIDTH gh_term_screen_width

typedef struct gh_term_cell_ {
    char	    c;
    //
    // Indicates that this cell has been modified since the last
    // frame was drawn.
    //
    uint8	    touched:1;
    //
    // Foreground and background
    //
    gh_term_color   fg;
    gh_term_color   bg;
} gh_term_cell;

extern gh_term_cell gh_term_cells[GH_TERM_MAX_SIZE][GH_TERM_MAX_SIZE/2];

extern int gh_term_x;
extern int gh_term_y;
extern int GH_TERM_WIDTH;
extern int GH_TERM_HEIGHT;
extern gh_term_color gh_term_fg_current;
extern gh_term_color gh_term_bg_current;
extern bool gh_term_cursor_move_only;
extern gh_term_color random_fg;
extern gh_term_color random_bg;

extern void gh_term_init(void);
extern void gh_term_exit(void);
extern void gh_term_refresh(void);
extern void gh_term_scroll(void);
extern void gh_term_refresh_small(void);

static inline bool gh_term_pos_valid (int x, int y)
{
    if (x >= GH_TERM_WIDTH) {
	return (false);
    }

    if (x < 0) {
	return (false);
    }

    if (y >= GH_TERM_HEIGHT) {
	return (false);
    }

    if (y < 0) {
	return (false);
    }

    return (true);
}

static inline void gh_term_put (gh_term_cell *e)
{
    if (gh_term_cursor_move_only) {
	gh_term_x++;
	return;
    }

    if (gh_term_x >= GH_TERM_WIDTH) {
	gh_term_x++;
	return;
    }

    if (gh_term_x < 0) {
	gh_term_x++;
	return;
    }

    if (gh_term_y >= GH_TERM_HEIGHT) {
	gh_term_x++;
	return;
    }

    if (gh_term_y < 0) {
	gh_term_x++;
	return;
    }

    gh_term_cell *o = &gh_term_cells[gh_term_x][gh_term_y];

    e->touched = true;

    *o = *e;
    gh_term_x++;
}

static inline void gh_term_putc (gh_term_cell *e)
{
    gh_term_cell *o = &gh_term_cells[gh_term_x][gh_term_y];

    e->touched = true;

    *o = *e;
}

static inline char gh_term_getc (void)
{
    if (gh_term_cursor_move_only) {
	gh_term_x++;
	return ('\0');
    }

    if (gh_term_x >= GH_TERM_WIDTH) {
	gh_term_x++;
	return ('\0');
    }

    if (gh_term_x < 0) {
	gh_term_x++;
	return ('\0');
    }

    if (gh_term_y >= GH_TERM_HEIGHT) {
	gh_term_x++;
	return ('\0');
    }

    if (gh_term_y < 0) {
	gh_term_x++;
	return ('\0');
    }

    return (gh_term_cells[gh_term_x][gh_term_y].c);
}

static inline void gh_term_cursor_right (void)
{
    gh_term_x++;
}

static inline void gh_term_putc (const char c)
{
    static gh_term_cell e = {0};

    e.c = c; 
    e.fg = gh_term_fg_current;
    e.bg = gh_term_bg_current;

    gh_term_put(&e);
}

static inline void gh_term_puts (const char* s)
{
    char c;

    while ((c = *s++) != '\0') {
	gh_term_putc(c);
    }
}

//
// Formatted puts, with %%fg=yellow being the format style
//
extern void gh_term_putf (const char *s);

//
// Formatted and centered on the x axis
//
extern void gh_term_putfy (int y, const char *s);

//
// Current y, x middle
//
extern void gh_term_putfmx (const char *s);

static inline void gh_term_fill_c (const char c)
{
    int x, y;

    for (x = 0; x < GH_TERM_WIDTH; x++) {
	for (y = 0; y < GH_TERM_HEIGHT; y++) {
	    gh_term_cells[x][y].c = c;
	    gh_term_cells[x][y].touched = true;
	}
    }
}

static inline void gh_term_fill_fg (const gh_term_color fg)
{
    int x, y;

    for (x = 0; x < GH_TERM_WIDTH; x++) {
	for (y = 0; y < GH_TERM_HEIGHT; y++) {
	    gh_term_cells[x][y].fg = fg;
	    gh_term_cells[x][y].touched = true;
	}
    }
}

static inline void gh_term_fill_line (const gh_term_color fg,
				      const gh_term_color bg)
{
    int x;

    for (x = 0; x < GH_TERM_WIDTH; x++) {
	gh_term_cells[x][gh_term_y].fg = fg;
	gh_term_cells[x][gh_term_y].bg = bg;
	gh_term_cells[x][gh_term_y].c = ' ';
	gh_term_cells[x][gh_term_y].touched = true;
    }
}

static inline void gh_term_fill_bg (const gh_term_color bg)
{
    int x, y;

    for (x = 0; x < GH_TERM_WIDTH; x++) {
	for (y = 0; y < GH_TERM_HEIGHT; y++) {
	    gh_term_cells[x][y].bg = bg;
	    gh_term_cells[x][y].touched = true;
	}
    }
}

static inline void gh_term_fg (gh_term_color fg)
{
    if (fg == GH_TERM_COLOR_RANDOM) {
	fg = random_fg;
    }
    gh_term_fg_current = fg;
}

static inline void gh_term_bg (gh_term_color bg)
{
    if (bg == GH_TERM_COLOR_RANDOM) {
	bg = random_bg;
    }
    gh_term_bg_current = bg;
}

static inline void gh_term_fgbg (gh_term_color fg,
				 gh_term_color bg)
{
    if (fg == GH_TERM_COLOR_RANDOM) {
	fg = random_fg;
    }

    if (bg == GH_TERM_COLOR_RANDOM) {
	bg = random_bg;
    }

    gh_term_fg_current = fg;
    gh_term_bg_current = bg;
}

static inline void gh_term_goto (int x, int y)
{
    gh_term_x = x;
    gh_term_y = y;
}

static inline void gh_term_cls (void)
{
    gh_term_fill_c(' ');
    gh_term_fill_bg(GH_TERM_COLOR_BLACK);
    gh_term_fill_fg(GH_TERM_COLOR_BLACK);
}

extern int gh_term_length(const char *s);

extern void gh_term_text_box(list<string> &l,
			     gh_term_color bg,
			     gh_term_color fg,
			     gh_term_color border_bg,
			     gh_term_color border_fg,
			     char border_char,
			     int justify // < 0, 0, > 0
			     );

#endif /* _GH_TERM_H_ */
