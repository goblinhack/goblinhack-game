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

void gh_player::display_help (void)
{
    GH_FUNCTION()

    list<string> help;

    display_game();

    help.push_back("");
    help.push_back("");
    help.push_back("%%fg=green%%bg=blackThy Keys of Mighty Control");
    help.push_back("");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   Arrow keys                    "
        "%%fg=white%%bg=black"
        "to stalk the dungeon             ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "     SPACE                       "
        "%%fg=white%%bg=black"
        "to throw thy deadly weapons      ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "       o                         "
        "%%fg=white%%bg=black"
        "to unlock doors                  ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "      a,z                        "
        "%%fg=white%%bg=black"
        "to select items in your bag      ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   u/ENTER/L-SHIFT, d            "
        "%%fg=white%%bg=black"
        "to use/drop items in your bag    ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "       q                         "
        "%%fg=white%%bg=black"
        "to quit the game                 ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "       p                         "
        "%%fg=white%%bg=black"
        "to pause/pay in shops            ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "       s                         "
        "%%fg=white%%bg=black"
        "to save a noble hero             ");
    help.push_back("");
    help.push_back(" %%fg=whitePress any key to continue ");
    help.push_back("");

    gh_term_text_box(help,
             GH_TERM_COLOR_BLACK, // middle bg/fg
             GH_TERM_COLOR_BLACK,
             GH_TERM_COLOR_BLACK, // border bg/fg
             GH_TERM_COLOR_RANDOM,
             '@',
             0 // no justify
             );
}

void gh_player::display_editor_help (void)
{
    GH_FUNCTION()

    list<string> help;

    display_game();

    help.push_back("");
    help.push_back("");
    help.push_back("%%fg=green%%bg=blackThy Keys of Editing");
    help.push_back("");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   Arrow and mouse               "
        "%%fg=white%%bg=black"
        "to move the cursor               ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   SPACE/Left Mouse              "
        "%%fg=white%%bg=black"
        "to place selected objects        ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-SPACE/Middle Mouse       "
        "%%fg=white%%bg=black"
        "to pick up objects               ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   Backspace/Right Mouse         "
        "%%fg=white%%bg=black"
        "to remove objects                ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   m/r/x/./g                     "
        "%%fg=white%%bg=black"
        "monsters/rock/walls/floor/goblins");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   e/D/d/G                       "
        "%%fg=white%%bg=black"
        "exits/doors/dragons/generators   ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   A/W/c/f/_                     "
        "%%fg=white%%bg=black"
        "armour/wands/coins/food/traps    ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   M/u/o/p                       "
        "%%fg=white%%bg=black"
        "big monsters/undead/orcs/potions ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   -/!/=                         "
        "%%fg=white%%bg=black"
        "moving walls/electro doors       ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   a,z                           "
        "%%fg=white%%bg=black"
        "scroll up/down in select buffer  ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-g                        "
        "%%fg=white%%bg=black"
        "to change the grid snap          ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-s                        "
        "%%fg=white%%bg=black"
        "to save the level                ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-f                        "
        "%%fg=white%%bg=black"
        "to flood fill                    ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-t                        "
        "%%fg=white%%bg=black"
        "to give a thing a title          ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-j                        "
        "%%fg=white%%bg=black"
        "to set the level jump on a thing ");
    help.push_back(
        "%%fg=red%%bg=black"
        "                                 "
        "%%fg=white%%bg=black"
        "e.g. forest/3/2 for level 3 sub 2");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   CTRL-n                        "
        "%%fg=white%%bg=black"
        "to clear the level               ");
    help.push_back("");
    help.push_back(
        "%%fg=red%%bg=black"
        "   F11                           "
        "%%fg=white%%bg=black"
        "to return to playing the level   ");
    help.push_back("");
    help.push_back("");
    help.push_back(" %%fg=whitePress any key to continue ");
    help.push_back("");

    gh_term_text_box(help,
             GH_TERM_COLOR_BLACK, // middle bg/fg
             GH_TERM_COLOR_BLACK,
             GH_TERM_COLOR_BLACK, // border bg/fg
             GH_TERM_COLOR_RANDOM,
             '@',
             0 // no justify
             );
}
