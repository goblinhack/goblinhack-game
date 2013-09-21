#!/bin/sh
#
#   ghack.
#   Copyright (C) 1999-2010 Neil McGill
#
#   This GAME is free software; you can redistribute it and/or
#   modify it under the terms of the GNU Library General Public
#   License as published by the Free Software Foundation; either
#   version 2 of the License, or (at your option) any later version.
#
#   This GAME is distributed in the hope that it will be fun,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   Library General Public License for more details.
#
#   You should have received a copy of the GNU Library General Public
#   License along with this GAME; if not, write to the Free
#   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#   Neil McGill
#
#	$Id: install.sh 1185 2009-05-30 17:38:15Z Goblinhack $
#

SRC=$1
DST=$2

GAME=ghack

/bin/rm -rf $DST/resources/things
/bin/rm -rf $DST/resources/rooms
/bin/rm -rf $DST/resources/
mkdir -p $DST
cp $SRC/$GAME $DST
cp $SRC/src/curl.exe $DST

FILES=`cd src/resources; find . -type f -a ! -regex ".*CVS.*" \
	-a ! -regex ".*\.o" \
	-a ! -regex ".*\.svn.*" \
	-a ! -regex ".* .*" \
	-a ! -regex "hiscores.gz" \
	-a ! -regex ".*.deleted.*" | sed 's/^\.\///g'`

for i in $FILES
do
	if [ ! -d `dirname $DST/resources/$i` ]
	then
		mkdir -p `dirname $DST/resources/$i`
	fi

	cp src/resources/$i $DST/resources/$i
done

cp installer/*.ico $DST

mkdir -p $DST/resources/saved
chmod a+rw $DST
chmod a+rw $DST/resources
chmod a+rw $DST/resources/saved
chmod a+rw $DST/resources/levels
chmod a+rw $DST/resources/levels/random
chmod a+rw $DST/resources/levels/random/hiscores*

echo
echo "All done! You can execute the following"
echo "        $DST/$GAME"
echo

# We're done!
exit 0

