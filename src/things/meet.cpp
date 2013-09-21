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

    $Id: meet.cpp 1145 2008-12-26 14:53:37Z Ghack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_player.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_room.h>
#include <gh_game.h>
#include <gh_math.h>

//
// Handle interactions between 2 things.
//
gh_inline void gh_thing::meet (gh_thing *meet_me)
{
    GH_FUNCTION()

    //
    // Members of the same generator group don't interact.
    //
    if (group_id && (meet_me->group_id == group_id)) {
        return;
    }

    //
    // Body parts don't do much
    //
    if (next && prev) {
        return;
    }

    //
    // Have we  bravely entered a new room?
    //
    if (meet_me->is_floor || meet_me->is_dirt || meet_me->is_corridor) {
        enter_room(meet_me);
        if (is_dead) {
            return;
        }
    }

    //
    // Shortcut floor tiles - they don't do a lot.
    //
    if (meet_me->is_floor || 
        meet_me->is_corridor || 
        meet_me->is_dirt) {
        return;
    }

    if (is_floor || 
        is_corridor || 
        is_dirt) {
        return;
    }

    //
    // No interactons whilst editing.
    //
    if (level->is_editing) {
        if (!gh_global::debug) {
            return;
        }
    }

    //
    // Own missiles bouncing off each other?
    //
    if (missile_thrower && (missile_thrower == meet_me->missile_thrower)) {
        return;
    }

    //
    // If I'm dead, I'm not going to interact much!
    //
    if (is_dead) {
        return;
    }

    //
    // If the other thing is dead, its not going to interact much!
    //
    if (meet_me->is_dead) {
        return;
    }

    //
    // If the target is a telport, can it teleport us?
    //
    if (meet_me->is_teleport) {
        if (teleport(meet_me)) {
            return;
        }

        if (is_dead) {
            return;
        }
    }

    //
    // Have we bumped into or stepped on something?
    //
    if (is_hero) {
        //
        // The return exit?
        //
        if (meet_me->is_return_exit) {
            hero_meet_return_exit(meet_me);
            return;
        }

        //
        // The exit?
        //
        if (meet_me->is_exit) {
            hero_meet_exit(meet_me);
            return;
        }
    }

    if (meet_me->is_dead) {
        return;
    }

    //
    // Have we hit some treasure or food we can eat?
    //
    if ((is_treasure_eater && meet_me->is_treasure) ||
        (is_edible_eater && meet_me->is_edible)) {

        bool can_touch = gh_thing_gfx_can_touch(this, meet_me);

        if (can_touch) {
            collect(meet_me);

            if (is_dead) {
                return;
            }
        }
    }

    if (!is_missile && !is_weapon) {
        //
        // Missiles and weapons always bite.
        //
        if (!gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                           bite_last_when)) {
            return;
        }
    }

    //
    // A missile trying to hit something?
    //
    if (is_missile) {
        //
        // If this missile has hit an obstacle, make it bounce. This is most
        // useful for potions and effects like that so that they don't get
        // wasted.
        //
        if (is_bouncy && meet_me->is_hard) {
            //
            // Rebounds can hit the initiator.
            //
            if (!is_gas) {
                friend_id = -1;
            }

            //
            // Try two alternate paths and see if we are still hitting the
            // same obstacle. Whichever path is not hitting this obstacle is
            // the one we take. This makes it look like we bounced off of it.
            //
            gh_fpoint3d fbounce_a = missile_at;
            gh_fpoint3d fbounce_b = missile_at;
            gh_point3d orig = at;
            gh_point3d bounce_a;
            gh_point3d bounce_b;

            //
            // Allow the missile to move further along each of the bounce
            // paths to see if either return path will be clear.
            //
            fbounce_a.x -= 10 * gh_math::fcos(missile_angle);
            fbounce_a.y += 10 * gh_math::fsin(missile_angle);

            fbounce_b.x += 10 * gh_math::fcos(missile_angle);
            fbounce_b.y -= 10 * gh_math::fsin(missile_angle);

            bounce_a.x = (int)fbounce_a.x;
            bounce_a.y = (int)fbounce_a.y;
            bounce_a.z = (int)fbounce_a.z;

            bounce_b.x = (int)fbounce_b.x;
            bounce_b.y = (int)fbounce_b.y;
            bounce_b.z = (int)fbounce_b.z;

            level->map.move_to(this, bounce_a);

            if (!gh_thing_gfx_can_hit(this, meet_me)) {
                double new_missile_angle;

                if (missile_angle >= M_270) {
                    new_missile_angle = M_180 + (M_360 - missile_angle);
                } else if (missile_angle >= M_180) {
                    new_missile_angle = M_180 - missile_angle;
                } else if (missile_angle >= M_90) {
                    new_missile_angle = M_360 + (M_180 - missile_angle);
                } else {
                    new_missile_angle = M_180 - missile_angle;
                }

                level->map.move_to(this, orig);

                missile_angle = new_missile_angle;

                //
                // Don't 'meet' this target.
                //
                return;
            }

            level->map.move_to(this, orig);

            level->map.move_to(this, bounce_b);

            if (!gh_thing_gfx_can_hit(this, meet_me)) {
                double new_missile_angle;

                if (missile_angle >= M_270) {
                    new_missile_angle = M_90 + (M_270 - missile_angle);
                } else if (missile_angle >= M_180) {
                    new_missile_angle = M_90 - missile_angle;
                } else if (missile_angle >= M_90) {
                    new_missile_angle = M_270 + (M_90 - missile_angle);
                } else {
                    new_missile_angle = M_270 + missile_angle;
                }

                level->map.move_to(this, orig);

                missile_angle = new_missile_angle;

                //
                // Don't 'meet' this target.
                //
                return;
            }

            level->map.move_to(this, orig);

            return;
        }
    }

    //
    // Can we do anything with this encounter?
    //
    bool can_hit = gh_thing_gfx_can_hit(this, meet_me);

    if (!can_hit) {
        return;
    }

    //
    // Attack.
    //
    if (is_hero) {
        if (!wants_to_hit) {
            //
            // Hero's only attack when we hit space (if they have a weapon).
            //
            return;
        }

        wants_to_hit = false;
    }

    try_to_hit(meet_me);
}

void gh_thing::meet_all_at_new_position (void)
{
    GH_FUNCTION()

    GH_THING_VERIFY(this);

    //
    // Save where we are on this level prior to moving. We use this for
    // the hero to move back to this location on returning to a level.
    //
    if (is_hero) {
        level->hero_end_where = at - direction;
    }

    //
    // No interactons whilst editing.
    //
    if (level->is_editing) {
        if (!gh_global::debug) {
            return;
        }
    }

    //
    // Keep the weapon moving along with the monster.
    //
    if (weapon) {
        GH_THING_VERIFY(weapon);

        weapon->move_to(at);

        if (is_dead) {
            return;
        }
    }

    const size_t res = level->map.res;
    gh_point3d orig_at = at;

    //
    // Optimize the squares we look for collisions based on how off center
    // we are.
    //
    int sx, ex, sy, ey;

    if ((at.x % GH_RES) < GH_RES / 2) {
        sx = -1;
        ex = 0;
    } else {
        sx = 0;
        ex = 1;
    }

    if ((at.y % GH_RES) < GH_RES / 2) {
        sy = -1;
        ey = 0;
    } else {
        sy = 0;
        ey = 1;
    }

    for (int i = sx; i <= ex; i++) {
        for (int j = sy; j <= ey; j++) {
            vector <gh_thing*> **vp =
                level->map.real_isatp(at + gh_point3d(i*res, j*res, 0));

            if (vp == NULL) {
                //
                // Collision if we're off the map
                //
                continue;
            }

            //
            // Recheck that there are still things here on the map.
            //
            vector <gh_thing*> *v = *vp;

            if (v == NULL) {
                continue;
            }

            //
            // Copy all the things we will meet to avoid reorganization
            // of the map list.
            //
            static gh_thing *walk_list[100];
            gh_thing **ws = walk_list;
            gh_thing **we = walk_list;
            gh_thing **wend = walk_list + GH_ARRAY_SIZE(walk_list);

            vector<gh_thing*>::iterator i;
            for (i = v->begin(); (i != v->end()) && (we < wend); i++) {
                *(we++) = *i;
            }

            //
            // Now do the meet on this cached list.
            //
            while (ws != we) {
                meet(*(ws++));
            }
        }
    }
}
