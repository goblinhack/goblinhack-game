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

    $Id: global.cpp 1265 2009-08-02 15:59:12Z Ghack $
*/

#include <ghack.h>
#include <gh_global.h>
#include <gh_screen.h>
#include <gh_math.h>

bool                                    gh_global::fullscreen;
gh_size                                 gh_global::screensize;
gh_size                                 gh_global::default_screensize;
list<string>                            gh_global::systemPath;
bool                                    gh_global::noframe;
bool                                    gh_global::hack;
bool                                    gh_global::done;
bool                                    gh_global::dump;
bool                                    gh_global::mouse_visible;
bool                                    gh_global::fps;
bool                                    gh_global::too_slow;
bool                                    gh_global::really_slow;
bool                                    gh_global::music;
bool                                    gh_global::sound;
bool                                    gh_global::debug;
bool                                    gh_global::playing;
string                                  gh_global::player_file;
bool                                    gh_global::trace_mode;
bool                                    gh_global::sdl_initted;
bool                                    gh_global::config_changed;
uint8                                   gh_global::anim_speed = 15;
gh_global                               global("config/config.gz");
gh_fpixel                               gh_global::flash_color(0,0,0);
int                                     gh_global::how_many_things;
float                                   gh_global::screen_shake;
int                                     gh_global::monst_count;
class gh_game *                         gh_global::game;
GLint                                   gh_global::gl_rgb;
GLint                                   gh_global::gl_max_texture_size;
GLint                                   gh_global::gl_max_lights;
GLint                                   gh_global::gl_max_elements_indices;
GLint                                   gh_global::gl_max_elements_vertices;
GLint                                   gh_global::gl_max_texture_units_arb;

gh_global::gh_global (const string gh_cfg_file)
{
    gh_backtrace_init();

    static string cwd;
    static char cwdbuf[1000];

    if (getcwd(cwdbuf, sizeof(cwdbuf))) {
        cwd = string(cwdbuf);
    }

    //
    // Prepend the current working directory to the search path.
    //
    gh_global::systemPath.push_back(cwd + "/src/resources");
    gh_global::systemPath.push_back(cwd + "/resources");
    gh_global::systemPath.push_back(cwd);

    //
    // This shouldn't be needed, but is here just in case for relative paths.
    //
    gh_global::systemPath.push_back("/src/resources");
    gh_global::systemPath.push_back("/resources");
    gh_global::systemPath.push_back("");

    //
    // The installed path.
    //
    gh_global::systemPath.push_back(GH_INSTALLED_DIR "/src/resources");
    gh_global::systemPath.push_back(GH_INSTALLED_DIR "/resources");
    gh_global::systemPath.push_back(GH_INSTALLED_DIR);

#ifdef DEBUG
    list<string>::iterator p;

    cout << "Paths to search for files: " << endl;
    for (p = gh_global::systemPath.begin();
        p != gh_global::systemPath.end(); p++) {
        cout << "    " << *p << endl;
    }
#endif

    gh_global::music = true;
    gh_global::sound = true;

    gh_math::rand_init();
    gh_math::fsin_init();
    gh_math::fcos_init();
}

gh_global::~gh_global (void)
{
    GH_FUNCTION()
}
