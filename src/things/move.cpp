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

    $Id: move.cpp 1284 2009-08-10 21:38:39Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_room.h>
#include <gh_math.h>

//
// Returns true on a collision with a thing.
//
bool gh_thing::hit_obstacle (bool ignore_hero)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
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
                // Can't hit thyself!
                //
                if (t == this) {
                    continue;
                }

                //
                // Can't bump into the floor unless really drunk.
                //
                if (t->is_floor || t->is_dirt || t->is_corridor) {
                    continue;
                }

                //
                // Hit thyself?
                //
                if (t->id == id) {
                    continue;
                }

                const gh_anim_frame *anim = get_anim();
                if (!anim) {
                    continue;
                }

                const gh_anim_frame *anim_t = t->get_anim();
                if (!anim_t) {
                    continue;
                }

                if (GH_DISTANCE(at.x, at.y, t->at.x, t->at.y) >
                    max((anim->pix_max_w + anim_t->pix_max_w)/2,
                        (anim->pix_max_h + anim_t->pix_max_h)/2)) {
                    continue;
                }

                //
                // Make sure placement of treasure can't be in obstacles.
                //
                if (is_treasure || is_edible ||
                    //
                    // Make sure if scared of fire you wont step on it.
                    //
                    (hates_fire && t->is_fire) ||
                    (hates_acid && t->is_acid) ||
                    (hates_cold && t->is_cold) ||
                    (hates_suffocation && t->is_suffocation) ||
                    (hates_water && t->is_water)) {

                    if (gh_thing_gfx_can_touch(this, t)) {
                        return (true);
                    }

                    continue;
                }

                //
                // Make sure shopkeepers stay in their shops.
                //
                if (is_shopkeeper) {
                    if (!t->room || !t->room->is_shop) {
                        return (true);
                    }
                }

                //
                // A weapon tring to hit something?
                //
                if (is_weapon) {
                    //
                    // Is this something we want to ignore?
                    //
                    if (!t->is_destroyable) {
                        continue;
                    }

                    //
                    // Bit sick to allow hitting dead bodies!
                    //
                    if (t->is_dead) {
                        continue;
                    }

                    //
                    // If it's something this weapon hits then we need
                    // to let it pass through so it can "bite"
                    //
                    if (!possible_to_attack(t)) {
                        continue;
                    }

                    //
                    // Don't allow oneself to shoot oneself in the flipper.
                    //
                    if (weapon_wielder == t) {
                        continue;
                    }

                    if (gh_thing_gfx_can_touch(this, t)) {
                        return (true);
                    }

                    continue;
                }

                //
                // If the target is a monster
                //
                if (t->is_monst) {
                    //
                    // Some things like, moving walls, don't care if they
                    // bump into something they bite.
                    //
                    if (never_slows_down) {
                        //
                        // Except if they smack into each other at high speed!
                        //
                        if (!t->never_slows_down) {
                            continue;
                        }

                        //
                        // Has it hit something?
                        //
                        if (gh_thing_gfx_can_touch(this, t)) {
                            return (true);
                        }
                    }

                    //
                    // And we're a hero
                    //
                    if (is_hero) {
                        //
                        // Heros can walk through any monster.
                        //
                        continue;
                    }

                    //
                    // Monsters only allow through things they attack
                    //
                    if (!possible_to_attack(t)) {
                        continue;
                    }

                    if (gh_thing_gfx_can_touch(this, t)) {
                        if (is_missile) {
                            if (t == missile_thrower) {
                                //
                                // Don't allow oneself to shoot oneself in
                                // the flipper.
                                //
                            } else {
                                //
                                // But can hit someone else!
                                //
                                return (true);
                            }
                        } else if (is_weapon) {
                            if (t == weapon_wielder) {
                                //
                                // Don't allow oneself to hit oneself in the
                                // flipper.
                                //
                            } else {
                                //
                                // But can hit someone else!
                                //
                                return (true);
                            }
                        } else {
                            //
                            // Something is hitting a monster!
                            //
                            return (true);
                        }
                    }

                    continue;
                }

                //
                // If the target is a hero?
                //
                if (t->is_hero && ignore_hero) {
                    //
                    // Allow spawning onto a hero.
                    //
                } else {
                    //
                    // This stops monsters spawning into walls
                    //
                    if (gh_thing_gfx_is_obstacle(this, t)) {
                        return (true);
                    }
                }
            }

        }
    }

    return (false);
}

//
// Returns true on a touching a thing!
//
bool gh_thing::touching_anything (void)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
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

                if (t->is_floor || t->is_dirt || t->is_corridor) {
                    continue;
                }

                //
                // Hit thyself?
                //
                if (t->id == id) {
                    continue;
                }

                //
                // Can't hit thyself!
                //
                if (t == this) {
                    continue;
                }

                if (gh_thing_gfx_can_touch(this, t)) {
                    return (true);
                }
            }

        }
    }

    return (false);
}

//
// Returns true on a touching part of your own body. Used for hydra head
// collision.
//
bool gh_thing::touching_self (void)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
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
                // Can't hit thyself!
                //
                if (t == this) {
                    continue;
                }

                if (t->is_floor || t->is_dirt || t->is_corridor) {
                    continue;
                }

                //
                // Hit thyself?
                //
                if (t->id != id) {
                    continue;
                }

                if (gh_thing_gfx_can_touch(this, t)) {
                    return (true);
                }
            }
        }
    }

    return (false);
}

bool gh_thing::move_delta (gh_point3d delta)
{
    GH_FUNCTION()

    //
    // Poor thing is stuck?
    //
    if (is_unable_to_move) {
        meet_all_at_new_position();
        return (false);
    }

    //
    // Stop heads wandering off of hydras.
    //
    if (next && is_chained_body_part) {
        gh_point3d proposed = at + delta;

        if (GH_DISTANCE(proposed.x, proposed.y, next->at.x, next->at.y) >
            next->follow_distance * 2) {
            return (false);
        }

        //
        // Keep hydra heads above their bodies.
        //
        if (!prev) {
            gh_thing *tor = torso();

            if (stay_above_torso) {
                if (at.y > tor->at.y) {
                    delta.y = -3;
                }

                if (gh_math::rand100() < 70) {
                    delta.y = 0;
                }
            }

            // 
            // If we have a torso, make sure the heads don't touch each other.
            //
            if (tor) {
                if (gh_math::rand100() < 50) {
                    if (touching_self()) {
                        return (false);
                    }
                }
            }
        }
    }

    if (!level->map.move(this, delta)) {
        if (is_monst) {
            monst_choose_new_dir();
        }

        return (false);
    }

    //
    // If part of a chain i.e. a snake body, ignore collisions. This is
    // allows snake bodies to follow smoothly. However, make sure that
    // hydra torsos check for collisions. So basically heads and torsos
    // not the wibbly stuff in between.
    //
    if (!prev || limb_next || limb_prev) {
        if (hit_obstacle(true /* ignore_hero */)) {
            is_chasing = false;

            level->map.move(this, gh_point3d(0,0,0) - delta);

            meet_all_at_new_position();

            if (is_monst) {
                monst_choose_new_dir();
            }

            return (false);
        }
    }

    direction = delta;
    update_anim();

    //
    // If this is a multi linmbed creature, move all the wiggly bits.
    //
    if (limb_next) {
        move_whole_body(delta);
    }

    meet_all_at_new_position();

    return (true);
}

bool gh_thing::move_to (const gh_point3d to)
{
    GH_FUNCTION()

    GH_THING_VERIFY(this);

    if (is_unable_to_move) {
        is_chasing = false;

        return (false);
    }

    level->map.move_to(this, to);

    meet_all_at_new_position();
    if (is_dead) {
        return (false);
    }

    return (true);
}

bool gh_thing::try_to_move_in_chosen_direction (bool try_other)
{
    GH_FUNCTION()

    if (is_dead) {
        return (false);
    }

    if (is_unable_to_move) {
        is_chasing = false;
        return (false);
    }

    if (direction_wanted == gh_point3d(0,0,0)) {
        if (!speed) {
            return (true);
        }
    }

    if (sleeping()) {
        return (false);
    }

    if (is_missile) {
        float dx = (float)metabolism() * gh_math::fcos(missile_angle);
        float dy = (float)metabolism() * gh_math::fsin(missile_angle);

        missile_at.x += 16.0 / 100.0 * dx;
        missile_at.y += 16.0 / 100.0 * dy;

        direction_last = direction;
        direction.x = (int)dx;
        direction.y = (int)dy;
        direction_wanted = direction;

        missile_angle += missile_angle_delta;

        if (!move_to(gh_point3d((uint32)missile_at.x,
                                (uint32)missile_at.y, 0))) {
            //
            // some bug with collisions
            //
            dead("couldn't move", false /* explode */);

            return (true);
        }

        if (is_dead) {
            return (false);
        }

        return (true);
    }

    if (is_dead) {
        return (false);
    }

    //
    // Weapons move with their onwers.
    //
    if (is_weapon) {
        return (false);
    }

    //
    // Can we move the way the thing intended
    //
    if (direction_wanted != gh_point3d(0,0,0)) {
        if (move_delta(direction_wanted)) {
            //
            // If we can keep going in the same dir then pick up some speed.
            //
            if (speed < metabolism() / 5) {
                speed++;
            }

            return (true);
        }

        if (is_dead) {
            return (false);
        }
    }

    if (!try_other) {
        return (false);
    }

    if (is_dead) {
        return (false);
    }

    //
    // Keep moving the last way we managed.
    //
    if (move_delta(direction)) {
        //
        // If we can keep going in the same dir then pick up some speed.
        //
        if (speed < metabolism() / 5) {
            speed++;
        }

        return (true);
    }

    if (is_dead) {
        return (false);
    }

    if (!never_slows_down) {
        //
        // Slow down if we can`t move the direction we want.
        //
        speed--;
        if (!speed) {
            speed = 1;
        }
    }

    return (false);
}

gh_point3d gh_thing::direction_left (void)
{
    GH_FUNCTION()

    return (gh_point3d(-speed,0,0));
}

gh_point3d gh_thing::direction_right (void)
{
    GH_FUNCTION()

    return (gh_point3d(speed,0,0));
}

gh_point3d gh_thing::direction_up (void)
{
    GH_FUNCTION()

    return (gh_point3d(0,-speed,0));
}

gh_point3d gh_thing::direction_down (void)
{
    GH_FUNCTION()

    return (gh_point3d(0,speed,0));
}

bool gh_thing::direction_is_left (void)
{
    GH_FUNCTION()

    return ((direction.x < 0) && (direction.y == 0));
}

bool gh_thing::direction_is_right (void)
{
    GH_FUNCTION()

    return ((direction.x > 0) && (direction.y == 0));
}

bool gh_thing::direction_is_up (void)
{
    GH_FUNCTION()

    return ((direction.x == 0) && (direction.y < 0));
}

bool gh_thing::direction_is_down (void)
{
    GH_FUNCTION()

    return ((direction.x == 0) && (direction.y > 0));
}

bool gh_thing::try_to_move_left (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(-speed,0,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_right (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(speed,0,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_up (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(0,-speed,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_down (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(0,speed,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_tl (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(-speed,-speed,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_tr (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(speed,-speed,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_bl (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(-speed,speed,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

bool gh_thing::try_to_move_br (void)
{
    GH_FUNCTION()

    direction_wanted = gh_point3d(speed,speed,0);
    return (try_to_move_in_chosen_direction(false /* try other */));
}

void gh_thing::monst_move (void)
{
    GH_FUNCTION()

    if (is_dead) {
        return;
    }

    if (sleeping()) {
        return;
    }

    if (is_chasing) {
        float distance_to_target =
            GH_DISTANCE(at.x, at.y, target.x, target.y);

        //
        // Have we roughly met our target. Slow down as we approach it
        // so we really hit it.
        //
        if (distance_to_target < GH_RES / 2) {
            speed /= 2;
        }

        if (distance_to_target < GH_RES / 4) {
            //
            // Find a new target.
            //
            is_chasing = false;
            target = gh_point3d(0,0,0);
            cell_target_age = 0;
            return;
        }

        direction = gh_point3d(0, 0, 0);

        //
        // Use this so we adjust our path to track the target.
        //
        if (target.x < at.x) {
            direction.x = -speed;
        }

        if (target.x > at.x) {
            direction.x = speed;
        }

        if (target.y < at.y) {
            direction.y = -speed;
        }

        if (target.y > at.y) {
            direction.y = speed;
        }

        //
        // Try a direct line to the target. If that fails, try sidling
        // up to it.
        //
        if (move_delta(direction)) {
            if (is_dead) {
                return;
            }

            if (speed < metabolism() / 5) {
                speed++;
            }

            //
            // Check we're not moving further from the target. If so,
            // look for another target. We could have an obstacle we
            // need a path algo to get past, whcih we don't have yet.
            //
            if (GH_DISTANCE(at.x, at.y, target.x, target.y) >
                distance_to_target) {

                monst_look_around();
            }

            return;
        }

        if (direction.x && direction.y) {
            int tmp = direction.y;
            direction.y = 0;

            if (direction.x) {
                if (move_delta(direction)) {
                    if (is_dead) {
                        return;
                    }

                    //
                    // We would have turned off the chasing flag due to the
                    // first move_delta fail. Turn it back on. We're good to
                    // keep chasing!
                    //
                    is_chasing = true;

                    if (speed < metabolism() / 5) {
                        speed++;
                    }

                    //
                    // If we've moved further away, maybe look for a
                    // better target?
                    //
                    if (GH_DISTANCE(at.x, at.y, target.x, target.y) >
                        distance_to_target) {

                        monst_look_around();
                    }

                    return;
                }
            }

            direction.y = tmp;
            tmp = direction.x;
            direction.x = 0;

            if (direction.y) {
                if (move_delta(direction)) {
                    if (is_dead) {
                        return;
                    }

                    //
                    // We would have turned off the chasing flag due to the
                    // first move_delta fail. Turn it back on. We're good to
                    // keep chasing!
                    //
                    is_chasing = true;

                    if (speed < metabolism() / 5) {
                        speed++;
                    }

                    //
                    // If we've moved further away, maybe look for a
                    // better target?
                    //
                    if (GH_DISTANCE(at.x, at.y, target.x, target.y) >
                        distance_to_target) {

                        monst_look_around();
                    }

                    return;
                }
            }

            direction.x = tmp;
        }

        //
        // Can't move. Choose a new target. Future idea is to improve this
        // with a maze search algo.
        //
        is_chasing = false;
        target = gh_point3d(0,0,0);
        cell_target_age = 0;
        monst_look_around();
    }

    if (try_to_move_in_chosen_direction(true /* try other */)) {
        return;
    }

    if (is_dead) {
        return;
    }

    int tries = 0;

    while (tries++ < 10) {
        monst_choose_new_dir();

        if (try_to_move_in_chosen_direction(true /* try other */)) {
            return;
        }

        if (is_dead) {
            return;
        }
    }
}

void gh_thing::monst_choose_new_dir (void)
{
    GH_FUNCTION()

    int r;
    gh_point3d old = direction_wanted;
    gh_point3d double_back;

    if (!speed) {
        speed = 1;
    }

    double_back.x = -old.x;
    double_back.y = -old.y;
    double_back.z = 0;

    int tries = 0;
    while (tries++ < 10) {
        //
        // 76 looks odd, but it compensates for the naffness of the
        // random number generator! 8(
        //
        r = gh_math::rand100();

        if (r > 76) {
            direction_wanted.x = -speed;
        } else if (r > 46) {
            direction_wanted.x = speed;
        } else if (r > 20) {
            direction_wanted.x = old.x;
        } else {
            direction_wanted.x = 0;
        }

        r = gh_math::rand100();

        if (r > 76) {
            direction_wanted.y = -speed;
        } else if (r > 46) {
            direction_wanted.y = speed;
        } else if (r > 20) {
            direction_wanted.y = old.y;
        } else {
            direction_wanted.y = 0;
        }

        if (!direction_wanted.x && !direction_wanted.y) {
            continue;
        }

        //
        // Some objects just bob up and down or left and right.
        //
        if (moves_left_right && moves_up_down) {
            //
            // This one is more complex and just moves in straight lines.
            //
            if (direction_wanted.x && direction_wanted.y) {
                //
                // Just pick one direction, up-down or left-right.
                //
                if (gh_math::rand100() > 50) {
                    direction_wanted.x = 0;
                } else {
                    direction_wanted.y = 0;
                }
            }
        } else if (moves_left_right) {
            //
            // Restrict the movement to the plane we want.
            //
            direction_wanted.y = 0;
        } else if (moves_up_down) {
            //
            // Restrict the movement to the plane we want.
            //
            direction_wanted.x = 0;
        }

        if (speed) {
            if ((direction_wanted.x == 0) && (direction_wanted.y == 0)) {
                direction_wanted = old;
            }
        }

        //
        // Doubling back is a bit lame and boring.
        //
        if (double_back.x || double_back.y) {
            //
            // Things that just bob in two directions always double back.
            //
            if ((moves_left_right && !moves_up_down) ||
                (!moves_left_right && moves_up_down)) {
                return;
            }

            //
            // Only choose the new dir if it isn't a double back.
            //
            if (direction_wanted != double_back) {
                return;
            }
        } else {
            //
            // Must be a new direction from 0,0.
            //
            return;
        }
    }
}
