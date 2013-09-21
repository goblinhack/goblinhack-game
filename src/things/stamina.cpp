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

    $Id: tick.cpp 495 2007-03-23 02:56:00Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_game.h>

void gh_thing::stamina_decrease (void)
{
    GH_FUNCTION()

    if (!is_hero) {
        return;
    }

    if (!stamina()) {
        speed = 0;
        return;
    }

    _stamina--;

    if (stamina() < orig_stamina / 10) {
        speed /= 10;

        if (is_hero) {
            level->game->new_message(
                 "%%fg=redYou are exhausted!%%fg=green");
        }
    } else if (stamina() < orig_stamina / 6) {
        speed /= 6;

        if (is_hero) {
            level->game->new_message(
                 "%%fg=redYou are drained!%%fg=green");
        }
    } else if (stamina() < orig_stamina / 2) {
        speed /= 2;

        if (is_hero) {
            level->game->new_message(
                 "%%fg=redYou are getting tired!%%fg=green");
        }
    } else {
        //
        // We're still fit.
        //
    }

    if (health < orig_health / 10) {
        speed /= 10;

        if (is_hero) {
            level->game->new_message(
                 "%%fg=redYou are critically injured!%%fg=green");
        }
    } else if (health < orig_health / 6) {
        speed /= 6;

        if (is_hero) {
            level->game->new_message(
                 "%%fg=redYou are seriously injured!%%fg=green");
        }
    } else if (health < orig_health / 2) {
        speed /= 2;

        if (is_hero) {
            level->game->new_message(
                 "%%fg=redYou are injured!%%fg=green");
        }
    } else {
        //
        // We're still fit.
        //
    }
}

void gh_thing::stamina_tick (void)
{
    GH_FUNCTION()

    bool working_hard = false;

    if ((metabolism() - (speed * 12)) < 5) {
        //
        // Slow down, you're running too fast.
        //
        if (!tireless) {
            working_hard = true;
            stamina_decrease();

            if (is_dead) {
                return;
            }
        }
    }

    //
    // Grr!
    //
    if (raging()) {
        if (!tireless) {
            working_hard = true;
            stamina_decrease();
            stamina_decrease();
            stamina_decrease();
            stamina_decrease();
            stamina_decrease();
            stamina_decrease();
            stamina_decrease();
        }
    }

    if (weapon && weapon->weapon_spin) {
        if (!tireless_swing) {
            //
            // Swinging that sword makes you tired.
            //
            working_hard = true;
            stamina_decrease();

            if (is_dead) {
                return;
            }
        }
    }

    if (!working_hard) {
        //
        // Recover when resting.
        //
        if (_stamina < orig_stamina) {
            _stamina++;

            if (_stamina < orig_stamina) {
                _stamina++;
            }

            if (_stamina < orig_stamina) {
                _stamina++;
            }

            if (can_regenerate1()) {
                if (_stamina < orig_stamina) {
                    _stamina++;
                }
            }

            if (can_regenerate2()) {
                if (_stamina < orig_stamina) {
                    _stamina++;
                }
            }

            if (can_regenerate3()) {
                if (_stamina < orig_stamina) {
                    _stamina++;
                }
            }

            if (tireless_swing) {
                if (_stamina < orig_stamina) {
                    _stamina++;
                }
            }

            if (tireless) {
                if (_stamina < orig_stamina) {
                    _stamina++;
                }
            }
        }

        //
        // Regenerate if we can
        //
        if (health < orig_health) {
            if (can_regenerate1()) {
                health++;
                flash(GH_DARK_GRAY);
            }

            if (can_regenerate2()) {
                health++;
                flash(GH_DARK_GRAY);
            }

            if (can_regenerate3()) {
                health++;
                flash(GH_DARK_GRAY);
            }
        }
    }

    //
    // If raging, regenerate health, but not stamina
    //
    if (raging()) {
        if (health < orig_health) {
            health++;
        }
    }
}

//
// Give the thing a boost of "stamina", that fades away
//
void gh_thing::stamina_boost_start (int boost)
{
    GH_FUNCTION()

    if (!boost) {
        return;
    }

    stamina_boost += boost;
}

//
// If a boost is in operation, gradually ease off of its effects.
//
void gh_thing::stamina_boost_tick (void)
{
    GH_FUNCTION()

    if (stamina_boost > 0) {
        stamina_boost--;
    } else if (stamina_boost < 0) {
        stamina_boost++;
    }
    if (stamina_boost == 0) {
        stamina_boost_stop();
    }
}

//
// The boost is done. Undo any settings.
//
void gh_thing::stamina_boost_stop (void)
{
    GH_FUNCTION()
}

