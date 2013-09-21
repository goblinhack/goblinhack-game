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

    $Id: gh_debug.h 606 2007-12-15 22:01:13Z goblinhack $
*/

#ifndef _GH_DEBUG_H_
#define _GH_DEBUG_H_

#define GH_WARN(msg) {\
    cout << endl << "** Warning ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
        "() " << msg << endl;\
    cerr << endl << "** Warning ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
        "() " << msg << endl;\
    gh_backtrace();\
}

extern void GH_LOG(string);
extern void GH_ROOM_LOG(void *, string);
extern void gh_thing_log(void *, string);
extern void gh_thing_log_mand(void *, string);

//
// Double macro to force __LINE__ expansion.
//
#define JOIN2(a, b) a ## b
#define JOIN(a, b) JOIN2(a, b)
#define GH_THING_LOG(__thing__, args...)                        \
{                                                               \
    if (gh_global::debug) {                                     \
        GH_FUNCTION()                                           \
        gh_log JOIN(log, __LINE__);                             \
        gh_thing_log(__thing__, ##args);                        \
    }                                                           \
}


extern int gh_log_indent_depth;

class gh_log {

public:
    gh_log (void) { gh_log_indent_depth++; }
    ~gh_log () { gh_log_indent_depth--; }
};

#define GH_THROW(msg) {\
    if (msg != string("")) {\
        cout << endl << "** Error ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
            "() " << msg << endl;\
        cerr << endl << "** Error ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
            "() " << msg << endl;\
        gh_backtrace();\
    } else { \
        cout << endl << "** Error ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
            "() " << endl;\
        cerr << endl << "** Error ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
            "() " << endl;\
    } \
    throw invalid_argument (string(msg));\
}

#define GH_EXIT(msg) {\
    cout << endl << "** Bail-out ** " << __FILE__ << "(" << __LINE__ << ")::" << __FUNCTION__ <<\
        "() " << msg << endl;\
    gh_backtrace();\
    exit(1); \
}

#endif /* _GH_DEBUG_H_ */
