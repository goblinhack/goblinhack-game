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

    $Id: collect.cpp 185 2006-10-08 01:50:52Z goblinhack $
*/

#include <ghack.h>
#include <gh_level.h>
#include <gh_subsys.h>
#include <gh_templates.h>
#include <gh_game.h>
#include <gh_player.h>
#include <gh_math.h>
#include <gh_string.h>

//
// Find every cell this thing can see.
//
void gh_thing::hit_all_i_can_see (gh_thing *item, vector<string> action)
{
    GH_FUNCTION()

    vector<string> attacks;
    vector<int> attack_chance;

    //
    // Walk through the list of patterns we want this potion to hit
    // when it spits out its missiles. Make up the chase list that
    // we then use to modify the missiles.
    //
    vector<string>::iterator act = action.begin();

    //
    // Skip the function name
    //
    act++;

    while (act != action.end()) {
        string pattern = *act;

        attacks.push_back(pattern);
        attack_chance.push_back(100);

        act++;
    }

    list <gh_thing *> thrown;
    list <gh_thing *>::iterator missile;

    thrown = try_to_throw_something(NULL, /* target */
                                    this, /* owner */
                                    "missiles/potion/hit_hero_only",
                                    true, /* want thrown list */
                                    true /* always_throw */);

    for (missile = thrown.begin(); missile != thrown.end(); missile++) {
        gh_thing *m = *missile;

        m->attacks              = attacks;
        m->wake_reasons         = wake_reasons;
        m->attack_chance        = attack_chance;
        m->bite_damage          = item->bite_damage;
        m->bite_armour          = item->bite_armour;
        m->bite_treasure        = item->bite_treasure;
        m->bite_karma           = item->bite_karma;
        m->bite_karma_chance    = item->bite_karma_chance;
        m->bite_intelligence            = item->bite_intelligence;
        m->bite_intelligence_chance     = item->bite_intelligence_chance;
        m->bite_poison          = item->bite_poison;
        m->bite_poison_chance   = item->bite_poison_chance;
        m->bite_stone           = item->bite_stone;
        m->bite_stone_chance    = item->bite_stone_chance;
        m->bite_fate            = item->bite_fate;
        m->bite_fate_chance     = item->bite_fate_chance;
        m->is_spell             = true;

        if (item->metabolism()) {
            m->_metabolism      = item->metabolism();
        }
    }
}
