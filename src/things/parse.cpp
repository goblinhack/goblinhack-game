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

    $Id: gfx.cpp 212 2006-10-21 15:32:50Z goblinhack $
*/

#include <ghack.h>
#include <gh_term.h>
#include <gh_thing.h>
#include <gh_console.h>
#include <gh_math.h>

#define nDEBUG_PARSE_FILE

//
// Generate strings corresponding to the GH_THING_MODE values.
//
DECLARE_ENUM_STRINGS(gh_thing_mode, GH_THING_MODE)

//
// Generate strings corresponding to the GH_THING_DIR values.
//
DECLARE_ENUM_STRINGS(gh_thing_dir, GH_THING_DIR)

//
// Map a string to an index.
//
static int gh_thing_mode_string_index (const char *str)
{
    GH_FUNCTION()

    const char *const*iterator;

    GH_FOR_ALL_IN_ARRAY(iterator, gh_thing_mode_strings) {
        if (!strcmp(str, *iterator)) {
            return (iterator - gh_thing_mode_strings);
        }
    }
    return (-1);
}

//
// Map an index to a string.
//
/* static */ const char * gh_thing_mode_string (const int index)
{
    GH_FUNCTION()

    if (index >= (decltype(index))GH_ARRAY_SIZE(gh_thing_mode_strings)) {
        return (NULL);
    }
    return (gh_thing_mode_strings[index]);
}

//
// Map a string to an index.
//
static int gh_thing_dir_string_index (const char *str)
{
    GH_FUNCTION()

    const char *const*iterator;

    GH_FOR_ALL_IN_ARRAY(iterator, gh_thing_dir_strings) {
        if (!strcmp(str, *iterator)) {
            return (iterator - gh_thing_dir_strings);
        }
    }
    return (-1);
}

//
// Map an index to a string.
//
/* static */ const char * gh_thing_dir_string (const int index)
{
    GH_FUNCTION()

    if (index >= (decltype(index))GH_ARRAY_SIZE(gh_thing_dir_strings)) {
        return (NULL);
    }
    return (gh_thing_dir_strings[index]);
}

typedef enum {
    GH_THING_PARSE_STATE_NONE,
    GH_THING_PARSE_STATE_DIR,
    GH_THING_PARSE_STATE_MODE,
    GH_THING_PARSE_STATE_START_DATA,
} gh_thing_parse_state;

typedef enum {
    GH_THING_PARSE_STATE_BLOCK_CH,
    GH_THING_PARSE_STATE_BLOCK_FG,
    GH_THING_PARSE_STATE_BLOCK_BG,
    GH_THING_PARSE_STATE_BLOCK_CAN_BITE_MASK,
    GH_THING_PARSE_STATE_BLOCK_OBSTACLE_MASK,
    GH_THING_PARSE_STATE_BLOCK_CAN_TOUCH_MASK,
    GH_THING_PARSE_STATE_BLOCK_MAX,
} gh_thing_parse_blocks;

#define GH_THING_PARSE_STATE_BLOCK_FIRST GH_THING_PARSE_STATE_BLOCK_CH
#define GH_THING_PARSE_STATE_BLOCK_LAST (GH_THING_PARSE_STATE_BLOCK_MAX-1)

//
// Called when pointing at the first char in a block description.
//
bool gh_thing::parse_block (const int dir,
                            const int mode,
                            const int level,
                            const char **atp,
                            const char*const end)
{
    GH_FUNCTION()

    const char *at = *atp;
    const char *tmp_at;

    int width = 0;
    int height = 0;
    int frames = 0;

    if (at >= end) {
        GH_THROW("Unexpected end at start of block");
        return (false);
    }

    //
    // Skip embedded comments
    //
    while (*at == '#') {
        while ((at < end) && (*at != '\n')) {
            at++;
        }
        at++;

        if (at >= end) {
            break;
        }
    }

    if (at >= end) {
        GH_THROW("Unexpected end after comment");
        return (false);
    }

    //
    // Find the next + to get the width of the first frame
    // All frames will be the same width.
    //
    // +--+--+--+--+
    // ^  ^
    // |..|..|..|..|
    // +--+--+--+--+
    //
    tmp_at = at;
    while ((++at < end) && (*at != '+')) {
    }
    width = at - tmp_at - 1;

    if (at >= end) {
        GH_THROW("Unexpected end before frame");
        return (false);
    }

    //
    // Skip to the end of the line so we can start reading cells.
    //
    // +--+--+--+--+
    //    ^         ^
    // |..|..|..|..|
    // +--+--+--+--+
    //
    while ((++at < end) && (*at != '\n')) { }
    frames = (at - 1 - tmp_at) / (width + 1);

    tmp_at = at + 1;

    if (at >= end) {
        GH_THROW("Unexpected end before frame (2)");
        return (false);
    }

    //
    // Skip to the next '+' at the start of a line to get the height.
    //
    while (++at < end) {
        if (*at == '\n') {
            height++;

            //
            // +--+--+--+--+
            // |..|..|..|..|
            // +--+--+--+--+ <
            // ^
            //
            if (*(at+1) == '+') {
                break;
            }

            //
            // Skip embedded comments
            //
            if (*(at+1) == '#') {
                height--;
                //
                // Skip the leading newline and then the whole comment
                //
                at++;
                while ((at < end) && (*at != '\n')) {
                    at++;
                }
                at--;
            }
        }
    }

    //
    // Which anim frame are we on? Make space for it.
    //
    anim_frames->anim_frame[dir][mode][level].resize(frames);

    for (int f = 0; f < frames; f++) {
        anim_frames->anim_frame[dir][mode][level][f].cell.resize(width * height);

        //
        // And prepare space for all the characters in this frame.
        //
        anim_frames->anim_frame[dir][mode][level][f].invisible = false;
        anim_frames->anim_frame[dir][mode][level][f].can_touch = false;
        anim_frames->anim_frame[dir][mode][level][f].can_hit = false;
        anim_frames->anim_frame[dir][mode][level][f].obstacle = false;
        anim_frames->anim_frame[dir][mode][level][f].width = width;
        anim_frames->anim_frame[dir][mode][level][f].height = height;
        anim_frames->anim_frame[dir][mode][level][f].pix_min_w = 0;
        anim_frames->anim_frame[dir][mode][level][f].pix_min_h = 0;
        anim_frames->anim_frame[dir][mode][level][f].pix_max_w = 0;
        anim_frames->anim_frame[dir][mode][level][f].pix_max_h = 0;
        anim_frames->anim_frame[dir][mode][level][f].u = 0;
        anim_frames->anim_frame[dir][mode][level][f].v = 0;
        anim_frames->anim_frame[dir][mode][level][f].tex = 0;
        anim_frames->anim_frame[dir][mode][level][f].mask = 0;
        anim_frames->anim_frame[dir][mode][level][f].l = 0;
    }

    at = tmp_at;

    //
    // Now for each block type, ch, fg, bg, mask...
    //
    for (int b = GH_THING_PARSE_STATE_BLOCK_FIRST;
            b <= GH_THING_PARSE_STATE_BLOCK_LAST; b++) {
        //
        // Skip embedded comments
        //
        while (*at == '#') {
            while ((at < end) && (*at != '\n')) {
                at++;
            }
            at++;
        }

        //
        // And for each row, skipping the first char.
        //
        at++;

#ifdef DEBUG_PARSE_FILE
        cerr << endl;
#endif
        for (int h = 0; h < height; h++) {
            //
            // And for each frame of animation we expect.
            //
            for (int f = 0; f < frames; f++) {
                //
                // And for each char in that frame of animation.
                //
                for (int w = 0; w < width; w++) {
                    //
                    // Adding tabs just confuses things when you try
                    // to look at the parse file.
                    //
                    if (*at == '\t') {
                        GH_THROW("no tabs in the syntax file please");
                    }

                    gh_anim_frame_cell *c;

                    vector<gh_anim_frame_cell> *cell;
                    cell = &anim_frames->anim_frame[dir][mode][level][f].cell;
                    c = &(*cell)[(h * width) + w];

                    switch (b) {
                    case GH_THING_PARSE_STATE_BLOCK_CH:
                        memset(c, 0, sizeof(*c));
                        c->c = *at;
                        break;
                    case GH_THING_PARSE_STATE_BLOCK_FG:
                        switch (*at) {
                            case ' ': c->fg = GH_TERM_COLOR_BLACK; break;
                            case 'w': c->fg = GH_TERM_COLOR_WHITE; break;
                            case 'r': c->fg = GH_TERM_COLOR_RED; break;
                            case 'E': c->fg = GH_TERM_COLOR_DARK_RED; break;
                            case 'g': c->fg = GH_TERM_COLOR_GREEN; break;
                            case 'e': c->fg = GH_TERM_COLOR_DARK_GREEN; break;
                            case 'y': c->fg = GH_TERM_COLOR_YELLOW; break;
                            case 'b': c->fg = GH_TERM_COLOR_BLUE; break;
                            case 'D': c->fg = GH_TERM_COLOR_DARK_BLUE; break;
                            case 'p': c->fg = GH_TERM_COLOR_PINK; break;
                            case 'c': c->fg = GH_TERM_COLOR_CYAN; break;
                            case 'R': c->fg = GH_TERM_COLOR_RANDOM; break;
                            case '_': c->fg = GH_TERM_COLOR_GRAY; break;
                            case 'W': c->fg = GH_TERM_COLOR_SNOW; break;
                            case 'o': c->fg = GH_TERM_COLOR_ORANGE; break;
                            case 'O': c->fg = GH_TERM_COLOR_LIGHT_ORANGE; break;
                            case 'G': c->fg = GH_TERM_COLOR_LIME; break;
                            case 'Y': c->fg = GH_TERM_COLOR_LIGHT_YELLOW; break;
                            case 'B': c->fg = GH_TERM_COLOR_LIGHT_BLUE; break;
                            case 'P': c->fg = GH_TERM_COLOR_PURPLE; break;
                            case 'C': c->fg = GH_TERM_COLOR_LIGHT_CYAN; break;
                            case '0': c->fg = GH_TERM_COLOR_GRAY_0; break;
                            case '1': c->fg = GH_TERM_COLOR_GRAY_1; break;
                            case '2': c->fg = GH_TERM_COLOR_GRAY_2; break;
                            case '3': c->fg = GH_TERM_COLOR_GRAY_3; break;
                            case '4': c->fg = GH_TERM_COLOR_GRAY_4; break;
                            case '5': c->fg = GH_TERM_COLOR_GRAY_5; break;
                            case '6': c->fg = GH_TERM_COLOR_GRAY_6; break;
                            case '7': c->fg = GH_TERM_COLOR_GRAY_7; break;
                            case '8': c->fg = GH_TERM_COLOR_GRAY_8; break;
                            case '9': c->fg = GH_TERM_COLOR_GRAY_9; break;
                            case 'n': c->fg = GH_TERM_COLOR_BROWN; break;
                        }
                        break;
                    case GH_THING_PARSE_STATE_BLOCK_BG:
                        switch (*at) {
                            case ' ': c->bg = GH_TERM_COLOR_BLACK; break;
                            case 'w': c->bg = GH_TERM_COLOR_WHITE; break;
                            case 'r': c->bg = GH_TERM_COLOR_RED; break;
                            case 'E': c->bg = GH_TERM_COLOR_DARK_RED; break;
                            case 'g': c->bg = GH_TERM_COLOR_GREEN; break;
                            case 'e': c->bg = GH_TERM_COLOR_DARK_GREEN; break;
                            case 'y': c->bg = GH_TERM_COLOR_YELLOW; break;
                            case 'b': c->bg = GH_TERM_COLOR_BLUE; break;
                            case 'D': c->bg = GH_TERM_COLOR_DARK_BLUE; break;
                            case 'p': c->bg = GH_TERM_COLOR_PINK; break;
                            case 'c': c->bg = GH_TERM_COLOR_CYAN; break;
                            case 'R': c->bg = GH_TERM_COLOR_RANDOM; break;
                            case '_': c->bg = GH_TERM_COLOR_GRAY; break;
                            case 'W': c->bg = GH_TERM_COLOR_SNOW; break;
                            case 'o': c->bg = GH_TERM_COLOR_ORANGE; break;
                            case 'O': c->bg = GH_TERM_COLOR_LIGHT_ORANGE; break;
                            case 'G': c->bg = GH_TERM_COLOR_LIME; break;
                            case 'Y': c->bg = GH_TERM_COLOR_LIGHT_YELLOW; break;
                            case 'B': c->bg = GH_TERM_COLOR_LIGHT_BLUE; break;
                            case 'P': c->bg = GH_TERM_COLOR_PURPLE; break;
                            case 'C': c->bg = GH_TERM_COLOR_LIGHT_CYAN; break;
                            case '0': c->bg = GH_TERM_COLOR_GRAY_0; break;
                            case '1': c->bg = GH_TERM_COLOR_GRAY_1; break;
                            case '2': c->bg = GH_TERM_COLOR_GRAY_2; break;
                            case '3': c->bg = GH_TERM_COLOR_GRAY_3; break;
                            case '4': c->bg = GH_TERM_COLOR_GRAY_4; break;
                            case '5': c->bg = GH_TERM_COLOR_GRAY_5; break;
                            case '6': c->bg = GH_TERM_COLOR_GRAY_6; break;
                            case '7': c->bg = GH_TERM_COLOR_GRAY_7; break;
                            case '8': c->bg = GH_TERM_COLOR_GRAY_8; break;
                            case '9': c->bg = GH_TERM_COLOR_GRAY_9; break;
                            case 'n': c->bg = GH_TERM_COLOR_BROWN; break;
                        }
                        break;
                    case GH_THING_PARSE_STATE_BLOCK_CAN_BITE_MASK:
                        switch (*at) {
                            case ' ':
                                c->can_hit_mask = false;
                                break;
                            default:
                                c->can_hit_mask = true;
                                break;
                        }
                        break;

                    case GH_THING_PARSE_STATE_BLOCK_OBSTACLE_MASK:
                        switch (*at) {
                            case ' ':
                                c->obstacle_mask = false;
                                break;
                            default:
                                c->obstacle_mask = true;
                                break;
                        }
                        break;

                    case GH_THING_PARSE_STATE_BLOCK_CAN_TOUCH_MASK:
                        switch (*at) {
                            case ' ':
                                c->can_touch_mask = false;
                                break;
                            default:
                                c->can_touch_mask = true;
                                break;
                        }
                        break;
                    }
#ifdef DEBUG_PARSE_FILE
                    cerr << *at;
#endif
                    at++;

                    if (at >= end) {
                        GH_THROW("Unexpected in frame");
                        return (false);
                    }
                }
#ifdef DEBUG_PARSE_FILE
                cerr << *at;
#endif
                at++;

                if (at >= end) {
                    GH_THROW("Unexpected in frame count");
                    return (false);
                }
            }

#ifdef DEBUG_PARSE_FILE
            cerr << endl;
#endif
            //
            // Discard all remaining junk until the next line is reached.
            //
            while ((at < end) && (*at != '\n')) { at++; }

            if (at >= end) {
                break;
            }

            //
            // Skip the newline
            //
            at++;

            if (at >= end) {
                break;
            }

            //
            // Skip embedded comments
            //
            while (*at == '#') {
                while ((at < end) && (*at != '\n')) {
                    at++;
                }
                at++;

                if (at >= end) {
                    break;
                }
            }

            //
            // Skip the first char on the new line.
            //
            at++;

            if (at >= end) {
                break;
            }
        }

        //
        // Discard all remaining junk until the start of the next
        // block is reached.
        //
        while ((at < end) && (*at != '\n')) { at++; }
        at++;
    }

    *atp = at;
    return (true);
}

void gh_thing::parse (const string file, int level)
{
    GH_FUNCTION()

    int line = 1;
    size_t size;
    char *const buf = gh_zip::instance()->gzreadall(file, &size, 32768);
    int dir;
    int mode;

    if (size == 0) {
        delete [] buf;
        GH_THROW("empty file \"" + file + "\"");
        return;
    }

    if (buf == NULL) {
        delete [] buf;
        GH_THROW("can't open file \"" + file + "\"");
    }

    const char *end = buf + size;
    const char *next = buf;
    const char *at;

    bool newline = true;

    string dir_str = "";
    string mode_str = "";

    gh_thing_parse_state state = GH_THING_PARSE_STATE_DIR;

    for (;;) {
        at = next;

        if (newline) {
            while (at < end) {
                if (*at == '#') {
                    while ((at < end) && (*at != '\n')) { at++; }
                    at++;
                } else {
                    while ((at < end) && isspace(*at)) { at++; }
                    break;
                }
            }

            newline = false;
        }

        if (at >= end) {
            break;
        }

        const char c = *at;
        next = at + 1;

        if (c == '\n') {
            line++;
            newline = true;
        }

        switch (state) {
        case GH_THING_PARSE_STATE_DIR:
            switch (c) {
            case '\n':
            case ' ':
                state = GH_THING_PARSE_STATE_MODE;
                continue;

            default:
                dir_str += c;
                continue;
            }
            break;

        case GH_THING_PARSE_STATE_MODE:
            switch (c) {
            case '\n':
            case ' ':
                state = GH_THING_PARSE_STATE_START_DATA;
                continue;

            default:
                mode_str += c;
                continue;
            }
            break;

        case GH_THING_PARSE_STATE_START_DATA:

            dir = gh_thing_dir_string_index(dir_str.c_str());
            if (dir == -1) {
                GH_THROW("unknown dir \"" + dir_str + "\"");
            }

            mode = gh_thing_mode_string_index(mode_str.c_str());
            if (mode == -1) {
                GH_THROW("unknown mode \"" + mode_str + "\"");
            }

            if (!parse_block(dir, mode, level, &at, end)) {
                GH_THROW("format error in dir \"" + dir_str + "\" and mode \"" + mode_str + "\"");
            }

            dir_str = "";
            mode_str = "";

            state = GH_THING_PARSE_STATE_DIR;
            next = at;
            newline = true;

            break;

        default:
            GH_THROW("parse error");
        }
    }

    delete [] buf;
}
