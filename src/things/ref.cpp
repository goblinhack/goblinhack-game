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
#include <gh_level.h>

gh_thing::hash_thing gh_thing::things;
list <gh_thing*> leaked_things;

bool gh_thing::isref (gh_thing_ref r)
{
    GH_FUNCTION()

    return (ref_bitmap & (1 << r));
}

void gh_thing::incref (gh_thing_ref r)
{
    GH_FUNCTION()

    if (gh_global::trace_mode) {
        GH_THING_LOG(this, "incref(" + string(gh_thing_ref_string(r)) + ")");
    }

    switch (r) {
    case GH_THING_REF_MISSILE_THROWER:
    case GH_THING_REF_OWNER:
    case GH_THING_REF_TERMINAL:
        //
        // allow multiple refs.
        //
        break;

    default:
        //
        // allow one ref only.
        //
        if (ref_bitmap & (1 << r)) {
            GH_THING_LOG(this, "incref(" + string(gh_thing_ref_string(r)) + ")");
            debug();
            GH_THROW("thing " + name + " ref already exists " + string(gh_thing_ref_string(r)));
        }
    }

    ref_bitmap |= (1 << r);
    refc[r]++;
    ref++;
}

void gh_thing::decref (gh_thing_ref r)
{
    GH_FUNCTION()

    if (gh_global::trace_mode) {
        GH_THING_LOG(this, "decref(" + string(gh_thing_ref_string(r)) + ")");
    }

    if (!(ref_bitmap & (1 << r))) {
        GH_THING_LOG(this, "decref(" + string(gh_thing_ref_string(r)) + ")");
        debug();
        GH_ERR("thing " + name + " ref does not exist " + string(gh_thing_ref_string(r)));
        return;
    }

    if (!refc[r]) {
        GH_THING_LOG(this, "decref(" + string(gh_thing_ref_string(r)) + ")");
        debug();
        GH_THROW("thing " + name + " has no ref to remove " + string(gh_thing_ref_string(r)));
    }

    refc[r]--;

    //
    // Last ref?
    //
    if (!refc[r]) {
        ref_bitmap &= ~(1 << r);
    }

    if (!ref) {
        GH_THING_LOG(this, "decref(" + string(gh_thing_ref_string(r)) + ")");
        debug();
        GH_THROW("thing " + name + " has no ref2 to remove " + string(gh_thing_ref_string(r)));
    }

    --ref;

    /*
     * Last reference?
     */
    if (!ref) {
        delete this;
        return;
    }

    if ((all_things_it_valid) && (ref == 1)) {
        //
        // This is the last ref and is a ref from the thing to itself.
        // We need to undo this or the thing will never be removed from
        // the list.
        //
        rm_all_things();
        return;
    }
}

void gh_thing::add_leaked_things (void)
{
    GH_FUNCTION()

    if (!leaked_things_it_valid) {
        leaked_things.push_back(this);
        list<class gh_thing *>::iterator i = leaked_things.end();
        leaked_things_it = --i;
        leaked_things_it_valid = true;

        //
        // Important not to used incref/decref here else we'll
        // never try to delete this object!
        //
    }
}

bool gh_thing::rm_leaked_things (void)
{
    GH_FUNCTION()

    if (leaked_things_it_valid) {
        leaked_things.erase(leaked_things_it);
        leaked_things_it_valid = false;

        //
        // Important not to used incref/decref here else we'll
        // never try to delete this object!
        //
        return (true);
    }

    return (false);
}

void gh_thing::add_all_things (void)
{
    GH_FUNCTION()

    if ((level != NULL) && (!all_things_it_valid)) {
        level->all_things.push_back(this);
        list<class gh_thing *>::iterator i = level->all_things.end();
        all_things_it = --i;
        all_things_it_valid = true;
        incref(GH_THING_REF_ALL_THINGS);
    }
}

bool gh_thing::rm_all_things (void)
{
    GH_FUNCTION()

    if ((level == NULL) && (all_things_it_valid)) {
        debug();
        GH_THROW("lost level; can't remove from all things");
    }

    if ((level != NULL) && (all_things_it_valid)) {
        level->all_things.erase(all_things_it);
        all_things_it_valid = false;
        decref(GH_THING_REF_ALL_THINGS);

        //
        // Thing may be invalid after here
        //
        return (true);
    }

    return (false);
}

void gh_thing::add_map_things (void)
{
    GH_FUNCTION()

    if ((level != NULL) && (!is_on_map)) {
        level->map_things.push_back(this);
        list<class gh_thing *>::iterator i = level->map_things.end();
        map_things_it = --i;
        is_on_map = true;
        incref(GH_THING_REF_MAP);
    }
}

bool gh_thing::rm_map_things (void)
{
    GH_FUNCTION()

    if ((level == NULL) && (is_on_map)) {
        debug();
        GH_THROW("lost level; can't remove from map things");
    }

    if ((level != NULL) && (is_on_map)) {
        level->map_things.erase(map_things_it);
        is_on_map = false;

        //
        // Thing may be invalid here
        //
        decref(GH_THING_REF_MAP);

        return (true);
    }

    return (false);
}

void gh_thing::add_does_stuff_things (void)
{
    GH_FUNCTION()

    if ((level != NULL) && (!does_stuff_things_it_valid)) {
        level->does_stuff_things.push_back(this);
        list<class gh_thing *>::iterator i = level->does_stuff_things.end();
        does_stuff_things_it = --i;
        does_stuff_things_it_valid = true;
        incref(GH_THING_REF_DOES_STUFF);
    }
}

bool gh_thing::rm_does_stuff_things (void)
{
    GH_FUNCTION()

    if ((level == NULL) && (does_stuff_things_it_valid)) {
        debug();
        GH_THROW("lost level; can't remove from does stuff things");
    }

    if ((level != NULL) && (does_stuff_things_it_valid)) {
        level->does_stuff_things.erase(does_stuff_things_it);
        does_stuff_things_it_valid = false;

        //
        // Thing may be invalid after here
        //
        decref(GH_THING_REF_DOES_STUFF);

        return (true);
    }

    return (false);
}

void gh_thing::add_dead_things (void)
{
    GH_FUNCTION()

    if ((level != NULL) && (!dead_things_it_valid)) {
        level->dead_things.push_back(this);
        list<class gh_thing *>::iterator i = level->dead_things.end();
        dead_things_it = --i;
        dead_things_it_valid = true;
        incref(GH_THING_REF_DEAD);
    }
}

bool gh_thing::rm_dead_things (void)
{
    GH_FUNCTION()

    if ((level == NULL) && (dead_things_it_valid)) {
        debug();
        GH_THROW("lost level; can't remove from dead things");
    }

    if ((level != NULL) && (dead_things_it_valid)) {
        level->dead_things.erase(dead_things_it);
        dead_things_it_valid = false;

        //
        // Thing may be invalid after here
        //
        decref(GH_THING_REF_DEAD);

        return (true);
    }

    return (false);
}

//
// This list should be empty at game end.
//
void gh_thing::find_leaks (void)
{
    GH_FUNCTION()

    list <gh_thing*>::iterator i= leaked_things.begin();
    gh_thing *t;

    while (i != leaked_things.end()) {
        t = *i;

        GH_THING_VERIFY(t);

        GH_THING_LOG(t, "leaked");

        GH_WARN("thing " + t->named() + " leaked");

        GH_THING_DESCRIBE(t);

        //
        // The templates are cleaned up but we don't tell all the
        // things. So at the end of the game this can crash if we
        // have  leak. Just ignore it.
        //
        t->template_thing = NULL;

        t->debug();

        i++;
    }
}
