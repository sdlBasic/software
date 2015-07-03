#####################################
# sdlBasic compiler script for fbsd #
#####################################

rm sdlBasic
cd ../../src

gmake -f Makefile.fbsd clean
gmake -f Makefile.fbsd
c[21~p sdlBasic ../installer/fbsd

