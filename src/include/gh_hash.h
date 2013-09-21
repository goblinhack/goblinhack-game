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

    $Id: gh_hash.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_HASH_H_
#define _GH_HASH_H_

#ifdef UNUSED
//
// String comparison functor
//
struct eqstr {
    bool operator()(const string &s1, const string &s2) const
    {
        return (strcmp(s1.c_str(), s2.c_str()) < 0);
    }
};
#endif

#endif /* _GH_HASH_H_ */
