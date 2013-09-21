/*
    debugit.
    Copyright (C) 2004 Neil McGill

    This code is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This tool is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this tool; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: debugit.cpp 875 2008-05-18 03:19:49Z Ghack $
*/

/*
 * __DEBUGIT__IGNORE_THIS_FILE_FOR_INSTALL
 * __DEBUGIT__IGNORE_THIS_FILE_FOR_REMOVE
 */
#include "debugit.h"
#include <string.h>
#include <ctype.h>

/*
 * The global on/off flag.
 */
int __debugit_flag__on = 0;
int __debugit_flag__instrument_only = 0;

#ifndef __DEBUGIT__PRINTER__
#include <stdio.h>
#define __DEBUGIT__PRINTER__ printf

static const char *__DEBUGIT__PREFIX = "";
static const char *__DEBUGIT__SUFFIX = "\n";
static const int __DEBUGIT__FILE_WIDTH = 20;

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#endif

/*
 * Global on off flag:
 */
int __DEBUGIT__ENABLED = 1;

/*
 * Local settings:
 */
static int __debugit__depth = 0;

static const int __DEBUGIT__DEPTH_MAX = 100;
static const char __DEBUGIT__INDENT[] = "            CRASH SOON !!!                                                                                                                                                                                                                                                                                                                                                                                                                                                                     ";

static inline const char *file_show (const char *file)
{
    const int sz = (int)strlen(file);

    if (sz > __DEBUGIT__FILE_WIDTH) {
        return (file + (sz - __DEBUGIT__FILE_WIDTH));
    }
    return (file);
}

static inline const char *indent_show (const int depth)
{
    return (&__DEBUGIT__INDENT[sizeof(__DEBUGIT__INDENT) - depth]);
}

void __debugit__beginning_of_function (const char *const file,
                                       const char* function,
                                       const int line,
                                       const char *const text)
{
    __debugit_flag__on++;

    __debugit__depth++;

    __DEBUGIT__PRINTER__("%s%05d %-*s %s%s() {%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        function,
                        __DEBUGIT__SUFFIX);
}

void __debugit__end_of_function (const char *const file,
                                 const char* function,
                                 const int line,
                                 const char *const text)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s} /* %s [%s] */%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        function, text,
                        __DEBUGIT__SUFFIX);

    if (__debugit__depth >= 1) {
        __debugit__depth--;
        if (__debugit_flag__on >= 1) {
            __debugit_flag__on--;
        }
    }
}

void __debugit__argument_cptr (const char *const file,
                               const char *const function,
                               const int line,
                               const char *const text,
                               const char *const ptr)
{
    char ptr_copy[17];
    int i;

    if (ptr == NULL) {
        __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=(nil)%s",
                            __DEBUGIT__PREFIX,
                            line, __DEBUGIT__FILE_WIDTH,
                            file_show(file),
                            indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                            text,
                            __DEBUGIT__SUFFIX);
        return;
    }
    
    for (i=0; i<(int)sizeof(ptr_copy)-1; i++) {
        const char c = ptr[i];

        //
        // End of a null terminated string? 
        //
        if (ptr[i] == '\0') {
            for (;i<(int)sizeof(ptr_copy)-1; i++) {
                ptr_copy[i] = '\0';
            }
            break;
        }

        //
        // If we can't print as a string, dump it as binary
        //
        if (!isprint((int)c)) {
            const unsigned char *uptr = (const unsigned char *)ptr;

            __DEBUGIT__PRINTER__("%s%05d %-*s %s "
                                "%s=%02X%02X %02X%02X %02X%02X %02X%02X "
                                "%02X%02X %02X%02X %02X%02X %02X%02X %s",
                                __DEBUGIT__PREFIX,
                                line, __DEBUGIT__FILE_WIDTH,
                                file_show(file),
                                indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                                text,
                                (unsigned int)uptr[0],
                                (unsigned int)uptr[1], 
                                (unsigned int)uptr[2], 
                                (unsigned int)uptr[3],
                                
                                (unsigned int)uptr[4], 
                                (unsigned int)uptr[5], 
                                (unsigned int)uptr[6], 
                                (unsigned int)uptr[7],
                                
                                (unsigned int)uptr[8], 
                                (unsigned int)uptr[9], 
                                (unsigned int)uptr[10], 
                                (unsigned int)uptr[11],
                                
                                (unsigned int)uptr[12], 
                                (unsigned int)uptr[13], 
                                (unsigned int)uptr[14], 
                                (unsigned int)uptr[15],
                                __DEBUGIT__SUFFIX);
            return;
        }

        ptr_copy[i] = c;
    }
    ptr_copy[i] = '\0';

    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=\"%s\"%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, ptr_copy,
                        __DEBUGIT__SUFFIX);
}

void __debugit__argument_ptr (const char *const file,
                              const char *const function,
                              const int line,
                              const char *const text,
                              const void *const ptr)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%p%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, ptr,
                        __DEBUGIT__SUFFIX);
}

#ifndef __cplusplus
void __debugit__argument_ulonglong (const char *const file,
                                    const char *const function,
                                    const int line,
                                    const char *const text,
                                    const unsigned long long val)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%llu%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, val,
                        __DEBUGIT__SUFFIX);
}

void __debugit__argument_longlong (const char *const file,
                                   const char *const function,
                                   const int line,
                                   const char *const text,
                                   const long long val)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%lld%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, val,
                        __DEBUGIT__SUFFIX);
}
#endif

void __debugit__argument_ulong (const char *const file,
                                const char *const function,
                                const int line,
                                const char *const text,
                                const unsigned long val)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%lu%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, val,
                        __DEBUGIT__SUFFIX);
}

void __debugit__argument_long (const char *const file,
                               const char *const function,
                               const int line,
                               const char *const text,
                               const long val)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%ld%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, val,
                        __DEBUGIT__SUFFIX);
}

void __debugit__argument_uint (const char *const file,
                               const char *const function,
                               const int line,
                               const char *const text,
                               const unsigned int val)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%u%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, val,
                        __DEBUGIT__SUFFIX);
}

void __debugit__argument_int (const char *const file,
                              const char *const function,
                              const int line,
                              const char *const text,
                              const int val)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s=%d%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text, val,
                        __DEBUGIT__SUFFIX);
}

void __debugit__case (const char *const file,
                      const char *const function,
                      const int line,
                      const char *const text)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s case %s:%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text,
                        __DEBUGIT__SUFFIX);
}

void __debugit__text (const char *const file,
                      const char *const function,
                      const int line,
                      const char *const text)
{
    __DEBUGIT__PRINTER__("%s%05d %-*s %s %s%s",
                        __DEBUGIT__PREFIX,
                        line, __DEBUGIT__FILE_WIDTH,
                        file_show(file),
                        indent_show(min(__debugit__depth, __DEBUGIT__DEPTH_MAX)),
                        text,
                        __DEBUGIT__SUFFIX);
}
