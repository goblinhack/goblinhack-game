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

    $Id: map.cpp 1290 2009-08-12 17:59:17Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_thing.h>
#include <gh_maze.h>

gh_map::gh_map (void)
{
    GH_FUNCTION()

    size = gh_size3d(GH_MAX_MAZE_SIZE, GH_MAX_MAZE_SIZE, 1);
    res = GH_RES;

    max_scroll_speed = 0;

    //
    // The pixel offset for smooth scrolling
    //
    smooth_scroll = gh_point(0,0);

    things = NULL;

    init();

    //
    // We're relying on the level to set the res here.
    //
}

void gh_map::init (void)
{
    GH_FUNCTION()

    //
    // convert size to the next largest power of 2, for speed
    //
    bhw = 0;
    bw = 0;

    for (bsize.w = 1; size.w > bsize.w; bsize.w <<= 1, bhw++, bw++) ;
    for (bsize.h = 1; size.h > bsize.h; bsize.h <<= 1, bhw++) ;
    for (bsize.d = 1; size.d > bsize.d; bsize.d <<= 1) ;

    bsizemask.w = bsize.w - 1;
    bsizemask.h = bsize.h - 1;
    bsizemask.d = bsize.d - 1;

    things = new vector<gh_thing*>* [bsize.w * bsize.h * bsize.d];

#ifdef UNUSED
    //
    // Create the has been seen map, so we can permanently display
    // portions of the map that have been drawn.
    //
    has_been_seen.resize(size.w * size.h);
#endif

    size = bsize;

    //
    // Null out the map.
    //
    gh_point3d p;

    for (p.z=0; p.z<size.d; p.z++) {
        for (p.x=0; p.x<size.w; p.x++) {
            for (p.y=0; p.y<size.h; p.y++) {
                vector<gh_thing*> ** v = isatpq(p);
                *v = NULL;
            }
        }
    }
}

void gh_map::deletemap (void)
{
    GH_FUNCTION()

    if (!things) {
        return;
    }

    gh_point3d p;

    for (p.x=0; p.x<size.w; p.x++) {
        for (p.y=0; p.y<size.h; p.y++) {
            for (p.z=0; p.z<size.d; p.z++) {
redo:
                vector<gh_thing*> ** v = isatpq(p);

                if (v == NULL) {
                    continue;
                }

                if (*v == NULL) {
                    continue;
                }

                vector<gh_thing*>::iterator vs;

                for (vs = (*v)->begin(); vs != (*v)->end(); vs++) {
                    gh_thing *j = *vs;

                    if (!j) {
                        things = NULL;
                        GH_THROW(" null thing on the map but not the level!");
                    }

                    //
                    // Can happen if we fail to create a thing when trying to
                    // push it onto the level.
                    //
                    if (!j->level) {
                        things = NULL;
                        GH_THROW(j->name + " is on the map but not the level!");
                    }

                    GH_THING_VERIFY(j);

                    pop(j);

                    goto redo;
                }

                delete *v;
                *v = NULL;
            }
        }
    }

    delete [] things;

    things = NULL;
}

gh_map::~gh_map (void)
{
    GH_FUNCTION()

    deletemap();
}

void gh_map::find_map_extremes (void)
{
    GH_FUNCTION()

    gh_point3d p(0,0,0);

    tl = gh_point3d(GH_MAX_MAZE_SIZE, GH_MAX_MAZE_SIZE, 0);
    br = gh_point3d(0, 0, 0);

    for (p.z=0; p.z<size.d; p.z++) {
        for (p.x=0; p.x<size.w; p.x++) {
            for (p.y=0; p.y<size.h; p.y++) {
                vector<gh_thing*> ** v = isatpq(p);

                if (*v == NULL) {
                    continue;
                }

                tl.x = GH_MIN(tl.x, p.x);
                tl.y = GH_MIN(tl.y, p.y);
                br.x = GH_MAX(br.x, p.x);
                br.y = GH_MAX(br.y, p.y);
            }
        }
    }
}

//
// If a thing is joinable, look for neighbouring things of the same
// type to join with.
//
void gh_map::join (void)
{
    GH_FUNCTION()

    gh_point3d at;

    for (at.z=0; at.z<size.d; at.z++) {
        for (at.x=tl.x; at.x<=br.x; at.x++) {
            for (at.y=tl.y; at.y<=br.y; at.y++) {

                gh_point3d p = at;
                
                p *= res;

                vector<gh_thing*> * v = real_isat(p);

                if (v == NULL) {
                    continue;
                }

                vector<gh_thing*>::reverse_iterator i;

                for (i = v->rbegin(); i != v->rend(); i++) {
                    gh_thing *t = *i;

                    if (!t->is_joinable) {
                        continue;
                    }

                    int total;

                    if (find_first_thing_same(
                        p + gh_point3d(0,-res,0), t)) {
                        total = 1;
                    } else {
                        total = 0;
                    }

                    if (find_first_thing_same(
                        p + gh_point3d(-res,0,0), t)) {
                        total += 2;
                    }

                    if (find_first_thing_same(
                        p + gh_point3d(res,0,0), t)) {
                        total += 4;
                    }

                    if (find_first_thing_same(
                        p + gh_point3d(0,res,0), t)) {
                        total += 8;
                    }

#define CASE(a,b,n,c,d) case (a+(b*2)+(c*4)+(d*8)):

                    switch (total) {
                        CASE(  0,
                             0,1,0,
                               0  )
                            t->join = GH_THING_DIR_NODE;
                        break;
                        CASE(  0,
                             1,1,1,
                               0  )
                            t->join = GH_THING_DIR_LR;
                        break;
                        CASE(  0,
                             1,1,0,
                               0  )
                            t->join = GH_THING_DIR_R;
                        break;
                        CASE(  0,
                             0,1,1,
                               0  )
                            t->join = GH_THING_DIR_L;
                        break;
                        CASE(  1,
                             0,1,0,
                               1  )
                            t->join = GH_THING_DIR_UD;
                        break;
                        CASE(  0,
                             0,1,0,
                               1  )
                            t->join = GH_THING_DIR_U;
                        break;
                        CASE(  1,
                             0,1,0,
                               0  )
                            t->join = GH_THING_DIR_D;
                        break;
                        CASE(  1,
                             0,1,1,
                               0  )
                            t->join = GH_THING_DIR_L0;
                        break;
                        CASE(  0,
                             0,1,1,
                               1  )
                            t->join = GH_THING_DIR_L90;
                        break;
                        CASE(  0,
                             1,1,0,
                               1  )
                            t->join = GH_THING_DIR_L180;
                        break;
                        CASE(  1,
                             1,1,0,
                               0  )
                            t->join = GH_THING_DIR_L270;
                        break;
                        CASE(  0,
                             1,1,1,
                               1  )
                            t->join = GH_THING_DIR_T0;
                        break;
                        CASE(  1,
                             1,1,0,
                               1  )
                            t->join = GH_THING_DIR_T90;
                        break;
                        CASE(  1,
                             1,1,1,
                               0  )
                            t->join = GH_THING_DIR_T180;
                        break;
                        CASE(  1,
                             0,1,1,
                               1  )
                            t->join = GH_THING_DIR_T270;
                        break;
                        CASE(  1,
                             1,1,1,
                               1  )
                            t->join = GH_THING_DIR_X;
                        break;
                    }

                    t->get_dir_and_mode(&t->dir, &t->mode);
                }
            }
        }
    }
}

//
// Add the maze border - used for level editing on fresh levels
//
void gh_map::add_border (void)
{
    GH_FUNCTION()

    gh_point3d at;
    int w = size.w / 4;
    int h = size.h / 4;

    for (at.x=0; at.x<w; at.x++) {
        gh_point3d p(at.x, 0, 0);
        
        p *= res;

        gh_thing *new_thing = new gh_thing(NULL, "room/border", p);

        if (new_thing) {
            push(new_thing);
            new_thing->decref(GH_THING_REF_FIRST);
        }
    }

    for (at.x=0; at.x<w; at.x++) {
        gh_point3d p(at.x, h - 1, 0);
        
        p *= res;

        gh_thing *new_thing = new gh_thing(NULL, "room/border", p);

        if (new_thing) {
            push(new_thing);
            new_thing->decref(GH_THING_REF_FIRST);
        }
    }

    for (at.y=0; at.y<h; at.y++) {
        gh_point3d p(0, at.y, 0);
        
        p *= res;

        gh_thing *new_thing = new gh_thing(NULL, "room/border", p);

        if (new_thing) {
            push(new_thing);
            new_thing->decref(GH_THING_REF_FIRST);
        }
    }

    for (at.y=0; at.y<h; at.y++) {
        gh_point3d p(w - 1, at.y, 0);
        
        p *= res;

        gh_thing *new_thing = new gh_thing(NULL, "room/border", p);

        if (new_thing) {
            push(new_thing);
            new_thing->decref(GH_THING_REF_FIRST);
        }
    }
}

void gh_map::update (void)
{
    GH_FUNCTION()

    find_map_extremes();
    join();
    fixup_for_shops();
}

gh_map_char::gh_map_char (gh_level *l,
                          const char map_char,
                          const string thing_name,
                          const int line,
                          const int column)
{
    GH_FUNCTION()

    this->map_char = map_char;
    this->thing_name = thing_name;
    this->column = column;
    this->line = line;

    static bool error_on_unknown_thing = true;

    gh_thing *tmp = gh_thing::find(thing_name, error_on_unknown_thing);
    if (!tmp) {
        GH_THROW("unknown thing " + thing_name);
    }

    //
    // Make sure we only place things in places they like if required.
    //
    is_found_only_in_shops = tmp->is_found_only_in_shops;
    not_in_shops = tmp->not_in_shops;
    is_monst = tmp->is_monst;
    appearing_chance = tmp->appearing_chance_on_level(l);
}

void operator>> (gh_stream_in& is, gh_map *me)
{
    GH_FUNCTION()

    me->get(is, &me->max_scroll_speed, "max_scroll_speed");
    me->get_class(is, &me->smooth_scroll, "smooth_scroll");
    me->get_class(is, &me->hero_at, "hero_at");
    me->get_class(is, &me->hero_map_at, "hero_map_at");
    me->get_class(is, &me->tl, "tl");
    me->get_class(is, &me->br, "br");
    me->get_class(is, &me->size, "size");
    me->get_class(is, &me->bsize, "bsize");
    me->get_class(is, &me->bsizemask, "bsizemask");
    me->get(is, &me->bw, "bw");
    me->get(is, &me->bhw, "bhw");
    me->get(is, &me->res, "res");
#ifdef UNUSED
    me->get_container(is, &me->has_been_seen, "has_been_seen");
#endif
}

void operator<< (gh_stream_out& os, const gh_map *me)
{
    GH_FUNCTION()

    me->put(os, &me->max_scroll_speed, "max_scroll_speed");
    me->put_class(os, &me->smooth_scroll, "smooth_scroll");
    me->put_class(os, &me->hero_at, "hero_at");
    me->put_class(os, &me->hero_map_at, "hero_map_at");
    me->put_class(os, &me->tl, "tl");
    me->put_class(os, &me->br, "br");
    me->put_class(os, &me->size, "size");
    me->put_class(os, &me->bsize, "bsize");
    me->put_class(os, &me->bsizemask, "bsizemask");
    me->put(os, &me->bw, "bw");
    me->put(os, &me->bhw, "bhw");
    me->put(os, &me->res, "res");
#ifdef UNUSED
    me->put_container(os, &me->has_been_seen, "has_been_seen");
#endif
}
