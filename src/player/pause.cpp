/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (look your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    Neil McGill

    $Id: loop.cpp 134 2006-09-24 02:43:04Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_game.h>
#include <gh_term.h>

void gh_player::display_pause (void)
{
    GH_FUNCTION()

    list<string> pause;

    pause.push_back("");
    pause.push_back("");
    pause.push_back("   %%fg=red%%bg=blackThou art paused  ");
    pause.push_back("");
    pause.push_back(" %%fg=yellowPress any key to continue ");
    pause.push_back("");
    pause.push_back("");

    gh_term_text_box(pause,
             GH_TERM_COLOR_BLACK, // middle bg/fg
             GH_TERM_COLOR_BLACK,
             GH_TERM_COLOR_RED, // border bg/fg
             GH_TERM_COLOR_RANDOM,
             '@',
             0 // no justify
             );

    display_game();
}
