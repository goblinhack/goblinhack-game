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

    $Id: gh_ui_thing.h 276 2006-11-20 21:16:05Z goblinhack $
*/

#ifndef _GH_TERM_THING_H_
#define _GH_TERM_THING_H_

extern void *gh_ui_thing_init(class gh_thing *thing);

extern void gh_ui_thing_fini(void *context);

extern void gh_ui_thing_render(class gh_thing *thing,
                                 const gh_point p);

extern void gh_ui_thing_get_size(class gh_thing *thing,
                                   int *w, int *h);

extern void gh_ui_thing_render_light_source(class gh_thing *thing,
                                              const gh_point p);

extern bool gh_ui_thing_collision(class gh_thing *a,
                                  class gh_thing *b);

extern bool gh_ui_thing_can_touch(class gh_thing *a,
                                  class gh_thing *b);

extern bool gh_ui_thing_biting(class gh_thing *a,
                                 class gh_thing *b);

extern void gh_ui_thing_make_can_see_list(class gh_thing *thing);

#endif /* _GH_TERM_THING_H_ */
