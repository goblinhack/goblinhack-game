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

    $Id: gh_global.h 620 2008-01-04 14:45:07Z goblinhack $
*/

#ifndef _GH_GLOBAL_H_
#define _GH_GLOBAL_H_

#include <gh_stream.h>
#include <gh_size.h>
#include <gh_pixel.h>

class gh_global : public gh_stream
{
public:
    gh_global(const string gh_cfg_file);
    ~gh_global(void);

    //
    // Default path used to look for all resources.
    //
    static list<string> systemPath;

    //
    // Loading files and dirs:
    //
    static const bool file_exists(string filename);
    static const int file_size(string filename);
    static const bool file_unlink(string filename);
    static const string locate(string filename,
                               bool quiet=false);
    static bool exists(string filename);
    static const string locatedir(string subdir);

    static vector<string> listdir_recursive(const string dir,
                                            const string suffix = "",
                                            const string exlude_suffix = "");
    static vector<string> listdir(const string dir);

    static vector<string> list_files_only(const string dir,
                                          const string suffix = "",
                                          const string exlude_suffix = "");
    static vector<string> list_dirs_only(const string dir);

    static bool                 fullscreen;
    static bool                 noframe;
    static bool                 done;
    static bool                 hack;
    static bool                 mouse_visible;
    static bool                 dump;
    static bool                 fps;
    static bool                 too_slow;
    static bool                 really_slow;
    static bool                 music;
    static bool                 sound;
    static bool                 debug;
    static bool                 playing;
    static string               player_file;
    static bool                 trace_mode;
    static bool                 sdl_initted;
    static bool                 config_changed;
    static gh_size              screensize;
    static gh_size              default_screensize;
    static uint8                anim_speed;
    static gh_fpixel            flash_color;
    static int                  how_many_things;
    static float                screen_shake;
    static int                  monst_count;
    static class gh_game *      game;
    static GLint                gl_rgb;
    static GLint                gl_max_texture_size;
    static GLint                gl_max_lights;
    static GLint                gl_max_elements_indices;
    static GLint                gl_max_elements_vertices;
    static GLint                gl_max_texture_units_arb;
};

extern gh_global global;

extern void gh_widget_main_window_init(void);
extern void gh_widget_main_window_fini(void);

extern void gh_widget_settings_init(void);
extern void gh_widget_settings_fini(void);

#endif /* _GH_GLOBAL_H_ */
