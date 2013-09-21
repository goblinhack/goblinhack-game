/*
    retreat.
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

    $Id: gh_music.h 475 2007-03-10 03:29:22Z goblinhack $
*/

#ifndef _GH_MUSIC_H_
#define _GH_MUSIC_H_

class gh_music : public gh_stream
{
public:

    void                    *music;
    string                  short_name;
    string                  full_name;

    //
    // Create music from file
    //
    gh_music(string file);

    ~gh_music();

    //
    // Audio bank operations
    //
    typedef map<string, gh_music*> hash_all_music;

    static hash_all_music all_music;
    static void init(void);
    static void fini(void);
    static void unload_all(void);
    static void load_all(void);

    //
    // Find this texture in the texture bank
    //
    gh_inline static gh_music* find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this music in the music bank
    //
    gh_inline static gh_music* find (const char* s1, const bool err = true)
    {
        gh_music::hash_all_music::iterator m = all_music.find(s1);

        if (m == all_music.end()) {
            if (err) {
                GH_THROW("couldn't find " + string(s1) + " in loaded list");
            }
            return (NULL);
        }

        gh_music *music = m->second;

        if (music == NULL) {
            GH_THROW("null music " + string(s1) + " in loaded list");
        }

        return (music);
    }

    ostream& put(ostream& os) const;

    static void play(string name, int volume=64); // full 128
    static void halt(void);
};

extern gh_music::hash_all_music all_music;

#endif /* _GH_MUSIC_H_ */
