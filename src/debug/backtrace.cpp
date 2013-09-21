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

    $Id: backtrace.cpp 990 2008-07-13 21:06:25Z Ghack $
*/

#include <ghack.h>

static const char *gh_function_names_arr[1024];
static const char *gh_function_files_arr[1024];
static uint16 gh_function_lines_arr[1024];
static const char gh_indent[] = "            CRASH SOON !!! ......................................................................................................................................................................................................................................................................................................................................................................................................................................................................";

const char **gh_function_names = gh_function_names_arr;
const char **gh_function_files = gh_function_files_arr;
uint16 *gh_function_lines = gh_function_lines_arr;

int gh_function_indent_depth;

static inline const char *gh_indent_show (const int depth)
{
    return (&gh_indent[sizeof(gh_indent) - depth]);
}

void gh_backtrace_init (void)
{
    if (gh_function_names) {
        return;
    }

    gh_function_names = gh_function_names_arr;
    gh_function_files = gh_function_files_arr;
    gh_function_lines = gh_function_lines_arr;
}

void gh_backtrace (void)
{
    fprintf(stderr, "backtrace:\n");

    for (int depth=0; depth < gh_function_indent_depth; depth++) {
        fprintf(stderr,
                "%30s:%-3d %*s%s()\n",
                gh_function_files_arr[depth],
                gh_function_lines_arr[depth],
                depth,
                gh_indent_show(depth),
                gh_function_names_arr[depth]);
    }
    fflush(stderr);

    fprintf(stdout, "backtrace:\n");

    for (int depth=0; depth < gh_function_indent_depth; depth++) {
        fprintf(stdout,
                "%30s:%-3d %*s%s()\n",
                gh_function_files_arr[depth],
                gh_function_lines_arr[depth],
                depth,
                gh_indent_show(depth),
                gh_function_names_arr[depth]);
    }
    fflush(stdout);
}

void gh_function_trace (void)
{
    int depth = gh_function_indent_depth - 1;

    printf("%30s:%-3d %*s%s()\n",
           gh_function_files_arr[depth],
           gh_function_lines_arr[depth],
           depth,
           gh_indent_show(depth),
           gh_function_names_arr[depth]);
}
