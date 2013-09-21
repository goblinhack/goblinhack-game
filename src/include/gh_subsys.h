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

    $Id: gh_subsys.h 384 2007-01-14 02:48:04Z goblinhack $
*/

#ifndef _GH_SUBSYS_H_
#define _GH_SUBSYS_H_

#include <gh_ui_event.h>
#include <gh_subsys.h>
#include <gh_sdl.h>

extern uint32 gh_time;

gh_inline void gh_set_time_milli (void)
{
    gh_time = gh_sdl_get_time_milli();
}

gh_inline uint32 gh_get_time_milli (void)
{
    return (gh_time);
}

gh_inline uint32 gh_get_time_milli_plus_tenths (uint32 tenths)
{
    return (gh_time + (tenths * 100));
}

gh_inline bool gh_have_x_tenths_passed_since (uint32 tenths, uint32 since)
{
    //
    // Cater for negative future times.
    //
    int32 delta = gh_time - since;

    return ((int32)(delta / 100) > (int32)tenths);
}

#endif /* _GH_SUBSYS_H_ */
