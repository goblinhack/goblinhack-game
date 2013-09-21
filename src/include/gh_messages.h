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

    $Id: gh_messages.h 308 2006-12-03 22:49:12Z goblinhack $
*/

#ifndef _GH_MESSAGES_H_
#define _GH_MESSAGES_H_

class gh_game_message
{
public:
    gh_game_message(void) { }
    gh_game_message(const string message);
    ~gh_game_message(void) { }

    uint32              created_when;
    uint8               count;
    string              message;
};

#endif /* _GH_MESSAGES_H_ */
