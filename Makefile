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
#   $Id: Makefile.in 1236 2009-07-06 21:45:34Z Goblinhack $
#

TOP=.

include $(TOP)/build/mk/config

SUBDIRS=build bin src lib
CLOBBER_SUBDIRS=src

include $(TOP)/build/mk/rules

depend::
	(cd $(TOP)/src; make depend)

clobber:: clean
	(cd $(TOP)/build/mk; $(RM) -rf config library program rules)
	(cd $(TOP)/build; $(RM) -rf autom4te.cache)
	$(RM) src/ghack*
	$(RM) ghack*
	$(RM) *dll
	$(RM) *exe
	$(RM) */*exe
	$(RM) */*dll
	$(RM) build/mk/config

clean::
	$(RM) stdout*
	$(RM) stderr*
	$(RM) */stdout*
	$(RM) */stderr*
	$(RM) */*/stdout*
	$(RM) */*/stderr*
	$(RM) src/resources/game.gz
	$(RM) src/resources/saved/*
	$(RM) src/resources/levels/*/hiscore*
	$(RM) src/resources/levels/*/*/hiscore*
	$(RM) src/ghack*
	$(RM) installer/ghack*tgz
	$(RM) installer/*exe

install::
	build/sh/install.sh . /usr/local/games/ghack

installer::
	sh build/sh/builddist.sh
