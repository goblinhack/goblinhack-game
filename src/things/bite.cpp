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

    $Id: bite.cpp 1130 2008-12-21 23:27:17Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_thing.h>
#include <gh_math.h>
#include <gh_thing_registration.h>
#include <gh_player.h>

bool gh_thing::met_biter (void)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);

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
                // Can't hit thyself!
                //
                if (t == this) {
                    continue;
                }

                //
                // To allow slime molds to move under heroes
                //
                if (t->is_hero) {
                    continue;
                }

                //
                // Silly...
                //
                if (is_part_of_my_body(t)) {
                    continue;
                }

                if (t->is_destroyable) {
                    if (gh_thing_gfx_can_hit(this, t)) {
                        return (true);
                    }
                }
            }
        }
    }

    return (false);
}

bool gh_thing::met_hero (void)
{
    GH_FUNCTION()

    const size_t res = level->map.res;
    gh_point3d loc(at.x / res, at.y / res, at.z / res);

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
                // Can't hit thyself!
                //
                if (t == this) {
                    continue;
                }

                if (t->is_hero) {
                    if (gh_thing_gfx_can_hit(this, t)) {
                        return (true);
                    }
                }
            }
        }
    }

    return (false);
}

//
// One thing is eating another thing, t
//
void gh_thing::try_to_hit (gh_thing *t)
{
    GH_FUNCTION()

    GH_THING_VERIFY(this);
    GH_THING_VERIFY(t);

    bool killed;

    if (weapon) {
        //
        // If you have a weapon, no biting!
        //
        return;
    }

    gh_thing_registration_args args;

    //
    // Don't let monsters bite too often.
    //
    if (is_monst || is_hero) {
        if (!gh_have_x_tenths_passed_since(bite_delay_in_tenths,
                                           bite_last_when)) {
            return;
        }

        bite_last_when = gh_get_time_milli();
    }

    hit(t, &killed,
        bite_damage,
        bite_armour,
        bite_treasure,
        bite_karma,
        bite_karma_chance,
        bite_intelligence,
        bite_intelligence_chance,
        bite_poison,
        bite_poison_chance,
        bite_stone,
        bite_stone_chance,
        bite_fate,
        bite_fate_chance,
        NULL /* weapon */,
        false /* mand attack */
        );

    if (!killed) {
        return;
    }
}
