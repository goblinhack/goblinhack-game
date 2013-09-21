/*
    retreat.
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

    $Id: audio.cpp 498 2007-04-07 14:43:17Z goblinhack $
*/

#include <ghack.h>
#include <gh_game.h>
#include <gh_player.h>
#include <gh_thing.h>
#include <gh_level.h>
#include <gh_audio.h>
#include <gh_math.h>

void gh_thing::play (string audio_name, int volume)
{
    GH_FUNCTION()

    if (!gh_global::sound) {
        return;
    }

    float distance = hero_distance();

    if (gh_global::too_slow) {
        if (is_monst) {
            if (distance > 15) {
                return;
            }
        }
    }

    volume = (int)((float)volume * (1.0 - (distance * 0.03)));

    if (volume <= 0) {
        return;
    }

    gh_audio::play(audio_name, volume);
}
