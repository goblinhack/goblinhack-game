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

    $Id: level_map.cpp 818 2008-04-20 14:15:39Z goblinhack $
*/

#include <ghack.h>
#include <gh_player.h>
#include <gh_level_map.h>
#include <gh_global.h>
#include <gh_zip.h>

gh_level_maps::gh_level_maps (void)
{
    GH_FUNCTION()

    gh_read_file(this, "levels/which",
                 true /* noisy */,
                 true /* err */);
}

gh_level_maps::~gh_level_maps (void)
{
    GH_FUNCTION()
}

//
// Given a level number, look at the array of arrays of level names and
// chances of appearing and pick one.
//
string gh_level_maps::choose (int level)
{
    level--;

    if (level >= (int) level_maps.size()) {
        return ("");
    }

    vector<gh_level_map>::iterator s = level_maps[level].level_maps_list.begin();
    vector<gh_level_map>::iterator e = level_maps[level].level_maps_list.end();

    while (true) {
        vector<gh_level_map>::iterator i;

        for (i = s; i != e; i++) {
            gh_level_map m = *i;

            if ((rand() % 100) < m.chance) {
                return (m.name);
            }
        }
    }
}

gh_level_maps_list::gh_level_maps_list (void)
{
}

gh_level_maps_list::~gh_level_maps_list (void)
{
}

void operator>> (gh_stream_in& is, gh_level_maps *me)
{
    GH_FUNCTION()

    me->get_class_container(is, &me->level_maps, "level_maps");
}

void operator<< (gh_stream_out& os, const gh_level_maps *me)
{
    GH_FUNCTION()

    me->put_class_container(os, &me->level_maps, "level_maps");
}

void operator>> (gh_stream_in& is, gh_level_maps_list *me)
{
    GH_FUNCTION()

    me->get_class_container(is, &me->level_maps_list, "list");
}

void operator<< (gh_stream_out& os, const gh_level_maps_list *me)
{
    GH_FUNCTION()

    me->put_class_container(os, &me->level_maps_list, "list");
}

void operator>> (gh_stream_in& is, gh_level_map *me)
{
    GH_FUNCTION()

    me->get_unnamed(is, &me->name);
    me->get_unnamed(is, &me->chance);
}

void operator<< (gh_stream_out& os, const gh_level_map *me)
{
    GH_FUNCTION()

    me->put_unnamed(os, &me->name);
    me->put_unnamed(os, &me->chance);
}
