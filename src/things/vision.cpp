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

    $Id: vision.cpp 1148 2009-01-04 04:14:10Z Ghack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_math.h>
#include <gh_global.h>
#include <gh_level.h>
#include <gh_templates.h>

//
// This is the list of all things that this monster can see or wants
// to explore too. It's a candidate list we filter down to the best.
// It's global and is only meant to be shortlived as a return value.
//
static gh_thing *gh_thing_can_see[4096];

//
// For the number of light rays requested, populate the given array
// with distance information for what this thing can see.
//
// For the monst, we only populate this array for use in debugging.
//
// For the hero, we do it to work out how far the light is cast.
//
void gh_thing::make_light_map (int how_many_rays, float *light_ray_length)
{
    GH_FUNCTION()

    if (is_monst) {
        GH_THING_LOG(this, "look around");
    }

    //
    // Reset to the start of the scratch pad where we'll store our
    // list of things we can see.
    //
    gh_thing **gh_thing_can_see_at = gh_thing_can_see;
    *gh_thing_can_see_at = NULL;

    //
    // How many blocks can we see?
    //
    int how_many_blocks_we_can_see = can_see_distance();
    int how_many_blocks_we_can_see2 = how_many_blocks_we_can_see / 2;
    int how_many_blocks_we_can_seer = how_many_blocks_we_can_see * GH_RES;

    if (is_hero) {
        //
        // Monsters are measured in blocks. Heroes in characters.
        //
        how_many_blocks_we_can_see /= (GH_RES / GH_RES_CHAR);
    }

    if (!how_many_blocks_we_can_see) {
        //
        // Blind.
        //
        return;
    }

    //
    // This is the oldest thing we find (for explorers).
    //
    gh_thing *oldest_thing = NULL;
    uint32 oldest_thing_age = 0;

    cell_max_age++;

    //
    // We use this to skip processing cells more than once in this
    // function. It's also used to tag frames for the hero as visible
    // forever.
    //
    gh_rendered_this_frame++;

    //
    // Set up the center of the light source to jitter in small amounts
    // randomly around the monster/hero location. It makes it look like
    // flickering light.
    //
    int at_x, at_y;

    if (is_hero) {
        at_x = level->map.hero_at.x + 1 - (gh_math::rand100() & 3);
        at_y = level->map.hero_at.y + 1 - (gh_math::rand100() & 3);
    } else {
        //
        // Monsters don't carry torches. Yet.
        //
        at_x = at.x;
        at_y = at.y;
    }

    //
    // Allocate memory for our 'visited cell' memory if we're an explorer.
    //
    if (is_explorer) {
        if (!cells_visited) {
            //
            // First time. Allocate.
            //
            int size = GH_MAX_MAZE_SIZE * GH_MAX_MAZE_SIZE * sizeof(uint32);

            cells_visited = (uint32*)malloc(size);
            if (!cells_visited) {
                //
                // Never happen.
                //
                return;
            }

            memset(cells_visited, 0, size);
        }
    }

    static uint32 gh_rendered_this_vision_frame;
    gh_rendered_this_vision_frame++;

    //
    // How many radians to look around in.
    //
    const float light_ray_angle_step = M_360 / (float)how_many_rays;

    //
    // For each light ray.
    //
    for (int ray = 0; ray < how_many_rays; ray++) {

        const float angle = light_ray_angle_step * ray;
        const float x_cos = gh_math::fcos(angle);
        const float y_sin = gh_math::fsin(angle);

        //
        // Opacity increases per ray till it hits 100. Then we go to
        // the next light ray.
        //
        int opacity = 0;

        //
        // Now stepping as far as we can see, look for opaque things
        // that are in the way of this light ray.
        //
        int step;

        if (is_hero) {
            //
            // The light looks smoother at the edges this way.
            //
            step = GH_RES / 3;
        } else {
            //
            // Bigger chunkier steps. Less accurate. Faster.
            //
            step = GH_RES / 2;
        }

        //
        // For each character in the ray.
        //
        float radius;

        for (radius = 0; 
             radius < how_many_blocks_we_can_seer;
             radius += step) {
redo:
            //
            // For all things at this map location.
            //
            gh_point3d ray_at((int)(at_x + radius * x_cos),
                              (int)(at_y + radius * y_sin), 0);

            vector<gh_thing*> * v = level->map.real_isat(ray_at);

            if (v == NULL) {
                continue;
            }

            vector<gh_thing*>::reverse_iterator it;

            for (it = v->rbegin(); it != v->rend(); it++) {
                gh_thing *t = *it;

                //
                // Avoid processing the same thing again and again just
                // slows stuff down if we've decided to attack/not attack
                // this monster. Just add to the opacity and continue.
                //
                if (is_monst) {
                    if (t->vision_seen == gh_rendered_this_vision_frame) {
                        break;
                    }
                    t->vision_seen = gh_rendered_this_vision_frame;

                    //
                    // For a speed optimization, ignore some floor tiles.
                    //
                    if (t->is_corridor || t->is_floor || t->is_dirt) {
                        static int skip_count;
                        if (++skip_count >= 2) {
                            skip_count = 0;
                            continue;
                        }
                    }

                    if (t->invisible()) {
                        continue;
                    }

                    if (!t->is_transparent) {
                        opacity += t->opacity;
                    }

                    if (opacity > 100) {
                        goto next_ray;
                    }
                } else { // hero
                    //
                    // The hero tags all seen spaces as visible forever,
                    // but they appear shaded out if not visible by the
                    // light in this frame.
                    //
                    t->has_been_seen = true;

                    t->rendered = gh_rendered_this_frame;
                }

                //
                // Ignore anything we can see over that is opaque.
                //
                if (t->is_water) {
                    continue;
                }

                if (is_monst) {
                    if (t->is_hard) {
                        goto next_ray;
                    }

                    //
                    // If not an explorer, skip floor cells for speed.
                    //
                    if (t->is_corridor || t->is_floor || t->is_dirt) {
                        if (!is_explorer) {
                            continue;
                        }
                    }

                    //
                    // Only look at the torso. For speed.
                    //
                    if (t->is_body_part) {
                        continue;
                    }

                    //
                    // Ignore others in the same generator group; for 
                    // performance. Goblins don't see other goblins.
                    //
                    if (group_id && (t->group_id == group_id)) {
                        continue;
                    }

                    //
                    // Don't see our own body.
                    //
                    if (t->id == id) {
                        continue;
                    }

                    if (t->is_corridor || t->is_floor || t->is_dirt) {
                        //
                        // Is this cell worth visiting?
                        //
                        int x = t->at.x / GH_RES;
                        int y = t->at.y / GH_RES;

                        uint32 *cell = 
                            &cells_visited[y * GH_MAX_MAZE_SIZE + x];
                        uint32 age;

                        //
                        // Is this cell close so we can skip searching 
                        // here next time? We use half the distance so 
                        // that the outer fringe of our vision ages more.  
                        // We will select those cells as older and
                        // preferred.
                        //
                        if (radius < how_many_blocks_we_can_see2) {
                            //
                            // Mark this thing as recently seen.
                            //
                            *cell = cell_max_age;
                        } else {
                            //
                            // This is on the fringe of our vision. See 
                            // just how old it is and prefer the oldest 
                            // for selection for exploring.
                            //
                            // Filter out cells that are future. These are 
                            // ones that we found earlier as targets and 
                            // now want to avoid searching again.
                            //
                            if (*cell < cell_max_age) {
                                age = cell_max_age - *cell;

                                //
                                // If we have an older selection, ignore 
                                // this one.
                                //
                                if (age > oldest_thing_age) {
                                    //
                                    // If this cell is not as old as our 
                                    // current target, ignore it.
                                    //
                                    if (!cell_target_age ||
                                        (age > cell_target_age)) {
                                        //
                                        // Oldest cell.
                                        //
                                        oldest_thing_age = age;
                                        oldest_thing = t;
                                    }
                                }
                            }
                        }
                    } else {
                        //
                        // A cell worth closer inspection?
                        //
                        if (possible_to_attack(t)) {
                            if (gh_thing_can_see_at < 
                                    gh_thing_can_see + 
                                    GH_ARRAY_SIZE(gh_thing_can_see)) {

                                *gh_thing_can_see_at++ = t;
                            }
                        }
                    }
                }

                //
                // Can our ray pass right on through?
                //
                if (!t->opacity) {
                    continue;
                }

                //
                // It can for open doors too.
                //
                if (t->is_open) {
                    continue;
                }

                if (t->invisible()) {
                    continue;
                }

                const int frames_sz = t->get_anim_size();
                if (!frames_sz) {
                    //
                    // An invisible thing? Pass right through it.
                    //
                    continue;
                }

                //
                // We have some kind of obstacle.
                //
                
                //
                // See if the light ray intersects this thing's anim frame
                //
                const gh_anim_frame *a = t->get_anim();

                if ((ray_at.x >= t->at.x + a->pix_minv.x) &&
                    (ray_at.x <= t->at.x + a->pix_maxv.x) &&
                    (ray_at.y >= t->at.y + a->pix_minv.y) &&
                    (ray_at.y <= t->at.y + a->pix_maxv.y)) {

                    //
                    // Ok, we know this ray is inside this thing. As such we 
                    // want to increase the opacity. However, we've been 
                    // jumping in large steps to find the first opaque thing.  
                    // Now we have it, step back and do small increments to 
                    // find a more accurate radius
                    //
                    if (is_hero) {
                        if (step == GH_RES / 3) {
                            if (radius >= GH_RES) {
                                step = 3;
                                radius -= GH_RES;
                                goto redo;
                            }

                            if (radius >= GH_RES / 2) {
                                step = 3;
                                radius -= GH_RES / 2;
                                goto redo;
                            }
                        }
                    }

                    if (!t->is_transparent) {
                        opacity += t->opacity;
                    }

                    if (opacity >= 100) {
                        goto next_ray;
                    }
                }
            } // per thing

        } // radius

next_ray:
        light_ray_length[ray] = radius;
    } // per ray

    //
    // If a monster, look at what we found.
    //
    if (is_hero) {
        return;
    }

    if (gh_thing_can_see == gh_thing_can_see_at) {
        //
        // Nothing to see here. Could be blind, so check there's nothing
        // on us.
        //
        if (!how_many_blocks_we_can_see) {
            meet_all_at_new_position();
        }
        return;
    }

    int best = -99999;
    gh_thing *best_attack_thing = NULL;
    gh_thing *best_follow_thing = NULL;

    //
    // For each thing that we can see.
    //
    while (--gh_thing_can_see_at >= gh_thing_can_see) {
        gh_thing *t = *gh_thing_can_see_at;

        int score = want_to_attack(t);

        GH_THING_LOG(this, "  score " + tostring(score) + 
                     "  attack " + t->named());

        if (score >= GH_THING_ANGER_LIMIT) {
            if (score > best) {
                best = score;

                best_attack_thing = t;
                best_follow_thing = NULL;

                GH_THING_LOG(this, "  BEST score " + tostring(score) + 
                             "  attack " + t->named());
            }
        }

        score = want_to_follow(t);

        //
        // If stuck, don't keep following.
        //
        if ((at == target_last_set_where) && (target_last_thing == t)) {
            score = 0;
        }

        if (score >= GH_THING_FOLLOW_LIMIT) {
            if (score > best) {
                best = score;

                best_attack_thing = NULL;
                best_follow_thing = t;

                GH_THING_LOG(this, "  BEST score " + tostring(score) + 
                             "  follow " + t->named());
            }
        }
    }

    if (best_attack_thing) {
        if (gh_global::debug) {
            debug_string = "%%fg=redattack " + best_attack_thing->named();
        }

        GH_THING_LOG(this, " best " + tostring(best) + 
                     "  attack " + best_attack_thing->named());

        found_something_to_chase(best_attack_thing);

        if (scared()) {
            found_something_to_run_from(best_attack_thing);
        } else {
            try_to_attack_victim(best_attack_thing);
        }
        return;
    }

    if (best_follow_thing) {
        if (gh_global::debug) {
            debug_string = "%%fg=yellowfollow " + best_follow_thing->named();
        }

        GH_THING_LOG(this, " best " + tostring(best) + 
                     "  follow " + best_follow_thing->named());

        found_something_to_chase(best_follow_thing);
        return;
    }

    if (oldest_thing) {
        if (gh_global::debug) {
            debug_string = "%%fg=greenwander";
        }

        if (cells_visited) {
            uint32 *cell = &cells_visited[
                        (oldest_thing->at.y / GH_RES) * GH_MAX_MAZE_SIZE +
                        (oldest_thing->at.x / GH_RES)];

            cell_target_age = cell_max_age - *cell;

            //
            // Age the cell so we don't consider it again. This makes
            // it a future cell.
            //
            *cell += gh_math::rand100();

            found_something_to_chase(oldest_thing);
        }
    }
}

//
// For every cell this thing can see, call the callback.
//
void gh_thing::what_can_i_see (void)
{
    GH_FUNCTION()

    make_light_map(GH_LIGHT_RAYS_MONST_MAX, light_ray_length);
}
