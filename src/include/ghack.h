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

    $Id: ghack.h 1235 2009-07-06 02:23:35Z Ghack $
*/ 

#ifndef _GH_GHACK_H_
#define _GH_GHACK_H_

//
// The maximum size of the maze we can ever generate.
//
#define GH_MAX_MAZE_SIZE                                200
#define GH_MIN_MAZE_SIZE                                32

//
// Check memory accesses
//
#define nGH_PTR_SANITY

//
// The number of pixels per char per map cell.
//
#define GH_RES_CHAR                                     10

//
// The number of pixels per map cell.
//
#define GH_RES                                          (6*GH_RES_CHAR)

//
// How many segments in our circle of vision we monsters look at.
//
#define GH_LIGHT_RAYS_MONST_MAX                         16

//
// How many segments in our circle of vision can our hero look at.
//
#define GH_LIGHT_RAYS_HERO_MAX                          256

//
// Debug the loading of files in detail
//
#define nGH_DO_PARSER_DEBUG

//
// Minor performance hit and caters for escape chars within strings
//
#define nGH_PARSER_SUPPORT_ESCAPE_CHARACTERS

//
// Debug the generation of graphics.
//
#define nGH_DEBUG_RENDER

//
// When a thing gets annoyed when you hit it.
//
#define GH_THING_ANGER_LIMIT                    35
#define GH_THING_FOLLOW_LIMIT                   35

#include "../../build/config.h"
#include <debugit.h>
#include <gh_version.h>

class gh_global;
class gh_player;
class gh_map;
class gh_thing;
class gh_thing_registration;
class gh_maze;
class gh_level;
class gh_room;
class gh_model;
class gh_model_anim;
class gh_editor;
class gh_stream_out;
class gh_stream_in;
class gh_surface;

#include <gh_main.h>
#include <gh_sys.h>
#include <gh_macro.h>
#include <gh_hash.h>
#include <gh_mathdef.h>
#include <gh_backtrace.h>
#include <gh_debug.h>
#include <gh_io.h>
#include <gh_size.h>
#include <gh_point.h>
#include <gh_string.h>
#include <gh_global.h>
#include <gh_global.h>

#endif /* _GH_GHACK_H_ */
