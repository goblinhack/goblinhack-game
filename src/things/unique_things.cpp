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

    $Id: hiscore.cpp 873 2008-05-17 22:44:38Z Ghack $
*/

#include <ghack.h>
#include <gh_thing.h>
#include <gh_math.h>
#include <gh_unique_things.h>

gh_unique_thing_list *gh_unique_things;

list<class gh_thing *> gh_unique_thing_list::things;

gh_unique_thing_list::gh_unique_thing_list (void)
{
    GH_FUNCTION()

    gh_read_file(this, "levels/names", true /* noisy */, true /* err */);

    list<class gh_thing *>::iterator i;

    for (i = gh_unique_thing_list::things.begin();
         i != gh_unique_thing_list::things.end(); i++) {

        gh_thing *t = *i;

        t->is_unique = true;

        switch (t->level_mod) {
        case 1:
            t->text = "%%fg=gray" + t->text;
            break;
        case 2:
            t->text = "%%fg=yellow" + t->text;
            break;
        case 3:
            t->text = "%%fg=orange" + t->text;
            break;
        case 4:
            t->text = "%%fg=red" + t->text;
            break;
        case 5:
            t->text = "%%fg=darkred" + t->text;
            break;
        }

#define SCALE_BY_PERCENT(x) (x) += (((x) / 10) * (t->level_mod * 3))

        SCALE_BY_PERCENT(t->health);
        SCALE_BY_PERCENT(t->score);
        SCALE_BY_PERCENT(t->armour);
        SCALE_BY_PERCENT(t->_strength);
        SCALE_BY_PERCENT(t->_dex);
        SCALE_BY_PERCENT(t->treasure);
        SCALE_BY_PERCENT(t->orig_health);
        SCALE_BY_PERCENT(t->bite_damage);
        SCALE_BY_PERCENT(t->bite_armour);
        SCALE_BY_PERCENT(t->bite_treasure);
        SCALE_BY_PERCENT(t->bite_karma);
        SCALE_BY_PERCENT(t->bite_intelligence);
        SCALE_BY_PERCENT(t->bite_poison);
        SCALE_BY_PERCENT(t->bite_stone);
        SCALE_BY_PERCENT(t->bite_fate);

        t->fate += t->level_mod / 2;

        unique_things_name[t->name].push_back(t->text);
        unique_things_text[t->text] = t;
    }
}

void gh_unique_thing_list::copy_a_named_thing (gh_thing *t)
{
    gh_unique_thing_list::hash_unique_things_name::iterator m = 
                    unique_things_name.find(t->name);

    if (m == unique_things_name.end()) {
        return;
    }

    vector<string> *names = &(m->second);

    vector<string>::iterator n = names->begin();
    int poss = names->size();

    string text = n[rand() % poss];

    gh_unique_thing_list::hash_unique_things_text::iterator m2 = 
                    unique_things_text.find(text);

    if (m2 == unique_things_text.end()) {
        return;
    }

    gh_thing *t2 = m2->second;
    
    if (gh_math::rand10000() < t2->appearing_chance_on_level(t->level)) {
        t->polycopy(t2);
        return;
    }
}

gh_unique_thing_list::~gh_unique_thing_list (void)
{
    GH_FUNCTION()

    list<class gh_thing *>::iterator i;

redo:
    for (i = gh_unique_thing_list::things.begin();
         i != gh_unique_thing_list::things.end(); i++) {

        gh_thing *t = *i;

        gh_unique_thing_list::things.erase(i);

        t->decref(GH_THING_REF_FIRST);

        goto redo;
    }
}

void operator>> (gh_stream_in& is, gh_unique_thing_list *me)
{
    GH_FUNCTION()

    me->get_class_pcontainer(is, &me->things, "things");
}

void operator<< (gh_stream_out& os, const gh_unique_thing_list *me)
{
    GH_FUNCTION()

    me->put_class_pcontainer(os, &me->things, "things");
}
