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

    $Id: gh_macro.h 519 2007-06-04 17:47:47Z goblinhack $
*/

#ifndef _GH_MACRO_H_
#define _GH_MACRO_H_

#define GH_STRUCT_OFFSET(type, field)    ((int) ((char *) &((type *) 0)->field))

#define GH_MULTIPLE_BITS(bitmask)       ((bitmask) & ((bitmask)-1))

static inline uint32 next_pow_of_2 (uint32 v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return (v);
}

#define GH_ARRAY_SIZE(a)    (sizeof(a)/sizeof(a[0]))

#define GH_STRUCT_INIT(__str__) memset(&(__str__), 0, sizeof(__str__))

#define GH_FOR_ALL_IN_ARRAY(iterator, array) \
    for ((iterator) = (array); \
         (iterator) < ((array) + GH_ARRAY_SIZE(array)); \
         (iterator)++)

#if defined(__GNUC__) /* { */
#    if (__GNUC__==4) 
#            define GH_COMPILER_PACKED(__MY_TYPE__) \
                                __attribute__((packed)) __MY_TYPE__
    /*
     * Cater for gcc 3.4.3 lameness.
     */
#    elif (__GNUC__==3) && (__GNUC_MINOR__==4) && (__GNUC_PATCHLEVEL__==3) /* { */
#        ifdef __cplusplus /* { */
#            define GH_COMPILER_PACKED(__TYPE__) __TYPE__
#        else /* } { */
#            define GH_COMPILER_PACKED(__MY_TYPE__) \
                                __attribute__((packed)) __MY_TYPE__
#        endif /* } */
#    else /* } { */
#        define GH_COMPILER_PACKED(__MY_TYPE__) \
                                __MY_TYPE__ __attribute__((mode(__byte__)))
#    endif /* } */
#else /* } { */
#    define GH_COMPILER_PACKED(__TYPE__) __TYPE___
#endif /* } */

//
// An enum syc technique.
//
#define ENUM_ARG(enum_arg, string_arg)           enum_arg
#define STRING_ARG(enum_arg, string_arg)         string_arg

#define DEFINE_ENUM(_name, _NAME)       \
    enum _name {                        \
        _NAME ## _LIST_MACRO(ENUM_ARG), \
        _NAME ## _MAX,                  \
    };                                                                          
                                                                                
#define DECLARE_ENUM_STRINGS(_name, _NAME)                      \
    const char *const _name ## _strings[_NAME ## _MAX] = {      \
        _NAME ## _LIST_MACRO(STRING_ARG)                        \
    };                                                                          
                                                 
#define GL_ERROR_CHECK() \
{\
    int gl_error = glGetError();\
    if (gl_error != GL_NO_ERROR) {\
        GH_WARN((string)"OpenGL " + (char*)gluErrorString(gl_error));\
    }\
}

//
// Avoid functions for these so that non optimised code is quicker too.
//
#define GH_MIN(a, b) ((a < b) ? a : b)
#define GH_MAX(a, b) ((a < b) ? b : a)

//
// #define LABEL(x) [x] =
//
// C++ is a bit crap and can't handle designated initializers
//
#define LABEL(x) 

#endif /* _GH_MACRO_H_ */
