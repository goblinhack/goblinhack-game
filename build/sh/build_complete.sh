#!/bin/sh

PLATFORM=$1
BASE=$3
TEMPORARY_TAR_FILE=$1.$$
VERSION=`cat build/etc/VERSION`

if [ "$PLATFORM" = "mingw" ]
then
	BINARY_DIST=$2-$VERSION-windows
	mkdir -p $BINARY_DIST

	cp src/ghack.exe $BINARY_DIST/ghack.exe
	strip $BINARY_DIST/ghack.exe

	cp src/curl.exe $BINARY_DIST/curl.exe
	strip $BINARY_DIST/curl.exe
else
	BINARY_DIST=$2-$VERSION-$PLATFORM
	mkdir -p $BINARY_DIST

	cp src/ghack $BINARY_DIST
	strip $BINARY_DIST/ghack
fi

do_copy () {
# echo "-- Found $1"
	cp $1 $2
	cp $1 src/
}

if [ "$BASE" != "" ]
then
	dirs="/usr/local/bin /usr/local/lib /mingw /mingw/bin /mingw/lib /usr /usr/bin /usr/lib /usr/X11R6/lib /usr/X11R6/bin /bin /lib"

	if [ "$PLATFORM" = "cygwin" ]
	then
		files="z cygjpeg6b jpeg SDL SDL_image SDL_mixer cygwin1 cygz"
		files="z cygwin1 cygz"
	elif [ "$PLATFORM" = "mingw" ]
	then
		files="SDL SDL_image SDL_mixer libcurl-4"
	else
		#
		# For all others just dump the libraries locally, just in case
		#
		files="z png jpeg SDL SDL_image SDL_mixer libcurl-4"
		files="z"
	fi

	for j in $files ; do

		found=0
#		echo "Looking for $j..."

		for i in $dirs ; do
			if test -r $i/$j.dll; then
				do_copy $i/$j.dll $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/lib$j.dll; then
				do_copy $i/lib$j.dll $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/$j.dll; then
				do_copy ${BASE}/$i/$j.dll $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/lib$j.dll; then
				do_copy ${BASE}/$i/lib$j.dll $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/$j.dll.a; then
				do_copy $i/$j.dll.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/lib$j.dll.a; then
				do_copy $i/lib$j.dll.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/lib$j.dll.a; then
				do_copy ${BASE}/$i/lib$j.dll.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/$j.dll.a; then
				do_copy ${BASE}/$i/$j.dll.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/$j; then
				do_copy $i/$j $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/$j.a; then
				do_copy $i/$j.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/$j; then
				do_copy ${BASE}/$i/$j $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/$j.a; then
				do_copy ${BASE}/$i/$j.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/lib$j; then
				do_copy $i/lib$j $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r $i/lib$j.a; then
				do_copy $i/lib$j.a $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/lib$j; then
				do_copy ${BASE}/$i/lib$j $BINARY_DIST;
				found=1;
				break;
			fi
			if test -r ${BASE}/$i/lib$j.a; then
				do_copy ${BASE}/$i/lib$j.a $BINARY_DIST;
				found=1;
				break;
			fi
		done

		if [ $found -eq 0 ]
		then
			echo "*** Didn't find $j ***"
		fi
	done
fi

FILES=`find src/resources/ -type f -a ! -regex ".*CVS.*" \
	-a ! -regex ".*\.o" \
	-a ! -regex ".*\.svn.*" \
	-a ! -regex ".* .*" \
	-a ! -regex "hiscores.gz" \
	-a ! -regex ".*.deleted.*" `

tar zcf $TEMPORARY_TAR_FILE.tgz $FILES README 
mv $TEMPORARY_TAR_FILE.tgz $BINARY_DIST

cd $BINARY_DIST
tar zxf $TEMPORARY_TAR_FILE.tgz
cd ..

/bin/rm $BINARY_DIST/$TEMPORARY_TAR_FILE.tgz
tar zcf $BINARY_DIST.tgz $BINARY_DIST

/bin/rm -rf installer/ghack*windows*
/bin/rm -rf installer/goblinhack

mv $BINARY_DIST* installer/

cd installer

if [ "$PLATFORM" = "mingw" ]
then
	cp -r $BINARY_DIST goblinhack

	if [ -x /c/Program\ Files/NSIS/makensis.exe ]
	then
		echo "-- /c/Program\ Files/NSIS/makensis.exe goblinhack-installer.nsi > nsis.log"
		/c/Program\ Files/NSIS/makensis.exe goblinhack-installer.nsi > nsis.log 

		if [ ! -x goblinhack-installer.exe ]
		then
		    echo "Failed in making the self extracting installer. Look at installer/nsis.log"
		fi

		mv goblinhack-installer.exe $BINARY_DIST.exe
		ls -la $BINARY_DIST.exe
	fi
fi

/bin/rm -rf $BINARY_DIST
/bin/rm -rf goblinhack

ls -la $BINARY_DIST.tgz

cd ..

/bin/rm -rf $TEMPORARY_TAR_FILE
