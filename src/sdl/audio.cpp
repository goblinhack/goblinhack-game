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

    $Id: audio.cpp 498 2007-04-07 14:43:17Z goblinhack $
*/

#include <ghack.h>
#include <gh_audio.h>
//#include <gh_subsys.h>

#ifdef MACOSX
#include <SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif

gh_audio::hash_all_audio gh_audio::all_audio;

gh_audio::gh_audio (string file)
{
    GH_FUNCTION()

    if (!file.size()) {
        GH_THROW("no file");
    }

    string filename = gh_global::locate(file);

//    cout << "AUDIO  \"" << filename << "\"" << endl;

    audio = Mix_LoadWAV((char*)filename.c_str());
    if (audio == NULL) {
        GH_WARN(("failed in loading " + filename));
        return;
    }

    channel = 0;

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

gh_audio::~gh_audio (void)
{
    GH_FUNCTION()

    if (channel) {
        Mix_HaltChannel(channel);
    }
}

//
// Nuke all sounds
//
void gh_audio::unload_all (void)
{
    GH_FUNCTION()

    GH_LOG("Unloading audio");

    hash_all_audio::iterator s = all_audio.begin();

    while (s != all_audio.end()) {
        delete s->second;
        s++;
    }

    hash_all_audio::iterator so = all_audio.begin();
    hash_all_audio::iterator eo = all_audio.end();

    all_audio.erase(so, eo);
}

void gh_audio::load_all (void)
{
    GH_FUNCTION()

    string dir = gh_global::locatedir("audio");

    if (dir == "") {
        GH_THROW("no audio dir?");
        return;
    }

    GH_LOG("Load audio");

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

        if (all_audio[s]) {
            GH_THROW(("duplicate named object " + full_name));
        }

        all_audio[s] = new gh_audio(full_name);

        if (!all_audio[s]) {
            GH_THROW(("couldn't create object " + full_name));
        }
    }
}

void gh_audio::init (void)
{
    GH_FUNCTION()

    int audio_rate = 22050;
    int audio_format = MIX_DEFAULT_FORMAT;
    int audio_channels = 2;
    int audio_buffers = 1024;

    if (Mix_OpenAudio(audio_rate,
                      audio_format,
                      audio_channels,
                      audio_buffers)) {

        GH_WARN("unable to initialize SDL Mix audio");
    }
}

void gh_audio::fini (void)
{
    GH_FUNCTION()

    Mix_CloseAudio();
}

void gh_audio::play (string name, int volume)
{
    GH_FUNCTION()

    if (!gh_global::sound) {
        return;
    }

    if (name == "") {
        return;
    }

    //
    // For performance, play less sounds.
    //
//    static uint32 last_sound_played_when;
//
//    if (!last_sound_played_when) {
//      last_sound_played_when = gh_get_time_milli();
//    } else {
//      if (!gh_have_x_tenths_passed_since(1, last_sound_played_when)) {
//          return;
//      }
//    }
//
//    last_sound_played_when = gh_get_time_milli();

    gh_audio *a = gh_audio::find(name.c_str(), false);
    if (!a) {
        GH_WARN("couldn't find " + name);
        return;
    }

    if (volume) {
        Mix_VolumeChunk((Mix_Chunk*) a->audio, volume);
    }

    a->channel = Mix_PlayChannel(-1, (Mix_Chunk*) a->audio, 0);
}

void gh_audio::halt (string name)
{
    GH_FUNCTION()

    if (name == "") {
        return;
    }

    gh_audio *a = gh_audio::find(name.c_str(), false);
    if (!a) {
        return;
    }

    if (!a->channel) {
        return;
    }

    Mix_HaltChannel(a->channel);

    a->channel = 0;
}
