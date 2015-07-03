######################################
# sdlBasic compiler script for linux #
######################################

rm sdlBasic
cd ../../src

make -f Makefile.linux clean
make -f Makefile.linux
cp sdlBasic ../installer/linux

