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

    $Id: gh_thing_registration.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_THING_REGISTRATION_H_
#define _GH_THING_REGISTRATION_H_

class gh_thing;

class gh_thing_registration;

//
// How to find your own callbacks.
//
extern class gh_thing_registration *gh_thing_registration_get(string name);

//
// The argument block we pass in all callbacks. This makes it easy to add
// new elements without impacting existing functions.
//
class gh_thing_registration_args {
public:
    class gh_thing *initiator;
    class gh_thing *me;

    gh_thing_registration_args(void)
    {
        this->initiator = NULL;
        this->me = NULL;
    }
};

typedef bool (*gh_thing_registration_fn)(gh_thing_registration_args *);

//
// Install/uninstall vectors - per thing .cpp
//
typedef void (*gh_thing_init_fn)(class gh_thing_registration *);
typedef void (*gh_thing_fini_fn)(class gh_thing_registration *);

class gh_thing_registration {
public:
    string name;

    gh_thing_init_fn my_init;
    gh_thing_fini_fn my_fini;
    gh_thing_registration_fn tick;
    gh_thing_registration_fn open;

    void private_init_1(string name);
    void private_init_2(void);
    void private_fini(void);

    gh_thing_registration (string name,
                           gh_thing_init_fn my_init,
                           gh_thing_fini_fn my_fini)
    {
        open = NULL;
        tick = NULL;

        private_init_1(name);

        //
        // Call the per thing init callback defined in the thing .cpp file
        //
        this->my_init = my_init;
        this->my_fini = my_fini;

        (*my_init)(this);

        private_init_2();
    }

    ~gh_thing_registration ()
    {
        private_fini();

        //
        // Call the per thing fini callback defined in the thing .cpp file
        //
        (*my_fini)(this);
    }
};

extern bool is_openable(gh_thing *thing, void *initiator);

#endif /* _GH_THING_REGISTRATION_H_ */
