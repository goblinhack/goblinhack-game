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

    $Id: debugit.h 875 2008-05-18 03:19:49Z Ghack $
*/

/*
 * __DEBUGIT__IGNORE_THIS_FILE_FOR_INSTALL
 * __DEBUGIT__IGNORE_THIS_FILE_FOR_REMOVE
 */

#ifndef __DEBUGIT__H__
#define __DEBUGIT__H__

extern int __debugit_flag__on;
extern int __debugit_flag__instrument_only;

extern void __debugit__beginning_of_function(const char *const file,
                                             const char *const function,
                                             const int line,
                                             const char *const text);

extern void __debugit__end_of_function(const char *const file,
                                       const char *const function,
                                       const int line,
                                       const char *const text);

extern void __debugit__argument_cptr(const char *const file,
                                     const char *const function,
                                     const int line,
                                     const char *const text,
                                     const char *const ptr);

extern void __debugit__argument_ptr(const char *const file,
                                    const char *const function,
                                    const int line,
                                    const char *const text,
                                    const void *const ptr);

#ifndef __cplusplus
extern void __debugit__argument_ulonglong(const char *const file,
                                          const char *const function,
                                          const int line,
                                          const char *const text,
                                          const unsigned long long val);

extern void __debugit__argument_longlong(const char *const file,
                                         const char *const function,
                                         const int line,
                                         const char *const text,
                                         const long long val);
#endif

extern void __debugit__argument_ulong(const char *const file,
                                      const char *const function,
                                      const int line,
                                      const char *const text,
                                      const unsigned long val);

extern void __debugit__argument_long(const char *const file,
                                     const char *const function,
                                     const int line,
                                     const char *const text,
                                     const long val);

extern void __debugit__argument_uint(const char *const file,
                                     const char *const function,
                                     const int line,
                                     const char *const text,
                                     const unsigned int val);

extern void __debugit__argument_int(const char *const file,
                                    const char *const function,
                                    const int line,
                                    const char *const text,
                                    const int val);

extern void __debugit__case(const char *const file,
                            const char *const function,
                            const int line,
                            const char *const text);

extern void __debugit__text(const char *const file,
                            const char *const function,
                            const int line,
                            const char *const text);

/*
 * Check if the global debug flag is on, or the per file flag, or the
 * per function flag.
 */
#define __ON__ (__debugit__on ||                                        \
                __debugit__on__ ## __FUNCTION__ ||                      \
                __debugit__on__ ## __FILE__ )

#define __DEBUGIT__BEGIN_FUNCTION__(__ON__, __TEXT__)                   \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__beginning_of_function(                               \
             __FILE__, __FUNCTION__, __LINE__, (__TEXT__));             \
    }                                                                   \
}

#define __DEBUGIT__END_FUNCTION__(__ON__, __TEXT__)                     \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__end_of_function(                                     \
             __FILE__, __FUNCTION__, __LINE__, (__TEXT__));             \
    }                                                                   \
}                                                                       \

#define __DEBUGIT__BEGIN_FUNCTION_BODY__ {

#define __DEBUGIT__END_FUNCTION_BODY__ }

#define __DEBUGIT__BEGIN_RETURN_STATEMENT__(__ON__, __TEXT__)           \
{                                                                       \
    __DEBUGIT__END_FUNCTION__(__ON__, (__TEXT__));

#define __DEBUGIT__END_RETURN_STATEMENT__                               \
}

#define __DEBUGIT__BEGIN_SPLIT_RETURN_STATEMENT__(__ON__, __TEXT__)     \
{                                                                       \
    __DEBUGIT__END_FUNCTION__(__ON__, (__TEXT__))

#define __DEBUGIT__END_SPLIT_RETURN_STATEMENT__                         \
}

#define __DEBUGIT__CASE(__ON__, __TEXT__)                               \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__case(__FILE__, __FUNCTION__, __LINE__, (__TEXT__));  \
    }                                                                   \
}

#define __DEBUGIT__TEXT(__ON__, __TEXT__)                               \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__text(__FILE__, __FUNCTION__, __LINE__, (__TEXT__));  \
    }                                                                   \
}

#define __DEBUGIT__ARG_CPTR(__ON__, __ARG__)                            \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_cptr(__FILE__, __FUNCTION__,                \
                __LINE__, #__ARG__, (const char*const)__ARG__);         \
    }                                                                   \
}

#define __DEBUGIT__ARG_PAKTYPE(__ON__, __ARG__)                                 \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_paktype(__FILE__, __FUNCTION__,             \
                __LINE__, #__ARG__, (const void*const)__ARG__);         \
    }                                                                   \
}

#define __DEBUGIT__ARG_IDB(__ON__, __ARG__)                             \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_idbtype(__FILE__, __FUNCTION__,             \
                __LINE__, #__ARG__, (const void*const)__ARG__);         \
    }                                                                   \
}

#define __DEBUGIT__ARG_FIBIDB(__ON__, __ARG__)                          \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_fibidbtype(__FILE__, __FUNCTION__,          \
                __LINE__, #__ARG__, (const void*const)__ARG__);         \
    }                                                                   \
}

#define __DEBUGIT__ARG_HWIDB(__ON__, __ARG__)                           \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_hwidbtype(__FILE__, __FUNCTION__,           \
                __LINE__, #__ARG__, (const void*const)__ARG__);         \
    }                                                                   \
}

#define __DEBUGIT__ARG_IPADDRTYPE(__ON__, __ARG__)                      \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_ipaddrtype(__FILE__, __FUNCTION__,          \
                __LINE__, #__ARG__, (const void*const)& __ARG__);       \
    }                                                                   \
}

#define __DEBUGIT__ARG_PTR(__ON__, __ARG__)                             \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_ptr(__FILE__, __FUNCTION__,                 \
                __LINE__, #__ARG__, (const void*const)__ARG__);         \
    }                                                                   \
}

#define __DEBUGIT__ARG_ULONGLONG(__ON__, __ARG__)                       \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_ulonglong(__FILE__, __FUNCTION__,           \
                __LINE__, #__ARG__,                                     \
                (unsigned long long)__ARG__);                           \
    }                                                                   \
}

#define __DEBUGIT__ARG_LONGLONG(__ON__, __ARG__)                        \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_longlong(__FILE__, __FUNCTION__,            \
                __LINE__, #__ARG__, (long long)__ARG__);                \
    }\
}

#define __DEBUGIT__ARG_ULONG(__ON__, __ARG__)                           \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_ulong(__FILE__, __FUNCTION__,               \
                __LINE__, #__ARG__, (unsigned long) __ARG__);           \
    }                                                                   \
}

#define __DEBUGIT__ARG_LONG(__ON__, __ARG__)                            \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_long(__FILE__, __FUNCTION__,                \
                __LINE__, #__ARG__, (long) __ARG__);                    \
    }                                                                   \
}

#define __DEBUGIT__ARG_UINT(__ON__, __ARG__)                            \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_uint(__FILE__, __FUNCTION__,                \
                __LINE__, #__ARG__, (unsigned int) __ARG__);            \
    }                                                                   \
}

#define __DEBUGIT__ARG_INT(__ON__, __ARG__)                             \
{                                                                       \
    if ((__ON__)) {                                                     \
        __debugit__argument_int(__FILE__, __FUNCTION__,                 \
                __LINE__, #__ARG__, (int) __ARG__);                     \
    }                                                                   \
}

/*
 * This file is automagically generated be each run. It better had exist.
 */
#include "debugit_instrumented_list.h"

#endif /* __DEBUGIT__H__ */
