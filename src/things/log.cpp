/*
    goblinhack.
    Copyright (C) 1999-2010 Neil McGill

    This game is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This game is distributed in the hope that it will be fun,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this game; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

    Neil McGill

    $Id: time.cpp 276 2006-11-20 21:16:05Z goblinhack $
*/

#include <ghack.h>
#include <gh_sdl.h>
#include <gh_thing.h>

int gh_log_indent_depth;

static const int gh_log_indent_max = 100;
static const char gh_log_indent[] = "            CRASH SOON !!! . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . ";

static inline const char *log_indent_show (const int depth)
{
    return (&gh_log_indent[sizeof(gh_log_indent) - depth]);
}

void gh_thing_log (void * thing, string msg)
{
    GH_FUNCTION()

    gh_thing *t = (gh_thing *)thing;

    if (!t) {
        return;
    }

    GH_THING_VERIFY(t);

    if (msg == "") {
        return;
    }

    if (gh_global::trace_mode) {
        printf("%s [%8p/%06d:%-30s]:%s%s\n",
               gh_sdl_get_time_string(),
               t,
               t->id,
               t->named().c_str(),
               log_indent_show(gh_log_indent_depth +
                               gh_function_indent_depth + 1),
               msg.c_str());
    } else {
        printf("%s [%8p/%06d:%-30s]:%s%s\n",
               gh_sdl_get_time_string(),
               t,
               t->id,
               t->named().c_str(),
               log_indent_show(gh_log_indent_depth + 1),
               msg.c_str());
    }
}

void gh_thing_log_mand (void * thing, string msg)
{
    GH_FUNCTION()

    gh_thing *t = (gh_thing *)thing;

    if (!t) {
        if (gh_global::trace_mode) {
            printf("%s %s%s\n",
                   gh_sdl_get_time_string(),
                   log_indent_show(gh_log_indent_depth +
                                   gh_function_indent_depth + 1),
                   msg.c_str());
        } else {
            printf("%s %s%s\n",
                   gh_sdl_get_time_string(),
                   log_indent_show(gh_log_indent_depth + 1),
                   msg.c_str());
        }

        return;
    }

    GH_THING_VERIFY(t);

    if (msg == "") {
        return;
    }

    if (gh_global::trace_mode) {
        printf("%s [%8p/%06d:%-30s]:%s%s\n",
               gh_sdl_get_time_string(),
               t,
               t->id,
               t->named().c_str(),
               log_indent_show(gh_log_indent_depth +
                               gh_function_indent_depth + 1),
               msg.c_str());
    } else {
        printf("%s [%8p/%06d:%-30s]:%s%s\n",
               gh_sdl_get_time_string(),
               t,
               t->id,
               t->named().c_str(),
               log_indent_show(gh_log_indent_depth + 1),
               msg.c_str());
    }
}
