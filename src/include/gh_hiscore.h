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

    $Id: gh_hiscore.h 557 2007-08-16 20:33:18Z goblinhack $
*/

#ifndef _GH_HISCORE_H_
#define _GH_HISCORE_H_

class gh_hiscore : public gh_stream
{   
public:
    gh_hiscore (void) {}
    gh_hiscore (string name, 
                int score, 
                int level_reached,
                string killed_by)
    {
        this->name = name;
        this->score = score;
        this->level_reached = level_reached;
        this->killed_by = killed_by;
    }

    ~gh_hiscore (void) { }

    //
    // Who made the score?
    //
    string  name;
    string  killed_by;
    int     score;
    int     level_reached;

    //
    // The number of scores in the table.
    //
    static const int max = 10;
    static const int max_displayed = 10;

    friend void operator>> (gh_stream_in& is, gh_hiscore *me);
    friend void operator<< (gh_stream_out& os, const gh_hiscore *me);
};

class gh_hiscores : public gh_stream
{
public:
    gh_hiscores (void) { name = ""; };
    gh_hiscores (string name);
    ~gh_hiscores (void);

    //
    // The name of the hiscore file ("hiscores")
    //
    string              name;

    //
    // All the hiscores held in the file.
    //
    vector<gh_hiscore>  hiscores;

    void add_new_hiscore(const class gh_player *player, const string name);
    bool is_new_hiscore(const class gh_player *player);
    bool is_new_highest_hiscore(const class gh_player *player);
    const char *place_str(const class gh_player *player);

    friend void operator>> (gh_stream_in& is, gh_hiscores *me);
    friend void operator<< (gh_stream_out& os, const gh_hiscores *me);
};

#endif /* _GH_HISCORE_H_ */
