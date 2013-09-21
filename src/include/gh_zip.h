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

    $Id: gh_zip.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_ZIP_H_
#define _GH_ZIP_H_

#include <string>

/*
 * Used to unzip a .gz or .tgz file
 */
int gzwriteall(string name, char *buf, size_t size, size_t compression = 9);
char *gzreadall(string name, size_t *size, size_t chunklet = 200000,
                bool binary = false);

extern bool gzequals(const string &a, const string &b);
extern int gzstrcmp(const char* a, const char* b);
extern bool gzfileequals(const string &a, const string &b);
extern bool gzfileequals(const char* a, const char* b);

#endif /* _GH_ZIP_H_ */
