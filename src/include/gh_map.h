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

    $Id: gh_map.h 519 2007-06-04 17:47:47Z goblinhack $
*/

#ifndef _GH_MAP_H_
#define _GH_MAP_H_

#include <gh_thing.h>

class gh_map : public gh_stream
{
public:
    gh_map(void);
    ~gh_map(void);
    void init(void);
    void deletemap(void);
    void find_map_extremes(void);
    void join(void);
    void add_border(void);
    void fixup_for_shops(void);
    void update(void);

    //
    // map_manip.cpp
    //
    gh_thing *find_thing_on_level(gh_thing_evaluator_callback cb,
                                  void *arg);

    gh_thing *find_thing(const gh_point3d &at,
                         gh_thing_evaluator_callback cb,
                         void *arg);

    gh_thing *find_nearest_thing(gh_point3d at,
                                 gh_thing_evaluator_callback cb,
                                 void *arg);

    gh_thing *find_nearest_teleport_awake_thing(gh_point3d at);

    bool is_on_corridor(gh_point3d at);
    bool is_on_floor(gh_point3d at);
    bool is_on_dirt(gh_point3d at);

    gh_thing *find_furthest_thing(gh_point3d at,
                                  gh_thing_evaluator_callback cb,
                                  void *arg);

    gh_thing *find_furthest_teleport_awake_thing(gh_point3d at);

    list <gh_thing *> find_all_touching_things(gh_thing *t);
    void find_all_touching_things(gh_thing *t,
                                  list <gh_thing *> *l);
    list <gh_thing *> find_all_touching_things_in_adjoining_squares(gh_thing *t);
    void find_all_touching_things_in_adjoining_squares(gh_thing *t,
                                                       list <gh_thing *> *l);


    gh_thing *find_first_thing_same_at(gh_point3d at, gh_thing *thing);
    gh_thing *find_first_thing_same(gh_point3d at, gh_thing *thing);
    gh_thing *find_first_thing_same(gh_thing * t);

    uint32 fill_until(gh_point3d p,
                      gh_thing *t,
                      gh_thing_evaluator_callback fn,
                      void *arg);

    uint32 fill_while(gh_point3d p,
                      gh_thing_evaluator_callback do_this,
                      gh_thing_evaluator_callback fn,
                      void *arg);

    gh_inline bool isok (const gh_point3d p)
    {
        if (p.x < 0) {
            return (false);
        }
        if (p.y < 0) {
            return (false);
        }
        if (p.x >= size.w) {
            return (false);
        }
        if (p.y >= size.h) {
            return (false);
        }
        return (true);
    }

    gh_inline bool real_isok (const gh_point3d p)
    {
        if (p.x < 0) {
            return (false);
        }
        if (p.y < 0) {
            return (false);
        }
        if ((p.x / GH_RES) >= size.w) {
            return (false);
        }
        if ((p.y / GH_RES) >= size.h) {
            return (false);
        }
        return (true);
    }

    gh_inline vector<gh_thing*> * isat (const gh_point3d p)
    {
         if (!isok(p)) {
            return (NULL);
         }

         return (things[(p.z << bhw) + (p.y << bw) + p.x]);
    }

    gh_inline vector<gh_thing*> ** isatp (const gh_point3d p)
    {
         if (!isok(p)) {
            return (NULL);
         }

         return (&things[(p.z << bhw) + (p.y << bw) + p.x]);
    }

    gh_inline vector<gh_thing*> * isatq (const gh_point3d p)
    {
         return (things[(p.z << bhw) + (p.y << bw) + p.x]);
    }

    gh_inline vector<gh_thing*> * isatq (const gh_point3d p) const
    {
         return (things[(p.z << bhw) + (p.y << bw) + p.x]);
    }

    gh_inline vector<gh_thing*> ** isatpq (const gh_point3d p)
    {
         return (&things[(p.z << bhw) + (p.y << bw) + p.x]);
    }

    gh_inline vector<gh_thing*> * thingisat (const gh_thing *t)
    {
        return isat(gh_point3d(t->at.x / GH_RES,
                               t->at.y / GH_RES,
                               t->at.z / GH_RES));
    }

    gh_inline vector<gh_thing*> ** thingisatp (const gh_thing *t)
    {
        return isatp(gh_point3d(t->at.x / GH_RES,
                                t->at.y / GH_RES,
                                t->at.z / GH_RES));
    }

    gh_inline gh_point3d to_real (const gh_point3d p)
    {
        return (gh_point3d(p.x / GH_RES,
                           p.y / GH_RES,
                           p.z / GH_RES));
    }

    gh_inline vector<gh_thing*> * real_isat (const gh_point3d p)
    {
        return (isat(gh_point3d(p.x / GH_RES,
                                p.y / GH_RES,
                                p.z / GH_RES)));
    }

    gh_inline vector<gh_thing*> ** real_isatp (const gh_point3d p)
    {
        return (isatp(gh_point3d(p.x / GH_RES,
                                 p.y / GH_RES,
                                 p.z / GH_RES)));
    }

    bool move_to(gh_thing *t, gh_point3d to);
    bool move(gh_thing *t, gh_point3d delta);
    gh_thing *push(gh_thing *t);
    gh_thing *push_copy(gh_thing *t);
    void pop(gh_thing *t);
    gh_thing * find_next(gh_point3d at, gh_thing *t);

    gh_size3d       size;

    //
    // Bitmaps to speed up map lookups.
    //
    gh_size3d       bsize;
    gh_size3d       bsizemask;

    //
    // y shift to use on p.y to get the offset to the desired y plane
    //
    uint16          bw;

    //
    // z shift to use on p.z to get the offset to the desired z plane
    //
    uint16          bhw;

    //
    // The extremes of the map which may be less than the map size and
    // correspond to the portion of the map that's in use. This can be
    // used to write quicker loops, but depends on find_map_extremes()
    // having been called since any map modifications.
    //
    gh_point3d      tl;
    gh_point3d      br;

    //
    // The resolution we allow characters to move within the map. i.e.
    // divide a block into res parts to see how smooth the monsts move.
    //
    uint8           res;

    //
    // This is the ptr to our x/y/z world map.
    //
    vector<gh_thing*> ** things;

    //
    // The pixel offset for smooth scrolling
    //
    int max_scroll_speed;
    gh_point smooth_scroll;

    //
    // This is a map, one per map resolution unit of what we've
    // already displayed on screen.
    //
#ifdef UNUSED
    vector<uint8> has_been_seen;
#endif

    //
    // A cache of where the hero is so we can stay centered even when
    // the hero is no more! 8(
    //
    gh_point3d hero_map_at;
    gh_point3d hero_at;

    friend void operator>> (gh_stream_in& is, gh_map *me);
    friend void operator<< (gh_stream_out& os, const gh_map *me);
};

class gh_map_char
{   
public:
    gh_map_char(gh_level *l,
                const char map_char,
                const string thing_name,
                const int line,
                const int column);

    char map_char;
    string thing_name;
    int column;
    int line;

    //
    // Cached from the thing template for speed
    //
    int appearing_chance;
    int is_monst;

    uint8 is_found_only_in_shops:1;
    uint8 not_in_shops:1;
};

#endif /* _GH_MAP_H_ */
