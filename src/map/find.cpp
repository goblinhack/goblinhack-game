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

    $Id: find.cpp 1251 2009-07-28 13:06:28Z Ghack $
*/

#include <ghack.h>
#include <gh_templates.h>
#include <gh_map.h>

gh_thing *gh_map::find_thing_on_level (gh_thing_evaluator_callback cb,
                                       void *arg)
{
    gh_point3d p(0,0,0);

    for (p.x=0; p.x<size.w; p.x++) {
        for (p.y=0; p.y<size.h; p.y++) {

            gh_thing *t = find_thing(gh_point3d(p.x * res, p.y * res, 0),
                                     cb, arg);

            if (t != NULL) {
                return (t);
            }
        }
    }

    return (NULL);
}

gh_thing *gh_map::find_nearest_thing (gh_point3d at,
                                      gh_thing_evaluator_callback cb,
                                      void *arg)
{
    gh_point3d p(0,0,0);
    gh_thing *t;

    for (int r = 1; r < size.w; r++) {
        p.y = -r;
        for (p.x = -r; p.x < r; p.x++) {
            t = find_thing(at - gh_point3d(p.x * res, p.y * res,0), cb, arg);
            if (t) {
                return (t);
            }
        }

        p.y = r;
        for (p.x = -r; p.x < r; p.x++) {
            t = find_thing(at - gh_point3d(p.x * res, p.y * res,0), cb, arg);
            if (t) {
                return (t);
            }
        }

        p.x = -r;
        for (p.y = -r; p.y < r; p.y++) {
            t = find_thing(at - gh_point3d(p.x * res, p.y * res,0), cb, arg);
            if (t) {
                return (t);
            }
        }

        p.x = r;
        for (p.y = -r; p.y < r; p.y++) {
            t = find_thing(at - gh_point3d(p.x * res, p.y * res,0), cb, arg);
            if (t) {
                return (t);
            }
        }
    }

    return (NULL);
}

list <gh_thing *> gh_map::find_all_touching_things (gh_thing *t)
{
    list <gh_thing *> l;

    find_all_touching_things(t, &l);

    return (l);
}

void gh_map::find_all_touching_things (gh_thing *t,
                                       list <gh_thing *> *l)
{
    const gh_anim_frame *anim = t->get_anim();
    const int32 w = (int)anim->pix_max_w;
    const int32 h = (int)anim->pix_max_h;

    gh_thing *other;

    other = gh_map::find_first_thing_same(gh_point3d(t->at.x - w,
                                                     t->at.y,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things(other, l);
        }
    }
        
    other = gh_map::find_first_thing_same(gh_point3d(t->at.x + w,
                                                     t->at.y,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things(other, l);
        }
    }
        
    other = gh_map::find_first_thing_same(gh_point3d(t->at.x,
                                                     t->at.y - h,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things(other, l);
        }
    }
        
    other = gh_map::find_first_thing_same(gh_point3d(t->at.x,
                                                     t->at.y + h,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things(other, l);
        }
    }
}

list <gh_thing *> gh_map::find_all_touching_things_in_adjoining_squares (gh_thing *t)
{
    list <gh_thing *> l;

    find_all_touching_things_in_adjoining_squares(t, &l);

    return (l);
}

void gh_map::find_all_touching_things_in_adjoining_squares (gh_thing *t,
                                                            list <gh_thing *> *l)
{
    const int32 w = GH_RES;
    const int32 h = GH_RES;

    gh_thing *other;

    other = gh_map::find_first_thing_same(gh_point3d(t->at.x - w,
                                                     t->at.y,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things_in_adjoining_squares(other, l);
        }
    }
        
    other = gh_map::find_first_thing_same(gh_point3d(t->at.x + w,
                                                     t->at.y,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things_in_adjoining_squares(other, l);
        }
    }
        
    other = gh_map::find_first_thing_same(gh_point3d(t->at.x,
                                                     t->at.y - h,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things_in_adjoining_squares(other, l);
        }
    }
        
    other = gh_map::find_first_thing_same(gh_point3d(t->at.x,
                                                     t->at.y + h,
                                                     t->at.z),
                                          t);
    if (other && (other != t)) {
        if (!contains(l, other)) {
            l->push_back(other);

            gh_map::find_all_touching_things_in_adjoining_squares(other, l);
        }
    }
}
gh_thing *gh_map::find_furthest_thing (gh_point3d at,
                                       gh_thing_evaluator_callback cb,
                                       void *arg)
{
    gh_point3d p(0,0,0);
    gh_thing *t;

    for (int radius = size.w / 2; radius > 0; radius--) {

        for (int dr = -radius; dr < radius; dr++) {

            p.x = at.x + dr * res;
            p.y = at.y + radius * res;

            t = find_thing(p, cb, arg);
            if (t) {
                return (t);
            }

            p.x = at.x + dr * res;
            p.y = at.y - radius * res;

            t = find_thing(p, cb, arg);
            if (t) {
                return (t);
            }

            p.x = at.x + radius * res;
            p.y = at.y + dr * res;

            t = find_thing(p, cb, arg);
            if (t) {
                return (t);
            }

            p.x = at.x - radius * res;
            p.y = at.y + dr * res;

            t = find_thing(p, cb, arg);
            if (t) {
                return (t);
            }
        }
    }

    return (NULL);
}

gh_thing *gh_map::find_nearest_teleport_awake_thing (gh_point3d at)
{
    GH_FUNCTION()

    return (find_nearest_thing(at, &is_teleport_awake, 0));
}

gh_thing *gh_map::find_furthest_teleport_awake_thing (gh_point3d at)
{
    GH_FUNCTION()

    return (find_furthest_thing(at, &is_teleport_awake, 0));
}

bool gh_map::is_on_corridor (gh_point3d at)
{
    GH_FUNCTION()

    return (find_thing(at, &is_corridor, 0) != NULL);
}

bool gh_map::is_on_floor (gh_point3d at)
{
    GH_FUNCTION()

    return (find_thing(at, &is_floor, 0) != NULL);
}

bool gh_map::is_on_dirt (gh_point3d at)
{
    GH_FUNCTION()

    return (find_thing(at, &is_dirt, 0) != NULL);
}

gh_thing *gh_map::find_thing (const gh_point3d &at,
                              gh_thing_evaluator_callback cb,
                              void *arg)
{
    vector<gh_thing*> * v = real_isat(at);

    if (v == NULL) {
        return (NULL);
    }

    vector<gh_thing*>::reverse_iterator i;

    for (i = v->rbegin(); i != v->rend(); i++) {
        gh_thing * t = *i;

        if ((*cb)(t, arg)) {
            return (t);
        }
    }

    return (NULL);
}

gh_thing *gh_map::find_first_thing_same_at (gh_point3d at, gh_thing *t)
{
    GH_FUNCTION()

    vector<gh_thing*> * v = real_isat(at);

    if (v == NULL) {
        return (NULL);
    }

    vector<gh_thing*>::reverse_iterator i;

    for (i = v->rbegin(); i != v->rend(); i++) {
        gh_thing * a_thing = *i;

        if (!t) {
            return (a_thing);
        }

        //
        // Skip me
        //
        if (a_thing == t) {
            continue;
        }

        //
        // Can't compare 't' as it might not be on the map.
        //
        if ((t->name == a_thing->name) && (a_thing->at == at)) {
            return (a_thing);
        }
    }

    return (NULL);
}

gh_thing *gh_map::find_first_thing_same (gh_point3d at, gh_thing *t)
{
    GH_FUNCTION()

    vector<gh_thing*> * v = real_isat(at);

    if (v == NULL) {
        return (NULL);
    }

    vector<gh_thing*>::reverse_iterator i;

    for (i = v->rbegin(); i != v->rend(); i++) {
        gh_thing * a_thing = *i;

        if (!t) {
            return (a_thing);
        }

        //
        // Skip me
        //
        if (a_thing == t) {
            continue;
        }

        if (t->name == a_thing->name) {
            return (a_thing);
        }
    }

    return (NULL);
}

gh_thing *gh_map::find_first_thing_same (gh_thing * f)
{
    GH_FUNCTION()

    if (f == NULL) {
        return (NULL);
    }

    return find_first_thing_same(f->at, f);
}

//
// get the next thing after the given thing
//
gh_thing * gh_map::find_next (gh_point3d at, gh_thing *t)
{
    GH_FUNCTION()

    vector<gh_thing*> * v = isat(at);

    if (v == NULL) {
        return (NULL);
    }

    vector<gh_thing*>::iterator i;

    if (t == NULL) {
        i = v->begin();

        return (*i);
    }

    for (i = v->begin(); i != v->end(); i++) {
        if (*i != t) {
            continue;
        }

        i++;

        if (i != v->end()) {
            return (*i);
        } else {
            return (NULL);
        }
    }

    return (NULL);
}
