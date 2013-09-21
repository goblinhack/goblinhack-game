#!/bin/sh

/bin/rm -f Makefile.bak

../contrib/mingw/makedepend -Y./include \
    *.cpp 				\
    */*.cpp 				\
    */*/*.cpp 				\
    */*/*/*.cpp 			\
    */*/*/*/*.cpp 			\
    */*/*/*/*/*.cpp 			\

/bin/rm -f Makefile.bak

makedepend -Y./include        		\
    *.cpp 				\
    */*.cpp 				\
    */*/*.cpp 				\
    */*/*/*.cpp 			\
    */*/*/*/*.cpp 			\
    */*/*/*/*/*.cpp 			\

/bin/rm -f Makefile.bak

cat Makefile | sed 's/\.obj/\.o/g' > Makefile.bak
mv -f Makefile.bak Makefile
