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

    $Id: io.cpp 1227 2009-06-28 03:29:28Z Ghack $
*/

#include <ghack.h>
#include <gh_zip.h>
#include <gh_lexical_cast.h>

const char gh_indent_str[GH_INDENT_STR_SIZE] = "                                                                                                   ";

//
// This routine checks for little in the way of format errors. This is
// intentional for speed. The input format is fixed and known. If the
// input is screwed up and we overrun, tough.
//

vector<gh_node> gh_parse::nodes;

gh_inline void gh_parse::alloc (const uint32 size)
{
    if (nodes.size() < size) {
        nodes.reserve(size);
    }
}

gh_inline void gh_parse::push (const int depth, const float tmp_float)
        {
    (*node).type = gh_node::GH_NODE_FLOAT;
    (*node).depth = depth;
    (*node).val.v_float = tmp_float;
    node++;
}

gh_inline void gh_parse::push (const int depth, const long tmp_int)
{
    (*node).type = gh_node::GH_NODE_INT;
    (*node).depth = depth;
    (*node).val.v_int = tmp_int;
    node++;
}

gh_inline void gh_parse::push (const int depth, char *tmp_at)
{
    (*node).type = gh_node::GH_NODE_NAME;
    (*node).depth = depth;
    (*node).val.v_name = tmp_at;
    node++;
}

gh_parse::gh_parse (const string infile)
{
    GH_FUNCTION()

    file = infile;
    line = 1;

    buf = gh_zip::instance()->gzreadall(file, &size, 32768, false /* binary */);
    if (size == 0) {
        //
        // Not always an error.
        //
        // GH_WARN("empty file \"" + file + "\"");
        return;
    }

    if (buf == NULL) {
        GH_THROW("can't parse file \"" + file + "\"");
        return;
    }

    end = buf + size;

    char *next = buf;
    char *at = buf;

    depth = 0;
    state = GH_PARSE_STATE_NONE;

    size += sizeof(node);

    alloc(size);

    node = nodes.begin();
    end_node = node;
    bool newline = true;

    if (at >= end) {
        (*node).type = gh_node::GH_NODE_EOF;
        (*node).val.v_name = NULL;
    }

    while (at < end) {
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
            (*node).type = gh_node::GH_NODE_EOF;
            (*node).val.v_name = NULL;
            break;
        }

        char c = *at;

        next = at + 1;

        if (at >= end) {
            GH_THROW("Unexpected end in \"" + file + "\"");
        }
        
        switch (state) {
        case GH_PARSE_STATE_NONE:
            switch (c) {
            case ' ':
            case '\t':
                continue;

            case '#':
                while ((at < end) && (*at != '\n')) { at++; }
                next = at;
                continue;

            default:
                state = GH_PARSE_STATE_NAME;
                tmp_at = at;
                continue;

            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
                state = GH_PARSE_STATE_INT;
                tmp_int = c - '0';
                tmp_mul = 1;
                tmp_at = at;
                continue;

            case '\"':
                state = GH_PARSE_STATE_STRING;
                tmp_at = at + 1;
                continue;

            case '[':
            case '{':
                (*node).type = gh_node::GH_NODE_BRA;
                (*node).depth = depth;
                (*node).val.v_name = NULL;
                node++;
                depth++;
                continue;

            case ']':
            case '}':
                depth--;
                (*node).type = gh_node::GH_NODE_KET;
                (*node).depth = depth;
                (*node).val.v_name = NULL;
                node++;
                continue;

            case 13:
                line++;
                newline = true;
                continue;

            case '\n':
                line++;
                newline = true;

#ifdef GH_DO_PARSER_OPTIMISE
                //
                // This assumes the file was not modified by a human!
                //
                if (depth) {
                    if ((*(at-1) == '{') ||
                        (*(at-1) == '[')) {
                        next += depth;
                    } else {
                        next += depth - 1;
                    }
                }
#endif
                continue;

            case '-':
                state = GH_PARSE_STATE_INT;
                tmp_int = 0;
                tmp_at = at;
                tmp_mul = -1;
                continue;

            case '.':
            case '+':
                state = GH_PARSE_STATE_INT;
                tmp_int = 0;
                tmp_at = at;
                tmp_mul = 1;
                continue;
            }
            continue;

        case GH_PARSE_STATE_INT:
            while (isdigit(c)) {
                tmp_int *= 10;
                tmp_int += c - '0';
                c = *++at;

                if (at >= end) {
                    GH_THROW("Unexpected end in \"" + file + "\"");
                }
            }

            if (c == '.') {

                bool need_lex = false;

                tmp_float = tmp_int;

                float mul = 10.0;

                c = *++at;

                while (!isspace(c)) {
                    if (at >= end) {
                        GH_THROW("Unexpected end in \"" + file + "\"");
                    }

                    if (need_lex) {
                        c = *++at;
                        continue;
                    }

                    if (c == 'e') {
                        c = *++at;
                        while (!isspace(c)) {
                            c = *++at;
                        }
                        break;
                    }

                    tmp_float += (float)(c - '0') / mul;
                    mul *= 10.0;
                    c = *++at;
                }

                if (need_lex) {
                    *at++ = '\0';

                    tmp_float = lexical_cast<float, char*>(tmp_at);
                } else {
                    tmp_float *= tmp_mul;
                }
                push(depth, tmp_float);
            } else {
                tmp_int *= tmp_mul;
                push(depth, tmp_int);
            }

            state = GH_PARSE_STATE_NONE;
            next = at;
            continue;

        case GH_PARSE_STATE_NAME:
            while ((c != '=') && (c != ' ') && (c != '\t') && (c != '\n')) {
                c = *++at;

                if (at >= end) {
                    GH_THROW("Unexpected end in \"" + file + "\"");
                }
            }

            if (c == '\n') {
                newline = true;
            }

            state = GH_PARSE_STATE_NONE;
            *at = '\0';
            next = at + 1;
            push(depth, tmp_at);
            continue;

        case GH_PARSE_STATE_STRING:
#ifdef GH_PARSER_SUPPORT_ESCAPE_CHARACTERS
            bool compress = false;
#endif
            while (c != '\"') {
                c = *++at;

#ifdef GH_PARSER_SUPPORT_ESCAPE_CHARACTERS
                if (c == '\\') {
                    at++;
                    compress = true;
                }
#endif
                if (at >= end) {
                    GH_THROW("Unexpected end in \"" + file + "\"");
                }
            }

            state = GH_PARSE_STATE_NONE;
            *at = '\0';
            next = at + 1;

#ifdef GH_PARSER_SUPPORT_ESCAPE_CHARACTERS
            if (compress) {
                remove_escape_characters(tmp_at);
            }
#endif
            push(depth, tmp_at);
            continue;
        }
    }

    end_node = node;
    node = nodes.begin();

    if (state != GH_PARSE_STATE_NONE) {
        parse_error("unexpected end", NULL);
    }

#ifdef GH_DO_PARSER_DEBUG
    cout << "--------------------------------------------" << endl;
    cout << *this << endl;
    cout << "--------------------------------------------" << endl;
#endif
}

//
// Report where we died in the input
//
void gh_parse::parse_error (string error, char *at)
{
    GH_FUNCTION()

    char *c = at;

    while (c < end) {
        if (*c == '\n') {
            *c = '\0';
            break;
        }
        c++;
    }

    if (at != NULL) {
        GH_THROW(error + " error in " + file + ", line " +
                    (lexical_cast<string, int>(line)) + " near:\n" +
                    string(at));
    } else {
        GH_THROW(error + " error in " + file + ", line " +
                    (lexical_cast<string, int>(line)));
    }
}

//
// Shrink a string by the space needed for its escape characters.
//
void gh_parse::remove_escape_characters (char *uncompressed)
{
    char * t = uncompressed;
    char * s = uncompressed;
    char c;

    while ((c = *s++)) {
        if (c != '\\') {
            *t++ = c;
            continue;
        }

        switch (*s++) {
            case 'n':
                *t++ = '\n';
                break;
            case 't':
                *t++ = '\t';
                break;
            case '%':
                *t++ = '%';
                break;
            case '#':
                *t++ = '@';
                break;
            case '\"':
                *t++ = '\"';
                break;
            case '\'':
                *t++ = '\'';
                break;
            case '\\':
                *t++ = '\\';
                break;
            default:
                char *o = s - 1;
                char orig;

                while (isdigit(*s)) { s++; }

                orig = *s;
                *s = '\0';
                *t++ = atoi(o);
                *s = orig;
                break;
        }
    }
    *t = '\0';
}

gh_parse::~gh_parse (void)
{
    GH_FUNCTION()

    if (buf) {
        delete buf;
        buf = NULL;
    }
}
