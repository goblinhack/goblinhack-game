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

    $Id: spawn.cpp 90 2006-09-10 15:59:22Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_level.h>
#include <gh_templates.h>
#include <gh_subsys.h>
#include <gh_math.h>

void gh_thing_for_each_body_part (gh_thing *t,
                                  for_each_body_part_fn f,
                                  void *arg)
{
    vector <gh_thing *> parts;

    gh_thing *i = t->torso();

    while (i) {
        parts.push_back(i);

        gh_thing *j = i->prev;
        while (j) {
            parts.push_back(j);
            j = j->prev;
        }

        i = i->limb_next;
    }

    vector <gh_thing *>::iterator p;

    for (p = parts.begin(); p != parts.end(); p++) {
        (*f)(*p, arg);
    }
}

//
// Follow the leader thing where it moves
//
void gh_thing::follow (gh_thing *leader)
{
    GH_FUNCTION()

    gh_fpoint p(leader->at.x, leader->at.y);
    gh_fpoint o(at.x, at.y);

    //
    // Move p to the origin
    //
    p -= o;

    //
    // What angle to get from p to the origin?
    //
    float angle = p.anglerot();
    float dist = p.length();
    float dist_delta = (float)follow_distance - dist;

    gh_point3d direction(-(int)(dist_delta * cos(angle)),
                         -(int)(dist_delta * sin(angle)), 0);

    (void) move_delta(direction);
}

//
// is the given thing part of the overall thing? this == the thing's head.
//
bool gh_thing::is_part_of_my_body (gh_thing *thing)
{
    GH_FUNCTION()

    return (thing->id == id);
}

gh_thing *gh_thing::head (void)
{
    GH_FUNCTION()

    gh_thing *t;

    t = this;
    while (t && t->prev) {
        t = t->prev;
    }

    return (t);
}

gh_thing *gh_thing::tail (void)
{
    GH_FUNCTION()

    gh_thing *t;

    t = this;
    while (t && t->next) {
        t = t->next;
    }

    return (t);
}

gh_thing *gh_thing::first_limb (void)
{
    GH_FUNCTION()

    gh_thing *t;

    t = this;
    while (t && t->limb_prev) {
        t = t->limb_prev;
    }

    return (t);
}

gh_thing *gh_thing::last_limb (void)
{
    GH_FUNCTION()

    gh_thing *t;

    t = this;
    while (t && t->limb_next) {
        t = t->limb_next;
    }

    return (t);
}

gh_thing *gh_thing::torso (void)
{
    GH_FUNCTION()

    return (tail()->first_limb());
}

//
// returns false on an error - no followers - no error
//
bool gh_thing::monst_create_followers (void)
{
    GH_FUNCTION()

    if (!followers.size()) {
        return (true);
    }

    vector<gh_thing *> created;

    GH_THING_LOG(this, "create followers");

    //
    // pop ourselves so we can place our followers without collisions.
    //
    incref(GH_THING_REF_FOLLOWER_HEAD);
    level->map.pop(this);

    gh_thing *prev = this;
    gh_point3d where = at;

    //
    // Create each follower. The head is "this". We chain each follower
    // onto the previous. If we fail to place any one follower on the
    // map we undo the whole chain and return false to indicate a request
    // to remove the head.
    //
    for (uint8 i = 0; i < followers.size(); i++) {
        //
        // Create it!
        //
        gh_thing *new_thing = new gh_thing(level, followers[i], where);

        new_thing->id = id;

        GH_THING_LOG(new_thing, " followers");

        //
        // Keep a reference so we can pop/unpop it with impunity.
        // Also so it can live on the created list safely.
        //
        new_thing->incref(GH_THING_REF_TMP_FOLLOWER);

        new_thing->decref(GH_THING_REF_FIRST);

        //
        // Now see where it will fit around the last thing?
        //
        vector<gh_point3d> possible;

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                //
                // Can't spawn on top of thyself!
                //
                if (!i && !j) {
                    continue;
                }

                const gh_anim_frame *anim_prev = prev->get_anim();
                const gh_anim_frame *anim_next = new_thing->get_anim();

                if (!new_thing->follow_distance) {
                    new_thing->follow_distance = (int)
                        ((anim_prev->pix_max_w / 2) +
                         (anim_next->pix_max_w / 2));
                }

                gh_point3d spawn_location;

                if (stay_above_torso) {
                    //
                    // Use -y so that long necks go up
                    //
                    spawn_location = 
                        gh_point3d(0,
                                    -new_thing->follow_distance * j,
                                    0);
                } else {
                    spawn_location = 
                        gh_point3d(new_thing->follow_distance * i,
                                   new_thing->follow_distance * j,
                                   0);
                }

                spawn_location += where;

                new_thing->at = gh_point3d(spawn_location);

                if (!level->map.push(new_thing)) {
                    continue;
                }

                //
                // Check that we can exist in this space on the map.
                //
                if (new_thing->hit_obstacle(true /* ignore_hero */)) {
                    GH_THING_LOG(new_thing, "body part hit obstacle");

                    level->map.pop(new_thing);

                    new_thing->dead("failed to place", false, false);
                    goto failed_to_place_follower;
                }

                if (new_thing->touching_anything()) {
                    GH_THING_LOG(new_thing, "body part hit something");

                    level->map.pop(new_thing);

                    new_thing->dead("failed to place", false, false);
                    goto failed_to_place_follower;
                }

                goto placed_follower;
            }
        }

failed_to_place_follower:

        //
        // Destroy the whole chain of followers.
        //
        GH_THING_LOG(new_thing, "pop whole chain");

        //
        // We couldn't place one of the followers.
        //
        new_thing->decref(GH_THING_REF_TMP_FOLLOWER);
        new_thing = NULL;

        for (;;) {
            vector<gh_thing *>::iterator c = created.begin();

            if (c == created.end()) {
                //
                // put our head back on! The caller can clean up.
                //
                level->map.push(this);
                decref(GH_THING_REF_FOLLOWER_HEAD);

                return (false);
            }

            if ((*c)->is_on_map) {
                level->map.pop(*c);
            }
            (*c)->decref(GH_THING_REF_TMP_FOLLOWER);

            (*c)->dead("failed to place", false, false);

            created.erase(c);
        }

        return (false);

placed_follower:

        GH_THING_LOG(new_thing, "placed follower");

        //
        // We've placed this follower on the chain successfully.
        //
        // Keep the reference until we remove it from this list.
        //
        created.push_back(new_thing);

        //
        // Remove it from the map so we can place the rest of the body
        // without collisions.
        //
        level->map.pop(new_thing);

        //
        // Use this body part as the next in the chain.
        //
        where = new_thing->at;
        prev = new_thing;
    }

    //
    // Push the whole chain of followers onto the map and
    // link them together.
    //
    prev = this;

    for (;;) {
        vector<gh_thing *>::iterator c = created.begin();

        if (c == created.end()) {
            break;
        }

        gh_thing *t = *c;

        level->map.push(t);

        //
        // Undo the original ref now it's safely on the map.
        //
        t->decref(GH_THING_REF_TMP_FOLLOWER);

        //
        // Link the things together.
        //
        t->prev = prev;
        prev->next = t;

        created.erase(c);

        prev = t;
    }

    //
    // put our head back on!
    //
    level->map.push(this);
    decref(GH_THING_REF_FOLLOWER_HEAD);

    //
    // Check the linkage, of things with body parts, is sane.
    //
    gh_thing *n = this;

    while (n) {
        GH_THING_VERIFY(n);

        if (n->next) {
            if (n->next->prev != n) {
                n->debug();
                GH_THING_LOG(n, " n");
                GH_THING_LOG(n->next, " n->next");
                GH_THING_LOG(n->next->prev, " n->next->prev");
                GH_THROW("next != this");
            }
        }

        if (n->prev) {
            if (n->prev->next != n) {
                n->debug();
                GH_THING_LOG(n, " n");
                GH_THING_LOG(n->prev, " n->prev");
                GH_THING_LOG(n->prev->next, " n->prev->next");
                GH_THROW("prev != this");
            }
        }

        n = n->next;
    }

    GH_THING_LOG(this, "created followers");

    if (limbs) {
        GH_THING_LOG(this, " place limbs");

        monst_create_limbs();
    }

    //
    // We've placed all followers on the chain successfully.
    //
    return (true);
}

//
// returns false on an error
//
bool gh_thing::monst_create_limbs (void)
{
    GH_FUNCTION()

    gh_thing *limb_head = head();
    gh_thing *limb_tail = tail();
    gh_thing *orig_limb_tail = tail();
    int l = limbs;
    int limb_no = 1;
    int dir = 1;
    int alt = 0;

    //
    // For each limb, copy the follower chain.
    //
    while (l--) {
        gh_thing *t1 = limb_head;
        gh_thing *t2 = NULL;
        gh_thing *t2_prev = NULL;

        //
        // Walk the old limb's body from the head to the tail.
        //
        while (t1) {
            //
            // Copy each limb element at the same location. Or rotated
            // around the u if needed.
            //
            gh_fpoint delta;

            delta.x = (float)(t1->at.x - orig_limb_tail->at.x);
            delta.y = (float)(t1->at.y - orig_limb_tail->at.y);

            double angle = delta.anglerot();

            angle += (double)(limb_no * dir) * 
                    gh_math::rad((double)orig_limb_tail->missile_spread_angle);

            float length = delta.length();
            delta.x = length * cos(angle);
            delta.y = length * sin(angle);

            if (!length) {
                delta.x = 0;
                delta.y = 0;
            }

            gh_point3d new_at = orig_limb_tail->at;
            new_at.x = (int)((float)new_at.x + delta.x);
            new_at.y = (int)((float)new_at.y + delta.y);

            t2 = new gh_thing(level, t1->short_name, new_at);

            if (!level->map.push(t2)) {
                GH_THROW("no limb push");
            }

            t2->id = id;

            //
            // Link it to its body part.
            //
            t2->prev = t2_prev;
            if (t2_prev) {
                t2_prev->next = t2;
            }
            t2_prev = t2;

            t1 = t1->next;
        }

        //
        // To make the angles sway around the original one for the next 
        // limb.
        //
        if (alt++ == 2) {
            alt = 0;
            limb_no++;
        }

        dir *= -1;

        //
        // Join the tail to the old limb's tail
        //
        t2->limb_prev = limb_tail;
        limb_tail->limb_next = t2;

        limb_tail = t2;

        //
        // Only the first body can move.
        //
        t2->is_unable_to_move = true;

        //
        // And it can't see
        //
        t2->_can_see_distance = 0;

        //
        // And it can't be drawn.
        //
        t2->anim_frames = NULL;

        t2->does_stuff = false;
        t2->is_monst = false;
    }

    //
    // We've placed all followers on the chain successfully.
    //
    return (true);
}

class gh_data
{
public:

    gh_point3d delta;
    gh_thing *torso;

    gh_data (gh_point3d delta, gh_thing * torso) : 
        delta(delta), torso(torso) { }
};

static void gh_thing_move_whole_body_fn (gh_thing *t,
                                         void *data)
{
    gh_data *d = (class gh_data *) data;

    if (d->torso == t) {
        return;
    }

    t->level->map.move(t, d->delta);
}

void gh_thing::move_whole_body (gh_point3d delta)
{
    class gh_data d(delta, this);

    gh_thing_for_each_body_part(this,
                                gh_thing_move_whole_body_fn,
                                (void*) &d);
}

class gh_dead_data
{
public:
    string killed_by;

    gh_dead_data (string killed_by) : killed_by(killed_by) { }
};

static void gh_thing_dead_whole_body_fn (gh_thing *t,
                                         void *data)
{
    gh_dead_data *d = (class gh_dead_data *) data;

    if (t->next) {
        t->next->prev = NULL;
        t->next = NULL;
    }

    if (t->prev) {
        t->prev->next = NULL;
        t->prev = NULL;
    }

    if (t->limb_next) {
        t->limb_next->limb_prev = NULL;
        t->limb_next = NULL;
    }

    if (t->limb_prev) {
        t->limb_prev->limb_next = NULL;
        t->limb_prev = NULL;
    }

    t->dead(d->killed_by, false, false);
}

void gh_thing::dead_whole_body (string killed_by)
{
    class gh_dead_data d(killed_by);

    gh_thing_for_each_body_part(this, 
                                gh_thing_dead_whole_body_fn,
                                (void*) &d);
}
