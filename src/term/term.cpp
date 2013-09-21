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

    $Id: term.cpp 1070 2008-08-11 23:30:42Z Goblinhack $
*/

#include <ghack.h>
#include <gh_console.h>
#include <gh_term.h>
#include <gh_figlet.h>
#include <gh_subsys.h>

int gh_term_screen_width;
int gh_term_screen_height;
int gh_term_x;
int gh_term_y;
gh_term_color gh_term_fg_current = GH_TERM_COLOR_WHITE;
gh_term_color gh_term_bg_current = GH_TERM_COLOR_BLACK;
gh_term_cell gh_term_cells[GH_TERM_MAX_SIZE][GH_TERM_MAX_SIZE/2];
figlet *font1;
figlet *font2;
gh_term_color random_fg;
gh_term_color random_bg;

bool gh_term_cursor_move_only;

void gh_term_init (void)
{
    GH_FUNCTION()

    string fontdir = gh_global::locatedir("fonts");

    font1 = new figlet(fontdir + "standard.flf");
    if (font1 == NULL) {
	GH_THROW("can't load fonts");
    }

    font2 = new figlet(fontdir + "script.flf");

    if (font2 == NULL) {
	GH_THROW("can't load fonts");
    }

    //
    // Set up our terminal we use for debugging and logging
    //
    GH_TERM_WIDTH = 100;
    GH_TERM_HEIGHT = 46;
}

void gh_term_exit (void)
{
    GH_FUNCTION()

    delete font1;
    delete font2;
}

void gh_term_scroll (void)
{
    GH_FUNCTION()

    int x, y;

    gh_term_refresh();

    for (y = 0; y < GH_TERM_HEIGHT - 1; y++) {
	for (x = 0; x < GH_TERM_WIDTH; x++) {
	    gh_term_cell *c = &gh_term_cells[x][y];
	    gh_term_cell *d = &gh_term_cells[x][y+1];

	    memcpy(c,d,sizeof(*c));
	}
    }

    for (x = 0; x < GH_TERM_WIDTH; x++) {
	gh_term_cell *c = &gh_term_cells[x][y];
	memset(c, 0, sizeof(*c));
    }
}

gh_term_color gh_term_color_string_to_index (const char **s)
{
    GH_FUNCTION()

    if (!strncmp(*s, "black", sizeof("black")-1)) {
	*s += sizeof("black")-1;
	return (GH_TERM_COLOR_BLACK);
    }
    if (!strncmp(*s, "red", sizeof("red")-1)) {
	*s += sizeof("red")-1;
	return (GH_TERM_COLOR_RED);
    }
    if (!strncmp(*s, "darkred", sizeof("darkred")-1)) {
	*s += sizeof("darkred")-1;
	return (GH_TERM_COLOR_RED);
    }
    if (!strncmp(*s, "gray", sizeof("gray")-1)) {
	*s += sizeof("gray")-1;
	return (GH_TERM_COLOR_GRAY);
    }
    if (!strncmp(*s, "green", sizeof("green")-1)) {
	*s += sizeof("green")-1;
	return (GH_TERM_COLOR_GREEN);
    }
    if (!strncmp(*s, "darkgreen", sizeof("darkgreen")-1)) {
	*s += sizeof("darkgreen")-1;
	return (GH_TERM_COLOR_DARK_GREEN);
    }
    if (!strncmp(*s, "yellow", sizeof("yellow")-1)) {
	*s += sizeof("yellow")-1;
	return (GH_TERM_COLOR_YELLOW);
    }
    if (!strncmp(*s, "blue", sizeof("blue")-1)) {
	*s += sizeof("blue")-1;
	return (GH_TERM_COLOR_BLUE);
    }
    if (!strncmp(*s, "darkblue", sizeof("darkblue")-1)) {
	*s += sizeof("darkblue")-1;
	return (GH_TERM_COLOR_DARK_BLUE);
    }
    if (!strncmp(*s, "pink", sizeof("pink")-1)) {
	*s += sizeof("pink")-1;
	return (GH_TERM_COLOR_PINK);
    }
    if (!strncmp(*s, "cyan", sizeof("cyan")-1)) {
	*s += sizeof("cyan")-1;
	return (GH_TERM_COLOR_CYAN);
    }
    if (!strncmp(*s, "white", sizeof("white")-1)) {
	*s += sizeof("white")-1;
	return (GH_TERM_COLOR_WHITE);
    }
    if (!strncmp(*s, "purple", sizeof("purple")-1)) {
	*s += sizeof("purple")-1;
	return (GH_TERM_COLOR_PURPLE);
    }
    if (!strncmp(*s, "orange", sizeof("orange")-1)) {
	*s += sizeof("orange")-1;
	return (GH_TERM_COLOR_ORANGE);
    }

    return (GH_TERM_COLOR_WHITE);
}

void gh_term_putf (const char *s)
{
    GH_FUNCTION()

    static uint32 last_boost_tick_when;
    static bool which;

    if (!last_boost_tick_when) {
	last_boost_tick_when = gh_get_time_milli();
    }

    if (gh_have_x_tenths_passed_since(5, last_boost_tick_when)) {

	last_boost_tick_when = gh_get_time_milli();
	which = !which;
    }

    char c;
    bool looking_for_start = false;

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
		    gh_term_fg(GH_TERM_COLOR_RANDOM);
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg=random", 9)) {
		    s += 9;
		    gh_term_bg(GH_TERM_COLOR_RANDOM);
		    looking_for_start = false;
		    continue;
		}

		//
		// fg1/fg2 are colors we alternate between
		//
		if (!strncmp(s, "fg1=", 4)) {
		    s += 4;
		    if (which) {
			gh_term_fg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "fg2=", 4)) {
		    s += 4;
		    if (!which) {
			gh_term_fg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg1=", 4)) {
		    s += 4;
		    if (which) {
			gh_term_bg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg2=", 4)) {
		    s += 4;
		    if (!which) {
			gh_term_bg(gh_term_color_string_to_index(&s));
		    } else {
			gh_term_color_string_to_index(&s);
		    }
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "fg=", 3)) {
		    s += 3;
		    gh_term_fg(gh_term_color_string_to_index(&s));
		    looking_for_start = false;
		    continue;
		}

		if (!strncmp(s, "bg=", 3)) {
		    s += 3;
		    gh_term_bg(gh_term_color_string_to_index(&s));
		    looking_for_start = false;
		    continue;
		}
	    }
	    gh_term_putc(c);
	}
	looking_for_start = false;

	gh_term_putc(c);
    }
}

int gh_term_length (const char *s)
{
    GH_FUNCTION()

    int len = 0;
    char c;
    bool looking_for_start = false;

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
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		if (!strncmp(s, "bg=random", 9)) {
		    s += 9;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		//
		// fg1/fg2 are colors we alternate between
		//
		if (!strncmp(s, "fg1=", 4)) {
		    s += 4;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		if (!strncmp(s, "fg2=", 4)) {
		    s += 4;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		if (!strncmp(s, "bg1=", 4)) {
		    s += 4;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		if (!strncmp(s, "bg2=", 4)) {
		    s += 4;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		if (!strncmp(s, "fg=", 3)) {
		    s += 3;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}

		if (!strncmp(s, "bg=", 3)) {
		    s += 3;
		    looking_for_start = false;
		    gh_term_color_string_to_index(&s);
		    continue;
		}
	    }
	}

	looking_for_start = false;

	len++;
    }

    return (len);
}

void gh_term_putfy (int y, const char *s)
{
    GH_FUNCTION()

    gh_term_goto(0, y);

    gh_term_cursor_move_only = true;
    gh_term_putf(s);
    gh_term_cursor_move_only = false;

    gh_term_goto(GH_TERM_WIDTH/2 - gh_term_x/2, y);
    gh_term_putf(s);
}

void gh_term_putfmx (const char *s)
{
    GH_FUNCTION()

    gh_term_putfy(gh_term_y, s);
}

void gh_term_text_box (list<string> &l,
		       gh_term_color bg,
		       gh_term_color fg,
		       gh_term_color border_bg,
		       gh_term_color border_fg,
		       char border_char,
		       int justify // < 0, 0, > 0
		       )
{
    GH_FUNCTION()

    list<string>::iterator i;

    int max_width = 0;
    int max_height = 0;

    i = l.begin();
    while (i != l.end()) {
	string s = *i;

	max_width = max(max_width, (int)gh_term_length(s.c_str()));

	i++;

	max_height++;
    }

    int start_y = GH_TERM_HEIGHT / 2 - max_height / 2;
    int end_y   = start_y + max_height;
    int start_x = GH_TERM_WIDTH / 2 - max_width / 2;
    int end_x   = start_x + max_width;

    for (int x = start_x; x < end_x; x++) {
	for (int y = start_y; y < end_y; y++) {
	    gh_term_goto(x,y);
	    gh_term_bg(bg);
	    gh_term_fg(fg);
	    gh_term_putc(' ');
	}
    }

    start_x --;
    start_y --;

    gh_term_bg(border_bg);
    gh_term_fg(border_fg);

    for (int x = start_x; x <= end_x; x++) {
	gh_term_goto(x, start_y);
	gh_term_putc(border_char);
	gh_term_goto(x, end_y);
	gh_term_putc(border_char);
    }

    for (int y = start_y; y <= end_y; y++) {
	gh_term_goto(start_x, y);
	gh_term_putc(border_char);
	gh_term_goto(end_x, y);
	gh_term_putc(border_char);
    }

    start_y++;
    i = l.begin();
    while (i != l.end()) {
	string s = *i;

	if (justify < 0) {
	    gh_term_goto(start_x + 1, start_y);
	    gh_term_putf(s.c_str());
	} else if (justify > 0) {
	    gh_term_goto(end_x + s.length(), start_y);
	    gh_term_putf(s.c_str());
	} else {
	    gh_term_putfy(start_y, s.c_str());
	}

	i++;

	start_y++;
    }
}

