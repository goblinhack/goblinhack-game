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

    $Id: find.cpp 626 2008-01-04 21:28:46Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>

#define TRY(A,a,b,c,d,e,f,h,i)                          \
    if (dir == A) {                             \
        things = get_action_things_in_direction(a, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(b, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(c, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(d, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(e, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(f, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(g, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
        things = get_action_things_in_direction(h, fn); \
        if (things.size()) {                            \
            return (things);                            \
        }                                               \
    }
//
// Look around, focusing on the direction we want to move, looking for
// things that satisfy the callback function e.g. looking for things
// that can be opened. The list returned is a sorted list of the things
// the player is most likely looking/pointing at. If nothing is directly
// being pointed at then we look in a circle, starting from the wanted
// direction.
//
vector <gh_thing *>
gh_thing::get_action_things (gh_thing_evaluator_callback fn)
{
    GH_FUNCTION()

    vector <gh_thing *> things;

    gh_point3d dir(0,0,0);

    if (direction_last.x < 0) {
        dir.x = -1;
    } else if (direction_last.x > 0) {
        dir.x = 1;
    }

    if (direction_last.y < 0) {
        dir.y = -1;
    } else if (direction_last.y > 0) {
        dir.y = 1;
    }

    int step = speed;
    if (!step) {
        step = 1;
    }

    for (int dist = step; dist < step * 20; dist += step) {

        const gh_point3d A(-1,-1,0);
        const gh_point3d B( 0,-1,0);
        const gh_point3d C( 1,-1,0);
        const gh_point3d D(-1, 0,0);
        const gh_point3d E( 0, 0,0);
        const gh_point3d F( 1, 0,0);
        const gh_point3d G(-1, 1,0);
        const gh_point3d H( 0, 1,0);
        const gh_point3d I( 1, 1,0);

        const gh_point3d a(-dist,-dist,0);
        const gh_point3d b( 0,-dist,0);
        const gh_point3d c( dist,-dist,0);
        const gh_point3d d(-dist, 0,0);
        const gh_point3d e( 0, 0,0);
        const gh_point3d f( dist, 0,0);
        const gh_point3d g(-dist, dist,0);
        const gh_point3d h( 0, dist,0);
        const gh_point3d i( dist, dist,0);


        //
        // abc
        // d f
        // ghi
        //
        TRY(A,a,b,d,c,g,f,h,i)
        TRY(B,b,a,c,d,f,g,i,h)
        TRY(C,c,b,f,a,i,d,h,g)
        TRY(D,d,a,h,b,h,c,i,f)
        TRY(F,f,c,i,b,h,a,g,d)
        TRY(G,g,d,h,a,i,b,f,c)
        TRY(H,h,g,i,d,f,a,c,b)
        TRY(I,i,h,f,g,c,d,b,a)
    }

    return (things);
}

vector <gh_thing *>
gh_thing::get_action_things_in_direction (gh_point3d delta,
                                          gh_thing_evaluator_callback fn)
{
    GH_FUNCTION()

    vector <gh_thing *> things;

    level->map.move(this, delta);
    things = get_overlapping_things(fn);
    level->map.move(this, gh_point3d(0,0,0) - delta);

    return (things);
}

static gh_thing *arg;

static bool compare_things (const gh_thing *a, const gh_thing *b)
{
    GH_FUNCTION()

    if (GH_DISTANCE(a->at.x, a->at.y, arg->at.x, arg->at.y) <
        GH_DISTANCE(b->at.x, b->at.y, arg->at.x, arg->at.y)) {
        return (true);
    }

    return (false);
}

vector <gh_thing *>
gh_thing::get_overlapping_things (gh_thing_evaluator_callback fn)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);
    vector <gh_thing *> r;

    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            vector <gh_thing*> **vp =
                level->map.real_isatp(at + gh_point3d(i*res, j*res, 0));

            if (vp == NULL) {
                //
                // Collision if we're off the map
                //
                continue;
            }

            vector <gh_thing*> *v = *vp;

            if (v == NULL) {
                continue;
            }

            vector<gh_thing*>::iterator things;

            for (things = v->begin(); things != v->end(); things++) {
                gh_thing *t = *things;

                //
                // Don't overlap with thyself!
                //
                if (t == this) {
                    continue;
                }

                if (gh_thing_gfx_can_touch(this, t)) {
                    if ((*fn)(t, this)) {
                        r.push_back(t);
                    }
                }
            }

        }
    }

    arg = this;

    sort(r.begin(), r.end(), compare_things);

    return (r);
}
