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

    $Id: music.cpp 498 2007-04-07 14:43:17Z goblinhack $
*/

#include <ghack.h>
#include <gh_music.h>

#ifdef MACOSX
#include <SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif

gh_music::hash_all_music gh_music::all_music;

gh_music::gh_music (string file)
{
    GH_FUNCTION()

    if (!file.size()) {
        GH_THROW("no file");
    }

    string filename = gh_global::locate(file);

//    cout << "MUSIC  \"" << filename << "\"" << endl;

    music = Mix_LoadMUS((char*)filename.c_str());
    if (music == NULL) {
        GH_WARN(("failed in loading " + filename));
        return;
    }

    //
    // Each file should be unique without the path
    //
    const char *sep;

    if ((sep = strrchr(filename.c_str(), '/')) != NULL) {
        short_name = sep + 1;
    } else {
        short_name = filename;
    }

    if (strstr(short_name.c_str(), ".gz")) {
        short_name.resize(short_name.size() - 3);
    }

    full_name = filename;
}

gh_music::~gh_music (void)
{
    GH_FUNCTION()

    Mix_HaltMusic();
    Mix_FreeMusic((Mix_Music*) music);
}

//
// Nuke all sounds
//
void gh_music::unload_all (void)
{
    GH_FUNCTION()

    GH_LOG("Unloading music");

    hash_all_music::iterator s = all_music.begin();

    while (s != all_music.end()) {
        delete s->second;
        s++;
    }

    hash_all_music::iterator so = all_music.begin();
    hash_all_music::iterator eo = all_music.end();

    all_music.erase(so, eo);
}

void gh_music::load_all (void)
{
    GH_FUNCTION()

    string dir = gh_global::locatedir("music");

    if (dir == "") {
        GH_THROW("no music dir?");
        return;
    }

    GH_LOG("Load music");

    vector<string> files = gh_global::listdir_recursive(dir, "");

    vector<string>::iterator i;

    for (i = files.begin(); i != files.end(); i++) {
        string full_name = *i;
        string short_name = *i;

        if (!string_contains(short_name, ".wav") &&
            !string_contains(short_name, ".ogg") &&
            !string_contains(short_name, ".mp3")) {
            continue;
        }

//      GH_LOG("  Loading " + full_name);

        if (strstr(short_name.c_str(), ".gz")) {
            short_name.resize(short_name.size() - 3);
        }

        const char *s = strrchr(short_name.c_str(), '/');

        if (s == NULL) {
            s = short_name.c_str();
        } else {
            s++;
        }

        string n = s;

        string_replace(n, ".wav","");
        string_replace(n, ".ogg","");
        string_replace(n, ".mp3","");

        s = n.c_str();

        if (all_music[s]) {
            GH_THROW(("duplicate named object " + full_name));
        }

        all_music[s] = new gh_music(full_name);

        if (!all_music[s]) {
            GH_THROW(("couldn't create object " + full_name));
        }
    }
}

void gh_music::init (void)
{
    GH_FUNCTION()
}

void gh_music::fini (void)
{
    GH_FUNCTION()
}

void gh_music::play (string name, int volume)
{
    GH_FUNCTION()

    if (!gh_global::music) {
        return;
    }

    if (name == "") {
        return;
    }

    gh_music *a = gh_music::find(name.c_str(), false);
    if (!a) {
        GH_WARN("couldn't find " + name);
        return;
    }

    Mix_VolumeMusic(volume / 2);

    Mix_FadeInMusic((Mix_Music*) a->music, -1, 3000);
}

void gh_music::halt (void)
{
    GH_FUNCTION()

    Mix_HaltMusic();
}
