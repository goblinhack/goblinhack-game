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

    $Id: map_manip.cpp 1148 2009-01-04 04:14:10Z Ghack $
*/

#include <ghack.h>
#include <gh_map.h>
#include <gh_level.h>

bool gh_map::move_to (gh_thing *t, gh_point3d to)
{
    GH_FUNCTION()

    //
    // If on the map then move the thing.
    // If not on the map then push it on.
    //
    if (t->is_on_map) {
        return (move(t, to - t->at));
    } else {
        t->at = to;

        push(t);
    }

    return (true);
}

bool gh_map::move (gh_thing *t, gh_point3d delta)
{
    GH_FUNCTION()

    vector<gh_thing*> ** vo = thingisatp(t);
    bool got;

    if (!t->is_on_map) {
        /*
         * Failed to move, not on map.
         */
        return (false);
    }

    vector<gh_thing*>::iterator vs;

    //
    // Check something exists there to move!
    //
    if ((vo == NULL) || (*vo == NULL)) {
        gh_out(t);
        GH_WARN("thing " + t->short_name + " couldn't move");
        return (false);
    }

    //
    // Try to move and see if we end up on a different map block.
    //
    t->at += delta;

    vector<gh_thing*> ** vn = thingisatp(t);

    if (vo == vn) {
        //
        // Ok to not undo the move here as we could be moving in
        // fractional bits.
        //
        return (true);
    }

    //
    // Couldn't move to the new position?
    //
    if (vn == NULL) {
        //
        // Undo the move. Is someone off the map?
        //
        t->at -= delta;

        /*
         * For witches, fairies and the like this is normal.
         *
        GH_LOG("thing " + t->short_name + " couldn't move to new pos");
         */
        return (false);
    }

    if (*vn == NULL) {
        *vn = new vector<gh_thing*> ();
    }

    //
    // Move to the new position.
    //
    bool got_one = false;

    for (vs = (*vn)->begin(); vs != (*vn)->end(); vs++) {
        gh_thing *i = *vs;

        if (i->at.y < t->at.y) {
            //
            // insert before i
            //
            (*vn)->insert(vs, t);
            got_one = true;
            break;
        }
    }

    if (!got_one) {
        (*vn)->push_back(t);
    }

    //
    // Remove ourselves from the old position.
    //
    got = false;

    for (vs = (*vo)->begin(); vs != (*vo)->end(); vs++) {
        if (*vs == t) {
            (*vo)->erase(vs);
            got = true;
            break;
        }
    }

    if (!got) {
        gh_out(t);
        GH_THROW("thing " + t->short_name + " failed to move");
    }

    if (!(*vo)->size()) {
        delete *vo;
        *vo = NULL;
    }

    return (true);
}

//
// push a thing onto the level
//
gh_thing *gh_map::push (gh_thing *t)
{
    GH_FUNCTION()

    if (!t->level) {
        //
        // This is messed up.
        //
        t->debug();
        GH_THROW("thing " + t->short_name + " has no level");
        return (NULL);
    }

    if (t->level->being_deleted) {
        //
        // No point in placing items at this point.
        //
        return (NULL);;
    }

    //
    // On the map already?
    //
    if (t->is_on_map) {
        t->debug();
        GH_THROW("thing " + t->short_name + " on the map already");
        return (NULL);
    }

    vector<gh_thing*> ** vn = thingisatp(t);

    if (vn == NULL) {
        /*
         * Usually off the map.
         */
        return (NULL);
    }

    if (*vn == NULL) {
        *vn = new vector<gh_thing*> ();
    }

    vector<gh_thing*>::iterator vs;

    bool got_one = false;

    for (vs = (*vn)->begin(); vs != (*vn)->end(); vs++) {
        gh_thing *i = *vs;

        if (i->at.y < t->at.y) {
            //
            // insert before i
            //
            (*vn)->insert(vs, t);
            got_one = true;
            break;
        }
    }

    if (!got_one) {
        (*vn)->push_back(t);
    }

    t->add_map_things();

    return (t);
}

//
// pop the last thing 'like' t and free it
//
void gh_map::pop (gh_thing *t)
{
    GH_FUNCTION()

    //
    // No longer on the map? No longer in a room.
    //
    // Note that this loses the hook to shops, so use owner.
    //
    t->room = NULL;

    vector<gh_thing*> ** v = thingisatp(t);

    if (v == NULL) {
        cout << t << endl;
        gh_out(t);
        GH_THROW("thing " + t->short_name + " failed to pop1");
        return;
    }

    if (*v == NULL) {
        cout << t << endl;
        gh_out(t);
        GH_THROW("thing " + t->short_name + " failed to pop2");
        return;
    }

    vector<gh_thing*>::iterator vs;
    bool got = false;

    for (vs = (*v)->end()-1; vs >= (*v)->begin(); vs--) {
        gh_thing *j = *vs;
        if (j == t) {
            GH_THING_VERIFY(j);

            j->rm_map_things();
            (*v)->erase(vs);
            got = true;
            break;
        }
    }

    if (!got) {
        cout << t << endl;
        gh_out(t);
        GH_THROW("thing " + t->short_name + " failed to pop3: didn't find the thing");
    }

    if (!(*v)->size()) {
        delete *v;
        *v = NULL;
    }
}

//
// push a copy of t onto the level
//
gh_thing * gh_map::push_copy (gh_thing *t)
{
    vector<gh_thing*> ** v = thingisatp(t);

    if (v == NULL) {
        return (NULL);
    }

    if (*v == NULL) {
        *v = new vector<gh_thing*> ();
    }

    gh_thing *n = new gh_thing(t->level, NULL, gh_point3d (0,0,0), t, NULL);

    (*v)->push_back(n);

    return (n);
}

//
// flood fill until the function callback is true. Just use is_true for
// any thing collision.
//
uint32 gh_map::fill_until (gh_point3d p,
                           gh_thing *t,
                           gh_thing_evaluator_callback fn,
                           void *arg)
{
    uint32 total = 0;
    vector<gh_point3d> points;
    gh_point3d p2;
    size_t at = 0;

    points.reserve(512*512);
    points.push_back(p);

    while (at < points.size()) {
        p = points[at++];

        if (!real_isok(p)) {
            continue;
        }

        if (find_first_thing_same(p, t)) {
            continue;
        }

        if (find_thing(p, fn, arg)) {
            continue;
        }

        total++;

        gh_thing *new_thing = new gh_thing(t->level, t->short_name, p);
        if (!new_thing) {
            continue;
        }

        if (new_thing->is_movable) {
            new_thing->at.x += rand() % (GH_RES / 2);
            new_thing->at.y += rand() % (GH_RES / 2);
            new_thing->at.x -= rand() % (GH_RES / 2);
            new_thing->at.y -= rand() % (GH_RES / 2);
        }

        if (!push(new_thing)) {
            new_thing->decref(GH_THING_REF_FIRST);
            continue;
        }

        new_thing->decref(GH_THING_REF_FIRST);

        //
        // Use the thing width for placement.
        //
        const gh_anim_frame *anim = new_thing->get_anim();

        int dx = (int)anim->pix_max_w;
        int dy = (int)anim->pix_max_h;

        p2 = p + gh_point3d(dx, 0,0);
        if (!find_first_thing_same(p2, t)) {
            points.push_back(p2);
        }

        p2 = p + gh_point3d(-dx,0,0);
        if (!find_first_thing_same(p2, t)) {
            points.push_back(p2);
        }

        p2 = p + gh_point3d(0, dy,0);
        if (!find_first_thing_same(p2, t)) {
            points.push_back(p2);
        }

        p2 = p + gh_point3d(0,-dy,0);
        if (!find_first_thing_same(p2, t)) {
            points.push_back(p2);
        }
    }

    return (total);
}

//
// flood fill until the function callback is true. Just use is_true for
// any thing collision.
//
uint32 gh_map::fill_while (gh_point3d p,
                           gh_thing_evaluator_callback do_this,
                           gh_thing_evaluator_callback fn,
                           void *arg)
{
    uint32 total = 0;
    vector<gh_point3d> points;
    gh_point3d p2;
    size_t at = 0;

    points.reserve(512*512);
    points.push_back(p);

    while (at < points.size()) {
        p = points[at++];

        if (!real_isok(p)) {
            continue;
        }

        if (!find_thing(p, fn, arg)) {
            continue;
        }

        total++;

        vector<gh_thing*> * v = real_isat(p);

        if (v == NULL) {
            continue;
        }

        vector<gh_thing*>::reverse_iterator i;

        for (i = v->rbegin(); i != v->rend(); i++) {
            gh_thing *t = *i;

            (*do_this)(t, arg);
        }

        //
        // Use the thing width for placement.
        //
        int dx = GH_RES;
        int dy = GH_RES;

        p2 = p + gh_point3d(dx, 0,0);
        points.push_back(p2);

        p2 = p + gh_point3d(-dx,0,0);
        points.push_back(p2);

        p2 = p + gh_point3d(0, dy,0);
        points.push_back(p2);

        p2 = p + gh_point3d(0,-dy,0);
        points.push_back(p2);
    }

    return (total);
}
