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

    $Id: gh_pixel.h 513 2007-05-07 01:46:54Z goblinhack $
*/ 

#ifndef _GH_PIXEL_H_
#define _GH_PIXEL_H_

#if defined WIN32 || defined __CYGWIN__
#include <windows.h>
#endif

#ifdef MACOSX
#include <gl.h>
#include <glu.h>
#else
#include <GL/gl.h>  
#include <GL/glu.h> 
#endif

#include <gh_io.h>

class gh_pixel : public gh_stream
{
public:
    uint32 rgb;
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a;

    //
    // string to pixel
    //
    typedef map<int, const char*> hash_pixel2;

    static hash_pixel2 pixels2;

    //
    // pixel to string
    //
    typedef map<string, gh_pixel> hash_pixel;
    static hash_pixel pixels;

    static void init(void);

    gh_pixel(void) : r(0), g(0), b(0), a(255) { }

    gh_pixel(uint8 r, uint8 g, uint8 b, uint8 a=255)
        : r(r), g(g), b(b), a(a) { }

    gh_inline void to_rgb (void)
    {
        rgb = (a << 24) | (r << 16) | (g << 8) | b;
    }

    gh_inline void from_rgb (void)
    {
        a = (uint8)((rgb & 0xff000000) >> 24);
        r = (uint8)((rgb & 0x00ff0000) >> 16);
        g = (uint8)((rgb & 0x0000ff00) >> 8);
        b = (uint8)((rgb & 0x000000ff));
    }

    //
    // Set current color in opengl
    //
    gh_inline void gl (void) const
    {   
        glColor4ubv(&r);
    }   

    //
    // Find this pixel in the pixel bank
    //
    gh_inline static gh_pixel find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this pixel in the pixel bank
    //
    gh_inline static gh_pixel find (const char* s1)
    {
        gh_pixel::hash_pixel::iterator m = pixels.find(s1);

        if (m == pixels.end()) {
            GH_THROW("couldn't find " + string(s1) + " in loaded list");
        }

        gh_pixel p = m->second;

        return (p);
    }

    //
    // Compare RGB components of two color
    //
    gh_inline friend bool operator== (gh_pixel a, gh_pixel b)
    {
        return (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a);
    }

    //
    // Compare RGB components of two color
    //
    gh_inline friend bool operator!= (gh_pixel a, gh_pixel b)
    {
        return (a.r != b.r) || (a.g != b.g) || (a.b != b.b) || (a.a != b.a);
    }

    friend void operator>> (gh_stream_in& is, gh_pixel *me);
    friend void operator<< (gh_stream_out& os, const gh_pixel *me);
};

class gh_fpixel : public gh_stream
{
public:
    float r;
    float g;
    float b;
    float a;

    gh_fpixel(void) : r(0), g(0), b(0), a(1.0) { }

    gh_fpixel(float r, float g, float b, float a=1.0)
        : r(r), g(g), b(b), a(a) { }

    //
    // Set current color in opengl
    //
    gh_inline void gl (void) const
    {   
        glColor4fv(&r);
    }   

    friend void operator>> (gh_stream_in& is, gh_fpixel *me);
    friend void operator<< (gh_stream_out& os, const gh_fpixel *me);
};

//
// Color is a wrapper for pixel, with a name and file name which allows
// loading from a file
//
class gh_color : public gh_stream
{
public:
    string              full_name; // ../resources/color/reds/fire
    string              short_name;// fire
    gh_pixel            pixel;

    //
    // string to color
    //
    typedef map<string, gh_color*> hash_color;
    static hash_color color;

    //
    // color.cpp
    //
    gh_color(const string name);

    //
    // Find this color in the color bank
    //
    gh_inline static gh_color* find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this color in the color bank
    //
    gh_inline static gh_color* find (const char* s1,
                                     const bool err = true)
    {
        gh_color::hash_color::iterator m = color.find(s1);

        if (m == color.end()) {
            if (err) {
                GH_THROW("couldn't find " + string(s1) + " in loaded list");
            }
            return (NULL);
        }

        gh_color *color = m->second;

        if (color == NULL) {
            GH_THROW("null color" + string(s1) + " in loaded list");
        }

        return (color);
    }

    static void load_all(void);
    static void unload_all(void);

    friend void operator>> (gh_stream_in& is, gh_color *me);
    friend void operator<< (gh_stream_out& os, const gh_color *me);
};

extern const gh_pixel GH_ALICE_BLUE          ;
extern const gh_pixel GH_ALICEBLUE           ;
extern const gh_pixel GH_ANTIQUEWHITE1       ;
extern const gh_pixel GH_ANTIQUEWHITE2       ;
extern const gh_pixel GH_ANTIQUEWHITE3       ;
extern const gh_pixel GH_ANTIQUEWHITE4       ;
extern const gh_pixel GH_ANTIQUE_WHITE       ;
extern const gh_pixel GH_ANTIQUEWHITE        ;
extern const gh_pixel GH_AQUAMARINE1         ;
extern const gh_pixel GH_AQUAMARINE2         ;
extern const gh_pixel GH_AQUAMARINE3         ;
extern const gh_pixel GH_AQUAMARINE4         ;
extern const gh_pixel GH_AQUAMARINE          ;
extern const gh_pixel GH_AZURE1              ;
extern const gh_pixel GH_AZURE2              ;
extern const gh_pixel GH_AZURE3              ;
extern const gh_pixel GH_AZURE4              ;
extern const gh_pixel GH_AZURE               ;
extern const gh_pixel GH_BEIGE               ;
extern const gh_pixel GH_BISQUE1             ;
extern const gh_pixel GH_BISQUE2             ;
extern const gh_pixel GH_BISQUE3             ;
extern const gh_pixel GH_BISQUE4             ;
extern const gh_pixel GH_BISQUE              ;
extern const gh_pixel GH_BLACK               ;
extern const gh_pixel GH_BLANCHED_ALMOND     ;
extern const gh_pixel GH_BLANCHEDALMOND      ;
extern const gh_pixel GH_BLUE1               ;
extern const gh_pixel GH_BLUE2               ;
extern const gh_pixel GH_BLUE3               ;
extern const gh_pixel GH_BLUE4               ;
extern const gh_pixel GH_BLUE                ;
extern const gh_pixel GH_BLUE_VIOLET         ;
extern const gh_pixel GH_BLUEVIOLET          ;
extern const gh_pixel GH_BROWN1              ;
extern const gh_pixel GH_BROWN2              ;
extern const gh_pixel GH_BROWN3              ;
extern const gh_pixel GH_BROWN4              ;
extern const gh_pixel GH_BROWN               ;
extern const gh_pixel GH_BURLYWOOD1          ;
extern const gh_pixel GH_BURLYWOOD2          ;
extern const gh_pixel GH_BURLYWOOD3          ;
extern const gh_pixel GH_BURLYWOOD4          ;
extern const gh_pixel GH_BURLYWOOD           ;
extern const gh_pixel GH_CADETBLUE1          ;
extern const gh_pixel GH_CADETBLUE2          ;
extern const gh_pixel GH_CADETBLUE3          ;
extern const gh_pixel GH_CADETBLUE4          ;
extern const gh_pixel GH_CADET_BLUE          ;
extern const gh_pixel GH_CADETBLUE           ;
extern const gh_pixel GH_CHARTREUSE1         ;
extern const gh_pixel GH_CHARTREUSE2         ;
extern const gh_pixel GH_CHARTREUSE3         ;
extern const gh_pixel GH_CHARTREUSE4         ;
extern const gh_pixel GH_CHARTREUSE          ;
extern const gh_pixel GH_CHOCOLATE1          ;
extern const gh_pixel GH_CHOCOLATE2          ;
extern const gh_pixel GH_CHOCOLATE3          ;
extern const gh_pixel GH_CHOCOLATE4          ;
extern const gh_pixel GH_CHOCOLATE           ;
extern const gh_pixel GH_CORAL1              ;
extern const gh_pixel GH_CORAL2              ;
extern const gh_pixel GH_CORAL3              ;
extern const gh_pixel GH_CORAL4              ;
extern const gh_pixel GH_CORAL               ;
extern const gh_pixel GH_CORNFLOWER_BLUE     ;
extern const gh_pixel GH_CORNFLOWERBLUE      ;
extern const gh_pixel GH_CORNSILK1           ;
extern const gh_pixel GH_CORNSILK2           ;
extern const gh_pixel GH_CORNSILK3           ;
extern const gh_pixel GH_CORNSILK4           ;
extern const gh_pixel GH_CORNSILK            ;
extern const gh_pixel GH_CYAN1               ;
extern const gh_pixel GH_CYAN2               ;
extern const gh_pixel GH_CYAN3               ;
extern const gh_pixel GH_CYAN4               ;
extern const gh_pixel GH_CYAN                ;
extern const gh_pixel GH_DARK_BLUE           ;
extern const gh_pixel GH_DARKBLUE            ;
extern const gh_pixel GH_DARK_CYAN           ;
extern const gh_pixel GH_DARKCYAN            ;
extern const gh_pixel GH_DARKGOLDENROD1      ;
extern const gh_pixel GH_DARKGOLDENROD2      ;
extern const gh_pixel GH_DARKGOLDENROD3      ;
extern const gh_pixel GH_DARKGOLDENROD4      ;
extern const gh_pixel GH_DARK_GOLDENROD      ;
extern const gh_pixel GH_DARKGOLDENROD       ;
extern const gh_pixel GH_DARK_GRAY           ;
extern const gh_pixel GH_DARKGRAY            ;
extern const gh_pixel GH_DARK_GREEN          ;
extern const gh_pixel GH_DARKGREEN           ;
extern const gh_pixel GH_DARK_GREY           ;
extern const gh_pixel GH_DARKGREY            ;
extern const gh_pixel GH_DARK_KHAKI          ;
extern const gh_pixel GH_DARKKHAKI           ;
extern const gh_pixel GH_DARK_MAGENTA        ;
extern const gh_pixel GH_DARKMAGENTA         ;
extern const gh_pixel GH_DARKOLIVEGREEN1     ;
extern const gh_pixel GH_DARKOLIVEGREEN2     ;
extern const gh_pixel GH_DARKOLIVEGREEN3     ;
extern const gh_pixel GH_DARKOLIVEGREEN4     ;
extern const gh_pixel GH_DARK_OLIVE_GREEN    ;
extern const gh_pixel GH_DARKOLIVEGREEN      ;
extern const gh_pixel GH_DARKORANGE1         ;
extern const gh_pixel GH_DARKORANGE2         ;
extern const gh_pixel GH_DARKORANGE3         ;
extern const gh_pixel GH_DARKORANGE4         ;
extern const gh_pixel GH_DARK_ORANGE         ;
extern const gh_pixel GH_DARKORANGE          ;
extern const gh_pixel GH_DARKORCHID1         ;
extern const gh_pixel GH_DARKORCHID2         ;
extern const gh_pixel GH_DARKORCHID3         ;
extern const gh_pixel GH_DARKORCHID4         ;
extern const gh_pixel GH_DARK_ORCHID         ;
extern const gh_pixel GH_DARKORCHID          ;
extern const gh_pixel GH_DARK_RED            ;
extern const gh_pixel GH_DARKRED             ;
extern const gh_pixel GH_DARK_SALMON         ;
extern const gh_pixel GH_DARKSALMON          ;
extern const gh_pixel GH_DARKSEAGREEN1       ;
extern const gh_pixel GH_DARKSEAGREEN2       ;
extern const gh_pixel GH_DARKSEAGREEN3       ;
extern const gh_pixel GH_DARKSEAGREEN4       ;
extern const gh_pixel GH_DARK_SEA_GREEN      ;
extern const gh_pixel GH_DARKSEAGREEN        ;
extern const gh_pixel GH_DARK_SLATE_BLUE     ;
extern const gh_pixel GH_DARKSLATEBLUE       ;
extern const gh_pixel GH_DARKSLATEGRAY1      ;
extern const gh_pixel GH_DARKSLATEGRAY2      ;
extern const gh_pixel GH_DARKSLATEGRAY3      ;
extern const gh_pixel GH_DARKSLATEGRAY4      ;
extern const gh_pixel GH_DARK_SLATE_GRAY     ;
extern const gh_pixel GH_DARKSLATEGRAY       ;
extern const gh_pixel GH_DARK_SLATE_GREY     ;
extern const gh_pixel GH_DARKSLATEGREY       ;
extern const gh_pixel GH_DARK_TURQUOISE      ;
extern const gh_pixel GH_DARKTURQUOISE       ;
extern const gh_pixel GH_DARK_VIOLET         ;
extern const gh_pixel GH_DARKVIOLET          ;
extern const gh_pixel GH_DEEPPINK1           ;
extern const gh_pixel GH_DEEPPINK2           ;
extern const gh_pixel GH_DEEPPINK3           ;
extern const gh_pixel GH_DEEPPINK4           ;
extern const gh_pixel GH_DEEP_PINK           ;
extern const gh_pixel GH_DEEPPINK            ;
extern const gh_pixel GH_DEEPSKYBLUE1        ;
extern const gh_pixel GH_DEEPSKYBLUE2        ;
extern const gh_pixel GH_DEEPSKYBLUE3        ;
extern const gh_pixel GH_DEEPSKYBLUE4        ;
extern const gh_pixel GH_DEEP_SKY_BLUE       ;
extern const gh_pixel GH_DEEPSKYBLUE         ;
extern const gh_pixel GH_DIM_GRAY            ;
extern const gh_pixel GH_DIMGRAY             ;
extern const gh_pixel GH_DIM_GREY            ;
extern const gh_pixel GH_DIMGREY             ;
extern const gh_pixel GH_DODGERBLUE1         ;
extern const gh_pixel GH_DODGERBLUE2         ;
extern const gh_pixel GH_DODGERBLUE3         ;
extern const gh_pixel GH_DODGERBLUE4         ;
extern const gh_pixel GH_DODGER_BLUE         ;
extern const gh_pixel GH_DODGERBLUE          ;
extern const gh_pixel GH_FIREBRICK1          ;
extern const gh_pixel GH_FIREBRICK2          ;
extern const gh_pixel GH_FIREBRICK3          ;
extern const gh_pixel GH_FIREBRICK4          ;
extern const gh_pixel GH_FIREBRICK           ;
extern const gh_pixel GH_FLORAL_WHITE        ;
extern const gh_pixel GH_FLORALWHITE         ;
extern const gh_pixel GH_FOREST_GREEN        ;
extern const gh_pixel GH_FORESTGREEN         ;
extern const gh_pixel GH_GAINSBORO           ;
extern const gh_pixel GH_GHOST_WHITE         ;
extern const gh_pixel GH_GHOSTWHITE          ;
extern const gh_pixel GH_GOLD1               ;
extern const gh_pixel GH_GOLD2               ;
extern const gh_pixel GH_GOLD3               ;
extern const gh_pixel GH_GOLD4               ;
extern const gh_pixel GH_GOLDENROD1          ;
extern const gh_pixel GH_GOLDENROD2          ;
extern const gh_pixel GH_GOLDENROD3          ;
extern const gh_pixel GH_GOLDENROD4          ;
extern const gh_pixel GH_GOLDENROD           ;
extern const gh_pixel GH_GOLD                ;
extern const gh_pixel GH_GRAY10              ;
extern const gh_pixel GH_GRAY11              ;
extern const gh_pixel GH_GRAY12              ;
extern const gh_pixel GH_GRAY13              ;
extern const gh_pixel GH_GRAY14              ;
extern const gh_pixel GH_GRAY15              ;
extern const gh_pixel GH_GRAY16              ;
extern const gh_pixel GH_GRAY17              ;
extern const gh_pixel GH_GRAY18              ;
extern const gh_pixel GH_GRAY19              ;
extern const gh_pixel GH_GRAY1               ;
extern const gh_pixel GH_GRAY20              ;
extern const gh_pixel GH_GRAY21              ;
extern const gh_pixel GH_GRAY22              ;
extern const gh_pixel GH_GRAY23              ;
extern const gh_pixel GH_GRAY24              ;
extern const gh_pixel GH_GRAY25              ;
extern const gh_pixel GH_GRAY26              ;
extern const gh_pixel GH_GRAY27              ;
extern const gh_pixel GH_GRAY28              ;
extern const gh_pixel GH_GRAY29              ;
extern const gh_pixel GH_GRAY2               ;
extern const gh_pixel GH_GRAY30              ;
extern const gh_pixel GH_GRAY31              ;
extern const gh_pixel GH_GRAY32              ;
extern const gh_pixel GH_GRAY33              ;
extern const gh_pixel GH_GRAY34              ;
extern const gh_pixel GH_GRAY35              ;
extern const gh_pixel GH_GRAY36              ;
extern const gh_pixel GH_GRAY37              ;
extern const gh_pixel GH_GRAY38              ;
extern const gh_pixel GH_GRAY39              ;
extern const gh_pixel GH_GRAY3               ;
extern const gh_pixel GH_GRAY40              ;
extern const gh_pixel GH_GRAY41              ;
extern const gh_pixel GH_GRAY42              ;
extern const gh_pixel GH_GRAY43              ;
extern const gh_pixel GH_GRAY44              ;
extern const gh_pixel GH_GRAY45              ;
extern const gh_pixel GH_GRAY46              ;
extern const gh_pixel GH_GRAY47              ;
extern const gh_pixel GH_GRAY48              ;
extern const gh_pixel GH_GRAY49              ;
extern const gh_pixel GH_GRAY4               ;
extern const gh_pixel GH_GRAY50              ;
extern const gh_pixel GH_GRAY51              ;
extern const gh_pixel GH_GRAY52              ;
extern const gh_pixel GH_GRAY53              ;
extern const gh_pixel GH_GRAY54              ;
extern const gh_pixel GH_GRAY55              ;
extern const gh_pixel GH_GRAY56              ;
extern const gh_pixel GH_GRAY57              ;
extern const gh_pixel GH_GRAY58              ;
extern const gh_pixel GH_GRAY59              ;
extern const gh_pixel GH_GRAY5               ;
extern const gh_pixel GH_GRAY60              ;
extern const gh_pixel GH_GRAY61              ;
extern const gh_pixel GH_GRAY62              ;
extern const gh_pixel GH_GRAY63              ;
extern const gh_pixel GH_GRAY64              ;
extern const gh_pixel GH_GRAY65              ;
extern const gh_pixel GH_GRAY66              ;
extern const gh_pixel GH_GRAY67              ;
extern const gh_pixel GH_GRAY68              ;
extern const gh_pixel GH_GRAY69              ;
extern const gh_pixel GH_GRAY6               ;
extern const gh_pixel GH_GRAY70              ;
extern const gh_pixel GH_GRAY71              ;
extern const gh_pixel GH_GRAY72              ;
extern const gh_pixel GH_GRAY73              ;
extern const gh_pixel GH_GRAY74              ;
extern const gh_pixel GH_GRAY75              ;
extern const gh_pixel GH_GRAY76              ;
extern const gh_pixel GH_GRAY77              ;
extern const gh_pixel GH_GRAY78              ;
extern const gh_pixel GH_GRAY79              ;
extern const gh_pixel GH_GRAY7               ;
extern const gh_pixel GH_GRAY80              ;
extern const gh_pixel GH_GRAY81              ;
extern const gh_pixel GH_GRAY82              ;
extern const gh_pixel GH_GRAY83              ;
extern const gh_pixel GH_GRAY84              ;
extern const gh_pixel GH_GRAY85              ;
extern const gh_pixel GH_GRAY86              ;
extern const gh_pixel GH_GRAY87              ;
extern const gh_pixel GH_GRAY88              ;
extern const gh_pixel GH_GRAY89              ;
extern const gh_pixel GH_GRAY8               ;
extern const gh_pixel GH_GRAY90              ;
extern const gh_pixel GH_GRAY91              ;
extern const gh_pixel GH_GRAY92              ;
extern const gh_pixel GH_GRAY93              ;
extern const gh_pixel GH_GRAY94              ;
extern const gh_pixel GH_GRAY95              ;
extern const gh_pixel GH_GRAY96              ;
extern const gh_pixel GH_GRAY97              ;
extern const gh_pixel GH_GRAY98              ;
extern const gh_pixel GH_GRAY99              ;
extern const gh_pixel GH_GRAY9               ;
extern const gh_pixel GH_GRAY                ;
extern const gh_pixel GH_GREEN1              ;
extern const gh_pixel GH_GREEN2              ;
extern const gh_pixel GH_GREEN3              ;
extern const gh_pixel GH_GREEN4              ;
extern const gh_pixel GH_GREEN               ;
extern const gh_pixel GH_GREEN_YELLOW        ;
extern const gh_pixel GH_GREENYELLOW         ;
extern const gh_pixel GH_GREY10              ;
extern const gh_pixel GH_GREY11              ;
extern const gh_pixel GH_GREY12              ;
extern const gh_pixel GH_GREY13              ;
extern const gh_pixel GH_GREY14              ;
extern const gh_pixel GH_GREY15              ;
extern const gh_pixel GH_GREY16              ;
extern const gh_pixel GH_GREY17              ;
extern const gh_pixel GH_GREY18              ;
extern const gh_pixel GH_GREY19              ;
extern const gh_pixel GH_GREY1               ;
extern const gh_pixel GH_GREY20              ;
extern const gh_pixel GH_GREY21              ;
extern const gh_pixel GH_GREY22              ;
extern const gh_pixel GH_GREY23              ;
extern const gh_pixel GH_GREY24              ;
extern const gh_pixel GH_GREY25              ;
extern const gh_pixel GH_GREY26              ;
extern const gh_pixel GH_GREY27              ;
extern const gh_pixel GH_GREY28              ;
extern const gh_pixel GH_GREY29              ;
extern const gh_pixel GH_GREY2               ;
extern const gh_pixel GH_GREY30              ;
extern const gh_pixel GH_GREY31              ;
extern const gh_pixel GH_GREY32              ;
extern const gh_pixel GH_GREY33              ;
extern const gh_pixel GH_GREY34              ;
extern const gh_pixel GH_GREY35              ;
extern const gh_pixel GH_GREY36              ;
extern const gh_pixel GH_GREY37              ;
extern const gh_pixel GH_GREY38              ;
extern const gh_pixel GH_GREY39              ;
extern const gh_pixel GH_GREY3               ;
extern const gh_pixel GH_GREY40              ;
extern const gh_pixel GH_GREY41              ;
extern const gh_pixel GH_GREY42              ;
extern const gh_pixel GH_GREY43              ;
extern const gh_pixel GH_GREY44              ;
extern const gh_pixel GH_GREY45              ;
extern const gh_pixel GH_GREY46              ;
extern const gh_pixel GH_GREY47              ;
extern const gh_pixel GH_GREY48              ;
extern const gh_pixel GH_GREY49              ;
extern const gh_pixel GH_GREY4               ;
extern const gh_pixel GH_GREY50              ;
extern const gh_pixel GH_GREY51              ;
extern const gh_pixel GH_GREY52              ;
extern const gh_pixel GH_GREY53              ;
extern const gh_pixel GH_GREY54              ;
extern const gh_pixel GH_GREY55              ;
extern const gh_pixel GH_GREY56              ;
extern const gh_pixel GH_GREY57              ;
extern const gh_pixel GH_GREY58              ;
extern const gh_pixel GH_GREY59              ;
extern const gh_pixel GH_GREY5               ;
extern const gh_pixel GH_GREY60              ;
extern const gh_pixel GH_GREY61              ;
extern const gh_pixel GH_GREY62              ;
extern const gh_pixel GH_GREY63              ;
extern const gh_pixel GH_GREY64              ;
extern const gh_pixel GH_GREY65              ;
extern const gh_pixel GH_GREY66              ;
extern const gh_pixel GH_GREY67              ;
extern const gh_pixel GH_GREY68              ;
extern const gh_pixel GH_GREY69              ;
extern const gh_pixel GH_GREY6               ;
extern const gh_pixel GH_GREY70              ;
extern const gh_pixel GH_GREY71              ;
extern const gh_pixel GH_GREY72              ;
extern const gh_pixel GH_GREY73              ;
extern const gh_pixel GH_GREY74              ;
extern const gh_pixel GH_GREY75              ;
extern const gh_pixel GH_GREY76              ;
extern const gh_pixel GH_GREY77              ;
extern const gh_pixel GH_GREY78              ;
extern const gh_pixel GH_GREY79              ;
extern const gh_pixel GH_GREY7               ;
extern const gh_pixel GH_GREY80              ;
extern const gh_pixel GH_GREY81              ;
extern const gh_pixel GH_GREY82              ;
extern const gh_pixel GH_GREY83              ;
extern const gh_pixel GH_GREY84              ;
extern const gh_pixel GH_GREY85              ;
extern const gh_pixel GH_GREY86              ;
extern const gh_pixel GH_GREY87              ;
extern const gh_pixel GH_GREY88              ;
extern const gh_pixel GH_GREY89              ;
extern const gh_pixel GH_GREY8               ;
extern const gh_pixel GH_GREY90              ;
extern const gh_pixel GH_GREY91              ;
extern const gh_pixel GH_GREY92              ;
extern const gh_pixel GH_GREY93              ;
extern const gh_pixel GH_GREY94              ;
extern const gh_pixel GH_GREY95              ;
extern const gh_pixel GH_GREY96              ;
extern const gh_pixel GH_GREY97              ;
extern const gh_pixel GH_GREY98              ;
extern const gh_pixel GH_GREY99              ;
extern const gh_pixel GH_GREY9               ;
extern const gh_pixel GH_GREY                ;
extern const gh_pixel GH_HONEYDEW1           ;
extern const gh_pixel GH_HONEYDEW2           ;
extern const gh_pixel GH_HONEYDEW3           ;
extern const gh_pixel GH_HONEYDEW4           ;
extern const gh_pixel GH_HONEYDEW            ;
extern const gh_pixel GH_HOTPINK1            ;
extern const gh_pixel GH_HOTPINK2            ;
extern const gh_pixel GH_HOTPINK3            ;
extern const gh_pixel GH_HOTPINK4            ;
extern const gh_pixel GH_HOT_PINK            ;
extern const gh_pixel GH_HOTPINK             ;
extern const gh_pixel GH_INDIANRED1          ;
extern const gh_pixel GH_INDIANRED2          ;
extern const gh_pixel GH_INDIANRED3          ;
extern const gh_pixel GH_INDIANRED4          ;
extern const gh_pixel GH_INDIAN_RED          ;
extern const gh_pixel GH_INDIANRED           ;
extern const gh_pixel GH_IVORY1              ;
extern const gh_pixel GH_IVORY2              ;
extern const gh_pixel GH_IVORY3              ;
extern const gh_pixel GH_IVORY4              ;
extern const gh_pixel GH_IVORY               ;
extern const gh_pixel GH_KHAKI1              ;
extern const gh_pixel GH_KHAKI2              ;
extern const gh_pixel GH_KHAKI3              ;
extern const gh_pixel GH_KHAKI4              ;
extern const gh_pixel GH_KHAKI               ;
extern const gh_pixel GH_LAVENDERBLUSH1      ;
extern const gh_pixel GH_LAVENDERBLUSH2      ;
extern const gh_pixel GH_LAVENDERBLUSH3      ;
extern const gh_pixel GH_LAVENDERBLUSH4      ;
extern const gh_pixel GH_LAVENDER_BLUSH      ;
extern const gh_pixel GH_LAVENDERBLUSH       ;
extern const gh_pixel GH_LAVENDER            ;
extern const gh_pixel GH_LAWN_GREEN          ;
extern const gh_pixel GH_LAWNGREEN           ;
extern const gh_pixel GH_LEMONCHIFFON1       ;
extern const gh_pixel GH_LEMONCHIFFON2       ;
extern const gh_pixel GH_LEMONCHIFFON3       ;
extern const gh_pixel GH_LEMONCHIFFON4       ;
extern const gh_pixel GH_LEMON_CHIFFON       ;
extern const gh_pixel GH_LEMONCHIFFON        ;
extern const gh_pixel GH_LIGHTBLUE1          ;
extern const gh_pixel GH_LIGHTBLUE2          ;
extern const gh_pixel GH_LIGHTBLUE3          ;
extern const gh_pixel GH_LIGHTBLUE4          ;
extern const gh_pixel GH_LIGHT_BLUE          ;
extern const gh_pixel GH_LIGHTBLUE           ;
extern const gh_pixel GH_LIGHT_CORAL         ;
extern const gh_pixel GH_LIGHTCORAL          ;
extern const gh_pixel GH_LIGHTCYAN1          ;
extern const gh_pixel GH_LIGHTCYAN2          ;
extern const gh_pixel GH_LIGHTCYAN3          ;
extern const gh_pixel GH_LIGHTCYAN4          ;
extern const gh_pixel GH_LIGHT_CYAN          ;
extern const gh_pixel GH_LIGHTCYAN           ;
extern const gh_pixel GH_LIGHTGOLDENROD1     ;
extern const gh_pixel GH_LIGHTGOLDENROD2     ;
extern const gh_pixel GH_LIGHTGOLDENROD3     ;
extern const gh_pixel GH_LIGHTGOLDENROD4     ;
extern const gh_pixel GH_LIGHT_GOLDENROD     ;
extern const gh_pixel GH_LIGHTGOLDENROD      ;
extern const gh_pixel GH_LIGHT_GOLDENROD_YELLOW ;
extern const gh_pixel GH_LIGHTGOLDENRODYELLOW ;
extern const gh_pixel GH_LIGHT_GRAY          ;
extern const gh_pixel GH_LIGHTGRAY           ;
extern const gh_pixel GH_LIGHT_GREEN         ;
extern const gh_pixel GH_LIGHTGREEN          ;
extern const gh_pixel GH_LIGHT_GREY          ;
extern const gh_pixel GH_LIGHTGREY           ;
extern const gh_pixel GH_LIGHTPINK1          ;
extern const gh_pixel GH_LIGHTPINK2          ;
extern const gh_pixel GH_LIGHTPINK3          ;
extern const gh_pixel GH_LIGHTPINK4          ;
extern const gh_pixel GH_LIGHT_PINK          ;
extern const gh_pixel GH_LIGHTPINK           ;
extern const gh_pixel GH_LIGHTSALMON1        ;
extern const gh_pixel GH_LIGHTSALMON2        ;
extern const gh_pixel GH_LIGHTSALMON3        ;
extern const gh_pixel GH_LIGHTSALMON4        ;
extern const gh_pixel GH_LIGHT_SALMON        ;
extern const gh_pixel GH_LIGHTSALMON         ;
extern const gh_pixel GH_LIGHT_SEA_GREEN     ;
extern const gh_pixel GH_LIGHTSEAGREEN       ;
extern const gh_pixel GH_LIGHTSKYBLUE1       ;
extern const gh_pixel GH_LIGHTSKYBLUE2       ;
extern const gh_pixel GH_LIGHTSKYBLUE3       ;
extern const gh_pixel GH_LIGHTSKYBLUE4       ;
extern const gh_pixel GH_LIGHT_SKY_BLUE      ;
extern const gh_pixel GH_LIGHTSKYBLUE        ;
extern const gh_pixel GH_LIGHT_SLATE_BLUE    ;
extern const gh_pixel GH_LIGHTSLATEBLUE      ;
extern const gh_pixel GH_LIGHT_SLATE_GRAY    ;
extern const gh_pixel GH_LIGHTSLATEGRAY      ;
extern const gh_pixel GH_LIGHT_SLATE_GREY    ;
extern const gh_pixel GH_LIGHTSLATEGREY      ;
extern const gh_pixel GH_LIGHTSTEELBLUE1     ;
extern const gh_pixel GH_LIGHTSTEELBLUE2     ;
extern const gh_pixel GH_LIGHTSTEELBLUE3     ;
extern const gh_pixel GH_LIGHTSTEELBLUE4     ;
extern const gh_pixel GH_LIGHT_STEEL_BLUE    ;
extern const gh_pixel GH_LIGHTSTEELBLUE      ;
extern const gh_pixel GH_LIGHTYELLOW1        ;
extern const gh_pixel GH_LIGHTYELLOW2        ;
extern const gh_pixel GH_LIGHTYELLOW3        ;
extern const gh_pixel GH_LIGHTYELLOW4        ;
extern const gh_pixel GH_LIGHT_YELLOW        ;
extern const gh_pixel GH_LIGHTYELLOW         ;
extern const gh_pixel GH_LIME_GREEN          ;
extern const gh_pixel GH_LIMEGREEN           ;
extern const gh_pixel GH_LIME                ;
extern const gh_pixel GH_LINEN               ;
extern const gh_pixel GH_MAGENTA1            ;
extern const gh_pixel GH_MAGENTA2            ;
extern const gh_pixel GH_MAGENTA3            ;
extern const gh_pixel GH_MAGENTA4            ;
extern const gh_pixel GH_MAGENTA             ;
extern const gh_pixel GH_MAROON1             ;
extern const gh_pixel GH_MAROON2             ;
extern const gh_pixel GH_MAROON3             ;
extern const gh_pixel GH_MAROON4             ;
extern const gh_pixel GH_MAROON              ;
extern const gh_pixel GH_MEDIUM_AQUAMARINE   ;
extern const gh_pixel GH_MEDIUMAQUAMARINE    ;
extern const gh_pixel GH_MEDIUM_BLUE         ;
extern const gh_pixel GH_MEDIUMBLUE          ;
extern const gh_pixel GH_MEDIUMORCHID1       ;
extern const gh_pixel GH_MEDIUMORCHID2       ;
extern const gh_pixel GH_MEDIUMORCHID3       ;
extern const gh_pixel GH_MEDIUMORCHID4       ;
extern const gh_pixel GH_MEDIUM_ORCHID       ;
extern const gh_pixel GH_MEDIUMORCHID        ;
extern const gh_pixel GH_MEDIUMPURPLE1       ;
extern const gh_pixel GH_MEDIUMPURPLE2       ;
extern const gh_pixel GH_MEDIUMPURPLE3       ;
extern const gh_pixel GH_MEDIUMPURPLE4       ;
extern const gh_pixel GH_MEDIUM_PURPLE       ;
extern const gh_pixel GH_MEDIUMPURPLE        ;
extern const gh_pixel GH_MEDIUM_SEA_GREEN    ;
extern const gh_pixel GH_MEDIUMSEAGREEN      ;
extern const gh_pixel GH_MEDIUM_SLATE_BLUE   ;
extern const gh_pixel GH_MEDIUMSLATEBLUE     ;
extern const gh_pixel GH_MEDIUM_SPRING_GREEN ;
extern const gh_pixel GH_MEDIUMSPRINGGREEN   ;
extern const gh_pixel GH_MEDIUM_TURQUOISE    ;
extern const gh_pixel GH_MEDIUMTURQUOISE     ;
extern const gh_pixel GH_MEDIUM_VIOLET_RED   ;
extern const gh_pixel GH_MEDIUMVIOLETRED     ;
extern const gh_pixel GH_MIDNIGHT_BLUE       ;
extern const gh_pixel GH_MIDNIGHTBLUE        ;
extern const gh_pixel GH_MINT_CREAM          ;
extern const gh_pixel GH_MINTCREAM           ;
extern const gh_pixel GH_MISTYROSE1          ;
extern const gh_pixel GH_MISTYROSE2          ;
extern const gh_pixel GH_MISTYROSE3          ;
extern const gh_pixel GH_MISTYROSE4          ;
extern const gh_pixel GH_MISTY_ROSE          ;
extern const gh_pixel GH_MISTYROSE           ;
extern const gh_pixel GH_MOCCASIN            ;
extern const gh_pixel GH_NAVAJOWHITE1        ;
extern const gh_pixel GH_NAVAJOWHITE2        ;
extern const gh_pixel GH_NAVAJOWHITE3        ;
extern const gh_pixel GH_NAVAJOWHITE4        ;
extern const gh_pixel GH_NAVAJO_WHITE        ;
extern const gh_pixel GH_NAVAJOWHITE         ;
extern const gh_pixel GH_NAVY_BLUE           ;
extern const gh_pixel GH_NAVYBLUE            ;
extern const gh_pixel GH_NAVY                ;
extern const gh_pixel GH_OLD_LACE            ;
extern const gh_pixel GH_OLDLACE             ;
extern const gh_pixel GH_OLIVEDRAB1          ;
extern const gh_pixel GH_OLIVEDRAB2          ;
extern const gh_pixel GH_OLIVEDRAB3          ;
extern const gh_pixel GH_OLIVEDRAB4          ;
extern const gh_pixel GH_OLIVE_DRAB          ;
extern const gh_pixel GH_OLIVEDRAB           ;
extern const gh_pixel GH_LIGHT_ORANGE        ;
extern const gh_pixel GH_ORANGE1             ;
extern const gh_pixel GH_ORANGE2             ;
extern const gh_pixel GH_ORANGE3             ;
extern const gh_pixel GH_ORANGE4             ;
extern const gh_pixel GH_ORANGE              ;
extern const gh_pixel GH_ORANGERED1          ;
extern const gh_pixel GH_ORANGERED2          ;
extern const gh_pixel GH_ORANGERED3          ;
extern const gh_pixel GH_ORANGERED4          ;
extern const gh_pixel GH_ORANGE_RED          ;
extern const gh_pixel GH_ORANGERED           ;
extern const gh_pixel GH_ORCHID1             ;
extern const gh_pixel GH_ORCHID2             ;
extern const gh_pixel GH_ORCHID3             ;
extern const gh_pixel GH_ORCHID4             ;
extern const gh_pixel GH_ORCHID              ;
extern const gh_pixel GH_PALE_GOLDENROD      ;
extern const gh_pixel GH_PALEGOLDENROD       ;
extern const gh_pixel GH_PALEGREEN1          ;
extern const gh_pixel GH_PALEGREEN2          ;
extern const gh_pixel GH_PALEGREEN3          ;
extern const gh_pixel GH_PALEGREEN4          ;
extern const gh_pixel GH_PALE_GREEN          ;
extern const gh_pixel GH_PALEGREEN           ;
extern const gh_pixel GH_PALETURQUOISE1      ;
extern const gh_pixel GH_PALETURQUOISE2      ;
extern const gh_pixel GH_PALETURQUOISE3      ;
extern const gh_pixel GH_PALETURQUOISE4      ;
extern const gh_pixel GH_PALE_TURQUOISE      ;
extern const gh_pixel GH_PALETURQUOISE       ;
extern const gh_pixel GH_PALEVIOLETRED1      ;
extern const gh_pixel GH_PALEVIOLETRED2      ;
extern const gh_pixel GH_PALEVIOLETRED3      ;
extern const gh_pixel GH_PALEVIOLETRED4      ;
extern const gh_pixel GH_PALE_VIOLET_RED     ;
extern const gh_pixel GH_PALEVIOLETRED       ;
extern const gh_pixel GH_PAPAYA_WHIP         ;
extern const gh_pixel GH_PAPAYAWHIP          ;
extern const gh_pixel GH_PEACHPUFF1          ;
extern const gh_pixel GH_PEACHPUFF2          ;
extern const gh_pixel GH_PEACHPUFF3          ;
extern const gh_pixel GH_PEACHPUFF4          ;
extern const gh_pixel GH_PEACH_PUFF          ;
extern const gh_pixel GH_PEACHPUFF           ;
extern const gh_pixel GH_PERU                ;
extern const gh_pixel GH_PINK1               ;
extern const gh_pixel GH_PINK2               ;
extern const gh_pixel GH_PINK3               ;
extern const gh_pixel GH_PINK4               ;
extern const gh_pixel GH_PINK                ;
extern const gh_pixel GH_PLUM1               ;
extern const gh_pixel GH_PLUM2               ;
extern const gh_pixel GH_PLUM3               ;
extern const gh_pixel GH_PLUM4               ;
extern const gh_pixel GH_PLUM                ;
extern const gh_pixel GH_POWDER_BLUE         ;
extern const gh_pixel GH_POWDERBLUE          ;
extern const gh_pixel GH_PURPLE1             ;
extern const gh_pixel GH_PURPLE2             ;
extern const gh_pixel GH_PURPLE3             ;
extern const gh_pixel GH_PURPLE4             ;
extern const gh_pixel GH_PURPLE              ;
extern const gh_pixel GH_RED1                ;
extern const gh_pixel GH_RED2                ;
extern const gh_pixel GH_RED3                ;
extern const gh_pixel GH_RED4                ;
extern const gh_pixel GH_RED                 ;
extern const gh_pixel GH_ROSYBROWN1          ;
extern const gh_pixel GH_ROSYBROWN2          ;
extern const gh_pixel GH_ROSYBROWN3          ;
extern const gh_pixel GH_ROSYBROWN4          ;
extern const gh_pixel GH_ROSY_BROWN          ;
extern const gh_pixel GH_ROSYBROWN           ;
extern const gh_pixel GH_ROYALBLUE1          ;
extern const gh_pixel GH_ROYALBLUE2          ;
extern const gh_pixel GH_ROYALBLUE3          ;
extern const gh_pixel GH_ROYALBLUE4          ;
extern const gh_pixel GH_ROYAL_BLUE          ;
extern const gh_pixel GH_ROYALBLUE           ;
extern const gh_pixel GH_SADDLE_BROWN        ;
extern const gh_pixel GH_SADDLEBROWN         ;
extern const gh_pixel GH_SALMON1             ;
extern const gh_pixel GH_SALMON2             ;
extern const gh_pixel GH_SALMON3             ;
extern const gh_pixel GH_SALMON4             ;
extern const gh_pixel GH_SALMON              ;
extern const gh_pixel GH_SANDY_BROWN         ;
extern const gh_pixel GH_SANDYBROWN          ;
extern const gh_pixel GH_SEAGREEN1           ;
extern const gh_pixel GH_SEAGREEN2           ;
extern const gh_pixel GH_SEAGREEN3           ;
extern const gh_pixel GH_SEAGREEN4           ;
extern const gh_pixel GH_SEA_GREEN           ;
extern const gh_pixel GH_SEAGREEN            ;
extern const gh_pixel GH_SEASHELL1           ;
extern const gh_pixel GH_SEASHELL2           ;
extern const gh_pixel GH_SEASHELL3           ;
extern const gh_pixel GH_SEASHELL4           ;
extern const gh_pixel GH_SEASHELL            ;
extern const gh_pixel GH_SIENNA1             ;
extern const gh_pixel GH_SIENNA2             ;
extern const gh_pixel GH_SIENNA3             ;
extern const gh_pixel GH_SIENNA4             ;
extern const gh_pixel GH_SIENNA              ;
extern const gh_pixel GH_SKYBLUE1            ;
extern const gh_pixel GH_SKYBLUE2            ;
extern const gh_pixel GH_SKYBLUE3            ;
extern const gh_pixel GH_SKYBLUE4            ;
extern const gh_pixel GH_SKY_BLUE            ;
extern const gh_pixel GH_SKYBLUE             ;
extern const gh_pixel GH_SLATEBLUE1          ;
extern const gh_pixel GH_SLATEBLUE2          ;
extern const gh_pixel GH_SLATEBLUE3          ;
extern const gh_pixel GH_SLATEBLUE4          ;
extern const gh_pixel GH_SLATE_BLUE          ;
extern const gh_pixel GH_SLATEBLUE           ;
extern const gh_pixel GH_SLATEGRAY1          ;
extern const gh_pixel GH_SLATEGRAY2          ;
extern const gh_pixel GH_SLATEGRAY3          ;
extern const gh_pixel GH_SLATEGRAY4          ;
extern const gh_pixel GH_SLATE_GRAY          ;
extern const gh_pixel GH_SLATEGRAY           ;
extern const gh_pixel GH_SLATE_GREY          ;
extern const gh_pixel GH_SLATEGREY           ;
extern const gh_pixel GH_SNOW1               ;
extern const gh_pixel GH_SNOW2               ;
extern const gh_pixel GH_SNOW3               ;
extern const gh_pixel GH_SNOW4               ;
extern const gh_pixel GH_SNOW                ;
extern const gh_pixel GH_SPRINGGREEN1        ;
extern const gh_pixel GH_SPRINGGREEN2        ;
extern const gh_pixel GH_SPRINGGREEN3        ;
extern const gh_pixel GH_SPRINGGREEN4        ;
extern const gh_pixel GH_SPRING_GREEN        ;
extern const gh_pixel GH_SPRINGGREEN         ;
extern const gh_pixel GH_STEELBLUE1          ;
extern const gh_pixel GH_STEELBLUE2          ;
extern const gh_pixel GH_STEELBLUE3          ;
extern const gh_pixel GH_STEELBLUE4          ;
extern const gh_pixel GH_STEEL_BLUE          ;
extern const gh_pixel GH_STEELBLUE           ;
extern const gh_pixel GH_TAN1                ;
extern const gh_pixel GH_TAN2                ;
extern const gh_pixel GH_TAN3                ;
extern const gh_pixel GH_TAN4                ;
extern const gh_pixel GH_TAN                 ;
extern const gh_pixel GH_THISTLE1            ;
extern const gh_pixel GH_THISTLE2            ;
extern const gh_pixel GH_THISTLE3            ;
extern const gh_pixel GH_THISTLE4            ;
extern const gh_pixel GH_THISTLE             ;
extern const gh_pixel GH_TOMATO1             ;
extern const gh_pixel GH_TOMATO2             ;
extern const gh_pixel GH_TOMATO3             ;
extern const gh_pixel GH_TOMATO4             ;
extern const gh_pixel GH_TOMATO              ;
extern const gh_pixel GH_TURQUOISE1          ;
extern const gh_pixel GH_TURQUOISE2          ;
extern const gh_pixel GH_TURQUOISE3          ;
extern const gh_pixel GH_TURQUOISE4          ;
extern const gh_pixel GH_TURQUOISE           ;
extern const gh_pixel GH_VIOLET              ;
extern const gh_pixel GH_VIOLETRED1          ;
extern const gh_pixel GH_VIOLETRED2          ;
extern const gh_pixel GH_VIOLETRED3          ;
extern const gh_pixel GH_VIOLETRED4          ;
extern const gh_pixel GH_VIOLET_RED          ;
extern const gh_pixel GH_VIOLETRED           ;
extern const gh_pixel GH_WHEAT1              ;
extern const gh_pixel GH_WHEAT2              ;
extern const gh_pixel GH_WHEAT3              ;
extern const gh_pixel GH_WHEAT4              ;
extern const gh_pixel GH_WHEAT               ;
extern const gh_pixel GH_WHITE               ;
extern const gh_pixel GH_WHITE_SMOKE         ;
extern const gh_pixel GH_WHITESMOKE          ;
extern const gh_pixel GH_YELLOW1             ;
extern const gh_pixel GH_YELLOW2             ;
extern const gh_pixel GH_YELLOW3             ;
extern const gh_pixel GH_YELLOW4             ;
extern const gh_pixel GH_YELLOW_GREEN        ;
extern const gh_pixel GH_YELLOWGREEN         ;
extern const gh_pixel GH_YELLOW              ;

#endif /* _GH_PIXEL_H_ */
