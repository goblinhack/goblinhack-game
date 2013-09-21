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
#	$Id: shell.sh 1212 2009-06-22 22:20:50Z Goblinhack $
#

. $TOP/build/sh/colour.sh

setenv() {
	eval "$1='$2'"; export $1;
}

log() {
	echo "${GH_LOG_ON}ghack: $*${GH_LOG_OFF}"
}

bright() {
	echo "${ERR_ON}$*${ERR_OFF}"
}

errlog() {
	echo "${ERR_ON}ghack: ERROR: $*${ERR_OFF}"
}

prompt() {
	line="`echo '12'`"
	if [ "$line" = '12' ]; then
		echo -n "${GH_LOG_ON}$*${GH_LOG_OFF}"
	else
		echo "${GH_LOG_ON}$*\c${GH_LOG_OFF}"
	fi
}

version() {
	cat $TOP/build/etc/VERSION
	uname -a
	g++ --version
}

buglog() {
	echo "********************************************************************************"

	version

	errlog "$*: "
	errlog "$*: Please report this to goblinhack@gmail.com"
	errlog "$*: "
	errlog "$*: The goblin responsible SHALL BE PUNISHED!"
	errlog "$*: "
	errlog "$*: You could try this and see if an update exists:"
	errlog "$*:     svn co https://goblinhack.svn.sourceforge.net/svnroot/goblinhack/trunk goblinhack"
	errlog "$*: "

	echo "********************************************************************************"

	return 1
}
