#!/bin/sh
#
#   ghack.
#   Copyright (C) 1999-2010 Neil McGill
#
#   This game is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Library General Public
#   License as published by the Free Software Foundation; either
#   version 2 of the License, or (at your option) any later version.
#
#   This game is distributed in the hope that it will be fun,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Library General Public License for more details.
#
#   You should have received a copy of the GNU Library General Public
#   License along with this game; if not, write to the Free
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#   Neil McGill
#
#   $Id: builddist.sh 3 2006-08-25 14:46:56Z goblinhack $
#

case `uname` in
MINGW*)
	build/sh/build_complete.sh mingw ghack /usr/local
	;;
CYGWIN*)
	build/sh/build_complete.sh cygwin ghack /usr/local
	;;
*)
	build/sh/build_complete.sh linux ghack
	;;
esac
