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

    $Id: gh_backtrace.h 618 2008-01-01 12:50:13Z goblinhack $
*/

#ifndef _GH_BACKTRACE_H_
#define _GH_BACKTRACE_H_

extern const char **gh_function_names;
extern const char **gh_function_files;
extern uint16 *gh_function_lines;
extern int gh_function_indent_depth;
extern void gh_function_trace(void);

#define GH_FUNCTION()                                           \
        gh_function destroyed_on_exit;                          \
                                                                \
        *(gh_function_files++) = __FILE__;                      \
        *(gh_function_names++) = __FUNCTION__;                  \
        *(gh_function_lines++) = __LINE__;                      \
                                                                \
        if (gh_global::trace_mode) {                            \
            gh_function_trace();                                \
        }

class gh_function {
public:
    gh_inline gh_function (void) {
        gh_function_indent_depth++;
    }

    gh_inline ~gh_function () {
        gh_function_indent_depth--;
        gh_function_files--;
        gh_function_names--;
        gh_function_lines--;
    }
};

extern void gh_backtrace_init(void);
extern void gh_backtrace(void);

#endif /* _GH_BACKTRACE_H_ */
