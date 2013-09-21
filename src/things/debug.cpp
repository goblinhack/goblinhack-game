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

    $Id: thing.cpp 278 2006-11-23 15:36:05Z goblinhack $
*/

#include <ghack.h>
#include <gh_thing.h>

//
// Generate strings corresponding to the GH_THING_REF values.
//
DECLARE_ENUM_STRINGS(gh_thing_ref, GH_THING_REF)

//
// Generate strings corresponding to the GH_THING_DIR values.
//
DECLARE_ENUM_STRINGS(gh_thing_dir, GH_THING_DIR)

//
// Map a string to an index.
//
int gh_thing_ref_string_index (const char *str)
{
    GH_FUNCTION()

    const char *const*iterator;

    GH_FOR_ALL_IN_ARRAY(iterator, gh_thing_ref_strings) {
        if (!strcmp(str, *iterator)) {
            return (iterator - gh_thing_ref_strings);
        }
    }

    return (-1);
}

//
// Map an index to a string.
//
const char * gh_thing_ref_string (const int index)
{
    GH_FUNCTION()

    if (index >= (decltype(index))GH_ARRAY_SIZE(gh_thing_ref_strings)) {
        return (NULL);
    }

    return (gh_thing_ref_strings[index]);
}

void gh_thing::debug (void)
{
    GH_FUNCTION()

    cerr << "name " << name << endl;
    cerr << "  this " << this << " ref " << ref << endl;
    cerr << "  self " << self << endl;
    cerr << "  ref " << ref << endl;

    //
    // Dump all the ref counts
    //
    uint32 bit = 0;
    uint32 ref_bits = ref_bitmap;

    while (ref_bits) {
        if (ref_bits & 1) {
            cerr << "  ref: " << gh_thing_ref_string(bit) << " [" <<
                            refc[bit] << "]" << endl;
        }

        bit++;
        ref_bits >>= 1;
    }

    cerr << name << endl;
    gh_err(this);
}
