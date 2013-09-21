#ifndef _ZIP_H_
#define _ZIP_H_

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

    $Id: zip.cpp 3 2006-08-25 14:46:56Z goblinhack $
*/

#include "../../build/config.h"
#include <debugit.h>

class gh_global;
class gh_player;
class gh_map;
class gh_thing;
class gh_thing_registration;
class gh_maze;
class gh_io;
class gh_stream_out;
class gh_stream_in;

#include <gh_sys.h>
#include <gh_macro.h>
#include <gh_hash.h>
#include <gh_mathdef.h>
#include <gh_backtrace.h>
#include <gh_debug.h>
#include <gh_string.h>


#include <zlib.h>

class gh_zip {

public:
    gh_zip();
    ~gh_zip();

    static gh_zip* instance();

    static void dos2unix (char * data, size_t *size);
    int gzwriteall (string name, char *buf, size_t size, size_t compression);
    char *gzreadall (string name, size_t *size, size_t chunklet, bool binary = false);

private:
    static gh_zip *mpInstance;

};


#endif // _ZIP_H_
