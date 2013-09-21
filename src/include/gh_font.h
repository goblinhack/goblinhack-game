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

    $Id: font.h,v 1.1 2004/03/22 19:03:49 nmcgill Exp $
*/

#ifndef _GH_FONT_H_
#define _GH_FONT_H_

#include "ftgl/FTGLExtrdFont.h"
#include "ftgl/FTGLOutlineFont.h"
#include "ftgl/FTGLPolygonFont.h"
#include "ftgl/FTGLTextureFont.h"
#include "ftgl/FTGLPixmapFont.h"
#include "ftgl/FTGLBitmapFont.h"

class gh_font
{
public:
    typedef enum {
        TTF_STYLE_BITMAP,
        TTF_STYLE_PIXMAP,
        TTF_STYLE_OUTLINE,
        TTF_STYLE_POLYGON,
        TTF_STYLE_EXTRUDE,
        TTF_STYLE_TEXTURE,
        MAX_TTF_STYLE,
    } ttf_style;

    static const char * const ttf_style_names[];

    static const size_t max_sizes = 200;

    FTFont                  * ftgl[MAX_TTF_STYLE][max_sizes];
    uint32                  size;
    uint32                  depth;
    string                  short_name;
    string                  full_name;

    gh_font(const string file);
    ~gh_font(void);

    typedef map<string, gh_font*> hash_fonts;

    static hash_fonts fonts;

    static void unload_all(void);
    static void load_all(void);
    static void reload_all(void);

    void text_at(
        const ttf_style style,
        const string text,
        const gh_fpoint3d v = gh_fpoint3d(0,0,0),
        const float scale = 1.0,
        const gh_pixel &pixel = GH_WHITE);

    //
    // Find this texture in the texture bank
    //
    gh_inline static gh_font* find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this texture in the texture bank
    //
    gh_inline static gh_font* find (const char* s1, const bool err = true)
    {
        gh_font *s = fonts[s1];

        if (s != NULL) {
            return (s);
        }

        if (err) {
            GH_THROW("couldn't find " + string(s1) + " in loaded list");
        }

        return (NULL);
    }
};

extern gh_font::hash_fonts fonts;
#endif /* _GH_FONT_H_ */
