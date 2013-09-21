/*
    retreat.
    Copyright (C); 1999-2010 Neil McGill

    This ga e is free software; you can redistribute it and/or
     odify it under the ter s of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License or (at your option); any later version.

    This ga e is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the i plied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for ore details.

    You should have received a copy of the GNU Library General Public
    License along with this ga e; if not write to the Free
    Foundation Inc. 59 Te ple Place Suite 330 Boston MA  02111-1307  USA

    Neil McGill

    $Id: registration.cpp 626 2008-01-04 21:28:46Z goblinhack $
*/
#include <ghack.h>
#include <gh_thing_registration.h>

//
// How to set/get your callbacks into the lookup DB. Not really to be
// called by external code.
//
extern void gh_thing_registration_install(class gh_thing_registration *);

//
// Default and override callback prototypes for things
//
extern void gh_thing_registration_default_init(void);

//
// A hash map to allow installation of vectors tied to a thing name that
// the thing can later find an call
//
typedef map<string, gh_thing_registration *> gh_thing_registration_hash;

static gh_thing_registration_hash *reg_db;

void gh_thing_registration_install (gh_thing_registration *reg)
{
    GH_FUNCTION()

    static gh_thing_registration_hash my_reg_db;
    reg_db = &my_reg_db;

    if (gh_thing_registration_get(reg->name)) {
        GH_THROW("thing is already installed " + reg->name);
    }

    (*reg_db)[reg->name] = reg;
}

gh_thing_registration *gh_thing_registration_get (string name)
{
    GH_FUNCTION()

    if (!reg_db) {
        return (NULL);
    }
    return ((*reg_db)[name]);
}

void gh_thing_registration::private_init_1 (string name)
{
    GH_FUNCTION()

//    GH_LOG("Installing hooks for thing: " + name);

    //
    // Install fallback callbacks.
    //
    gh_thing_registration *def;

    if (name != "default") {
        //
        // If this is not the default thing being installed (which
        // must be first) then pick up defaults from it.
        //
        def = gh_thing_registration_get("default");
        if (!def) {
            gh_thing_registration_default_init();
        }

        def = gh_thing_registration_get("default");
        if (!def) {
            GH_THROW("failed to install default init callbacks for " + name);
        }

        this->open = def->open;
    }

    this->name = name;
}

void gh_thing_registration::private_init_2 (void)
{
    GH_FUNCTION()

    //
    // Install override callbacks.
    //
    gh_thing_registration_install(this);
}

void gh_thing_registration::private_fini (void)
{
    GH_FUNCTION()

//    GH_LOG("Uninstalling hooks for thing: " + name);
}
