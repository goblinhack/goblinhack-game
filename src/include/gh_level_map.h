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

    $Id: gh_level_map.h 557 2007-08-16 20:33:18Z goblinhack $
*/

#ifndef _GH_LEVEL_MAP_H_
#define _GH_LEVEL_MAP_H_

class gh_level_map : public gh_stream
{   
public:
    gh_level_map (void)
    {
        name = "";
        chance = 0;
    }

    gh_level_map (string name, int chance)
    {
        this->name = name;
        this->chance = chance;
    }

    ~gh_level_map (void) { }

    //
    // Who made the chance?
    //
    string  name;
    int     chance;

    friend void operator>> (gh_stream_in& is, gh_level_map *me);
    friend void operator<< (gh_stream_out& os, const gh_level_map *me);
};

class gh_level_maps_list : public gh_stream
{
public:
    gh_level_maps_list (void);
    ~gh_level_maps_list (void);

    //
    // All the level_maps_list held in the file.
    //
    vector<gh_level_map>        level_maps_list;

    friend void operator>> (gh_stream_in& is, gh_level_maps_list *me);
    friend void operator<< (gh_stream_out& os, const gh_level_maps_list *me);
};

class gh_level_maps : public gh_stream
{
public:
    gh_level_maps (void);
    ~gh_level_maps (void);
    string choose(int level);

    //
    // All the level_maps held in the file.
    //
    vector<gh_level_maps_list>  level_maps;

    friend void operator>> (gh_stream_in& is, gh_level_maps *me);
    friend void operator<< (gh_stream_out& os, const gh_level_maps *me);
};

#endif /* _GH_LEVEL_MAP_H_ */
