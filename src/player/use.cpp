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

    $Id: tick.cpp 3 2006-08-25 14:46:56Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_term.h>
#include <gh_thing.h>
#include <gh_templates.h>
#include <gh_game.h>

void gh_player::use_item (void)
{
    GH_FUNCTION()

    if (!items.size()) {
        game->new_message("Thou hast nothing of any use");
        return;
    }

    list<gh_item>::iterator i;
    for (i = items.begin(); i != items.end(); i++) {
        gh_item *item = &(*i);

        if ((items.size() == 1) || (item->item_no == item_last_selected)) {
            thing->use_item_name(item->name);
            return;
        }
    }
}
