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

    $Id: gh_audio.h 475 2007-03-10 03:29:22Z goblinhack $
*/

#ifndef _GH_AUDIO_H_
#define _GH_AUDIO_H_

class gh_audio : public gh_stream
{
public:

    void                    *audio;
    int                     channel;
    string                  short_name;
    string                  full_name;

    //
    // Create audio from file
    //
    gh_audio(string file);

    ~gh_audio();

    //
    // Audio bank operations
    //
    typedef map<string, gh_audio*> hash_all_audio;

    static hash_all_audio all_audio;
    static void init(void);
    static void fini(void);
    static void unload_all(void);
    static void load_all(void);

    //
    // Find this texture in the texture bank
    //
    gh_inline static gh_audio* find (const string &s1)
    {
        return (find(s1.c_str()));
    }

    //
    // Find this audio in the audio bank
    //
    gh_inline static gh_audio* find (const char* s1, const bool err = true)
    {
        gh_audio::hash_all_audio::iterator m = all_audio.find(s1);

        if (m == all_audio.end()) {
            if (err) {
                GH_THROW("couldn't find " + string(s1) + " in loaded list");
            }
            return (NULL);
        }

        gh_audio *audio = m->second;

        if (audio == NULL) {
            GH_THROW("null audio " + string(s1) + " in loaded list");
        }

        return (audio);
    }

    ostream& put(ostream& os) const;

    static void play(string name, int volume=48); // full 128
    static void halt(string);
};

extern gh_audio::hash_all_audio all_audio;

#endif /* _GH_AUDIO_H_ */
