##########################################
# sdlBasic compiler script for amiga os4 #
##########################################

rm sdlBasic
cd ../../src

gmake -f Makefile.os4 clean
gmake -f Makefile.os4
cp sdlBasic ../installer/os4

