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

    $Id: string.cpp 276 2006-11-20 21:16:05Z goblinhack $
*/

#include <ghack.h>
#include <gh_sdl.h>
#include <gh_thing.h>
#include <gh_thing_ptr.h>

map<long, class gh_thing_ptr *> gh_thing_ptrs;

class gh_thing_ptr {
public:
    gh_thing_ptr (void) { in_use = false; name = ""; }
    ~gh_thing_ptr () {}

    uint8 in_use:1;
    string name;
    const char *last_ok_file;
    int last_ok_line;
};

void gh_thing_ptr_add (gh_thing *thing)
{
#ifdef GH_PTR_SANITY
    gh_thing_ptr *p = gh_thing_ptrs[(long)thing];

    if (p) {
        //
        // We've seen this pointer before.
        //
        if (p->in_use) {
            //
            // Odd. Should be freed.
            //
            GH_WARN("reallocating unfreed ptr? " + tostring((long)thing));
        }

        //
        // Reusing an old freed pointer ref. Fine.
        //
    } else {
        //
        // A new pointer ref.
        //
        p = gh_thing_ptrs[(long)thing] = new gh_thing_ptr();
    }

    //
    // Record that we've seen this pointer for future verification.
    //
    p->in_use = true;
    p->name = thing->short_name;
    p->last_ok_file = "";
    p->last_ok_line = -1;
#endif
}

bool gh_thing_ptr_verify (gh_thing *thing,
                          const char *file,
                          const int line)
{
#ifdef GH_PTR_SANITY
    gh_thing_ptr *p = (gh_thing_ptr*) gh_thing_ptrs[(long)thing];

    if (p) {
        //
        // We've found it, lets check it isn't freed.
        //
        if (p->in_use) {
            p->last_ok_file = file;
            p->last_ok_line = line;
            return (true);
        }

        //
        // Oops.
        //
        printf("%s LOG [%8p:%-30s]: STALE\n",
               gh_sdl_get_time_string(),
               thing,
               p->name.c_str());

        fflush(stdout);
        fflush(stderr);

        GH_WARN("stale pointer " + tostring((void *)thing) + " at " +
                string(file) + " " + tostring(line) +
                " last ok " + string(p->last_ok_file) + " " +
                tostring(p->last_ok_line));

        return (false);
    }

    //
    // Junk pointer.
    //
    GH_WARN("invalid pointer " + tostring((void *)thing) +
             " " + string(file) + " " + tostring(line));

    fflush(stdout);
    fflush(stderr);

    printf("%s LOG [%8p:%-30s]: BAD POINTER\n",
           gh_sdl_get_time_string(),
           thing,
           p->name.c_str());

    fflush(stdout);
    fflush(stderr);
    return (false);
#else
    return (true);
#endif
}

void gh_thing_ptr_describe (gh_thing *thing,
                            const char *file,
                            const int line)
{
#ifdef GH_PTR_SANITY
    gh_thing_ptr *p = (gh_thing_ptr*) gh_thing_ptrs[(long)thing];

    if (p) {
        //
        // Oops.
        //
        printf("%s LOG [%8p:%-30s]: STALE\n",
               gh_sdl_get_time_string(),
               thing,
               p->name.c_str());

        fflush(stdout);
        fflush(stderr);

        GH_WARN("pointer " + tostring((void *)thing) + " at " +
                string(file) + " " + tostring(line) +
                " last ok " + string(p->last_ok_file) + " " +
                tostring(p->last_ok_line));

        return;
    }

    //
    // Junk pointer.
    //
    GH_WARN("invalid pointer " + tostring((void *)thing) +
             " " + string(file) + " " + tostring(line));

    fflush(stdout);
    fflush(stderr);

    printf("%s LOG [%8p:%-30s]: BAD POINTER\n",
           gh_sdl_get_time_string(),
           thing,
           p->name.c_str());

    fflush(stdout);
    fflush(stderr);
#endif
}

void gh_thing_ptr_free (gh_thing *thing)
{
#ifdef GH_PTR_SANITY
    gh_thing_ptr *p = (gh_thing_ptr*) gh_thing_ptrs[(long)thing];

    if (p) {
        if (p->in_use) {
            //
            // Normal case.
            //
            p->in_use = false;
            return;
        }

        //
        // Oops.
        //
        GH_WARN("stale pointer");
    } else {
        //
        // Oops.
        //
        GH_WARN("invalid pointer");
    }
#endif
}
