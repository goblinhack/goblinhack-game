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

    $Id: ghack.h 276 2006-11-20 21:16:05Z goblinhack $
*/ 

#ifndef _GH_ARRAY2D_H_
#define _GH_ARRAY2D_H_

#include <vector>
#include <functional>
#include <algorithm>

#define GH_ARRAY2D(__TYPE__) vector< vector< __TYPE__ > >

template <class T> GH_ARRAY2D(T) *gh_array2d (int w, int h, T t)
{
    GH_ARRAY2D(T) *my_array = new GH_ARRAY2D(T);

    (*my_array).resize(w);

    while (--w >= 0) {
        (*my_array)[w].resize(h);
    }

    return (my_array);
}

/*
 * e.g.
 *
 * class member {
 * public:
 *     int data;
 * 
 *     member(void) {
 *      data = 0;
 *     }
 *     
 *     ~member(void) {
 *     }
 * };
 * 
 * int main()
 * {
 *     int x = 100;
 *     int y = 100;
 *     member type;
 * 
 *     GH_ARRAY2D(member) *a = array(x, y, type);
 * 
 *     (*a)[3][3].data = 1;
 *     (*a)[1][1].data = 2;
 *     (*a)[2][2].data = 3;
 * 
 *     cerr << (*a)[3][3].data << endl;
 *     cerr << (*a)[1][1].data << endl;
 *     cerr << (*a)[2][2].data << endl;
 * 
 *     delete a;
 * }
 */
    
#endif
