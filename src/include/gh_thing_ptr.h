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

    $Id: gh_io.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_THING_PTR_H_
#define _GH_THING_PTR_H_

void gh_thing_ptr_add(class gh_thing *thing);
bool gh_thing_ptr_verify(class gh_thing *thing,
                         const char *file,
                         const int line);
void gh_thing_ptr_describe(class gh_thing *thing,
                           const char *file,
                           const int line);
void gh_thing_ptr_free(class gh_thing *thing);

#ifdef GH_PTR_SANITY
#define GH_THING_VERIFY(thing) gh_thing_ptr_verify(thing, __FILE__, __LINE__)
#define GH_THING_DESCRIBE(thing) gh_thing_ptr_describe(thing, __FILE__, __LINE__)
#else
#define GH_THING_VERIFY(thing) 
#define GH_THING_DESCRIBE(thing) 
#endif

#endif /* _GH_THING_PTR_H_ */
