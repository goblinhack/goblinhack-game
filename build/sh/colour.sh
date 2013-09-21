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
#   $Id: colour.sh 1212 2009-06-22 22:20:50Z Goblinhack $
#

#
# File that can read by shell or make
#
ESC=""

DIRECTORY_GH_LOG=35
COLOUR_GH_LOG=32
COLOUR_ERR=31

GH_LOG_ON="${ESC}[${COLOUR_GH_LOG}m"
GH_LOG_OFF="${ESC}[m"

ERR_ON="${ESC}[${COLOUR_ERR}m"
ERR_OFF="${ESC}[m"

DIR_ON="${ESC}[${DIRECTORY_GH_LOG}m"
DIR_OFF="${ESC}[m"
