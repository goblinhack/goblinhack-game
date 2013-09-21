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

    $Id: gh_sdl_event.h 513 2007-05-07 01:46:54Z goblinhack $
*/ 

#ifndef _GH_SDL_EVENT_H_
#define _GH_SDL_EVENT_H_

#include <SDL/SDL.h>

extern char sdl_evt_to_char(const SDL_keysym &evt);

struct gh_sdl_key_eq;
class gh_sdl_key;

class gh_sdl_key : public gh_stream
{
public:
        gh_sdl_key() : key_name(""),
                           mod_name(""),
                           key(SDLK_UNKNOWN),
                           mod(KMOD_NONE),
                           key_down(false),
                           key_up(false),
                           key_held(false),
                           mouse_button(-1),
                           mouse_down(false),
                           mouse_up(false),
                           mouse_held(false)
                           { }

        static void init(void);

        string key_name;
        string mod_name;

        int key;
        int mod;
        bool key_down;
        bool key_up;
        bool key_held;

        int mouse_button;
        bool mouse_down;
        bool mouse_up;
        bool mouse_held;

        friend void operator>> (gh_stream_in& is, gh_sdl_key *me);
        friend void operator<< (gh_stream_out& os, const gh_sdl_key *me);
};

struct gh_sdl_key_eq {
    bool operator()(const gh_sdl_key &s1, const gh_sdl_key &s2) const
    {
        return ((s1.key < s2.key) && (s1.mod < s2.mod));
    }
};

//
// Hash to get from a SDL key number to a name
//
typedef map<gh_sdl_key, const char*, gh_sdl_key_eq> hash_sdl_key_to_name;

//
// Hash to get from a SDL key name to a number
//
typedef map<string, gh_sdl_key> hash_sdl_name_to_key;

extern hash_sdl_key_to_name sdl_key_to_name;
extern hash_sdl_name_to_key sdl_name_to_key;

extern char sdl_evt_to_char(const gh_sdl_key &evt);


#endif /* _GH_SDL_H_ */
