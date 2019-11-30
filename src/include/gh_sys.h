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

    $Id: gh_sys.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_SYS_H_
#define _GH_SYS_H_

//
// All the horrible system dependent stuff
//
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <typeinfo>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <iomanip>

//#ifndef __GXX_EXPERIMENTAL_CXX0X__
//#define decltype typeof
//#endif

using namespace std;

#undef uint8
#define uint8   unsigned char
#undef uint16
#define uint16  unsigned short
#undef uint32
#define uint32  unsigned int
#undef uint64
#define uint64  unsigned long long

#undef int8 
#define int8    char
#undef int16
#define int16   short
#undef int32
#define int32   int
#undef int64
#define int64   long long

#define gh_inline   inline

#endif /* _GH_SYS_H_ */
