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

    $Id: gh_unique_thing.h 226 2006-10-22 21:01:19Z goblinhack $
*/

#ifndef _GH_UNIQUE_THINGS_H_
#define _GH_UNIQUE_THINGS_H_

#include <ghack.h>

class gh_unique_thing_list : public gh_stream
{   
public:
    void *self;

    gh_unique_thing_list(void);
    ~gh_unique_thing_list(void);

    //
    // List of all named things, 'orc/bob', 'orc/sue', ...
    //
    static list<class gh_thing *> things;

    //
    // Hash to an array of titles for a thing name 'orc' -> 'bob', 'sue' ...
    //
    typedef map<string, vector<string> > hash_unique_things_name;

    hash_unique_things_name unique_things_name;

    //
    // Hash to a titles to that thing 'bob' -> thing
    //
    typedef map<string, class gh_thing *> hash_unique_things_text;

    hash_unique_things_text unique_things_text;

    void copy_a_named_thing(class gh_thing *t);

    friend void operator>> (gh_stream_in& is, gh_unique_thing_list *me);
    friend void operator<< (gh_stream_out& os, const gh_unique_thing_list *me);
};

extern gh_unique_thing_list *gh_unique_things;

#endif /* _GH_UNIQUE_THINGS_H_ */
