/*
 * File that facilitates C++ program debugging.
 *
 * Copyright 1999 by Gray Watson
 *
 * This file is part of the dmalloc package.
 *
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies, and that the name of Gray Watson not be used in advertising
 * or publicity pertaining to distribution of the document or software
 * without specific, written prior permission.
 *
 * Gray Watson makes no representations about the suitability of the
 * software described herein for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * The author may be contacted via http://www.dmalloc.com/
 *
 * $Id: dmalloc.cpp 276 2006-11-20 21:16:05Z goblinhack $
 */

/*
 * This file is used to effectively redirect new to the more familiar
 * malloc and delete to the more familiar free so they can be debugged
 * with the debug malloc library..  They also give the known error
 * behavior, too.
 *
 * Compile and link this in with the C++ program you want to debug.
 *
 * NOTE: I am not a C++ hacker so feedback in the form of other hints
 * and ideas for C++ users would be much appreciated.
 */

#include <iostream>

#ifdef DMALLOC
#include <dmalloc.h>
#else
#include <stdlib.h>
#endif

/*
 * An overload function for the C++ new.
 */
void * operator new(size_t size, const char * file, unsigned int line)
{
    void *n;
#ifdef DMALLOC
    n = _malloc_leap(file, line, size);
#else
    n = malloc(size);
#endif
    return (n);
}

/*
 * An overload function for the C++ new[].
 */
void * operator new[](size_t size, const char * file, unsigned int line)
{
    void *n;
#ifdef DMALLOC
    n = _malloc_leap(file, line, size);
#else
    n = malloc(size);
#endif
    return (n);
}

/*
 * An overload function for the C++ new.
 */
void * operator new(size_t size)
{
    void *n;
    n = malloc(size);
    return (n);
}

/*
 * An overload function for the C++ new[].
 */
void * operator new[](size_t size)
{
    void *n;
    n = malloc(size);
    return (n);
}

/*
 * An overload function for the C++ delete.
 */
void operator delete (void *pnt)
{
#ifdef DMALLOC
    _free_leap("delete", 0, pnt);
#else
    free(pnt);
#endif
}

/*
 * An overload function for the C++ delete[].
 */
void operator delete[] (void *pnt)
{
#ifdef DMALLOC
    _free_leap("delete[]", 0, pnt);
#else
    free(pnt);
#endif
}
