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
#include <gh_room.h>
#include <gh_console.h>
#include <gh_math.h>
#include <gh_map.h>

#define nDEBUG_PARSE_FILE

//
// Called when pointing at the first char in a block description.
//
bool gh_room::parse_block (const int room_no,
                           const char **atp,
                           const char*const end)
{
    GH_FUNCTION()

    const char *at = *atp;
    const char *tmp_at;

    int width = 0;
    int height = 0;

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

    while (*at == ':') {
        if (end - at > (int)sizeof(":map . ...")) {
            if (!strncmp(at - 1, ":map ", sizeof(":map ") - 1)) {
                char map_char;
                string thing_name = "";

                at += sizeof("map" - 1);

                map_char = *at++;
                at++;

                do {
                    char c = *at++;

                    switch (c) {
                        case '\n':
                        case ' ':
                            //
                            // Reached the end of a thing name
                            //
                            map_chars.push_front(
                                gh_map_char(NULL,
                                            map_char,
                                            thing_name,
                                            0,
                                            0));
                            thing_name = "";
                            break;

                        default:
                            thing_name += c;
                            break;
                    }
                } while (thing_name != "");
            }
        }
        break;
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
    while ((++at < end) && (*at != '\n')) {
    }

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

    at = tmp_at;
    if (at >= end) {
        return (true);
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
            return (true);
        }
    }

    //
    // And for each row, skipping the first char.
    //
    at++;
    if (at >= end) {
        return (true);
    }

    contents[room_no].cell.resize(width * height);

    //
    // And prepare space for all the characters in this frame.
    //
    contents[room_no].width = width;
    contents[room_no].height = height;

#ifdef DEBUG_PARSE_FILE
    cerr << endl;
#endif
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            //
            // Adding tabs just confuses rooms when you try
            // to look at the parse file.
            //
            if (*at == '\t') {
                GH_THROW("no tabs in the syntax file please");
            }

            gh_room_contents_cell *c;

            vector<gh_room_contents_cell> *cell;
            cell = &contents[room_no].cell;
            c = &(*cell)[(h * width) + w];

            memset(c, 0, sizeof(*c));
            c->c = *at;

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
#endif
        at++;

        if (at >= end) {
            GH_THROW("Unexpected in frame count");
            return (false);
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
    // Jump back to the start of the +----+ for the next room.
    //
    while ((at < end) && (*at != '+')) { at--; }

    *atp = at;
    return (true);
}

void gh_room::parse (const string file)
{
    GH_FUNCTION()

    int line = 1;
    size_t size;
    char *const buf = gh_zip::instance()->gzreadall(file, &size, 32768);
    int room_no = 0;

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

        gh_room_contents r;
        contents.push_back(r);

        if (!parse_block(room_no, &at, end)) {
            GH_THROW("format error");
        }

        //
        // Last room?
        //
        if (!contents[room_no].cell.size()) {
            contents.pop_back();
            break;
        }

        next = at;
        newline = true;
        room_no++;
    }

    delete [] buf;
}
