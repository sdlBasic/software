#######################################
# sdlBasic compiler script for netbsd #
#######################################

rm sdlBasic
cd ../../src

gmake -f Makefile.netbsd clean
gmake -f Makefile.netbsd
cp sdlBasic ../installer/netbsd

