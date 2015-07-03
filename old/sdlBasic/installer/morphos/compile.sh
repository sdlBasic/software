########################################
# sdlBasic compiler script for morphos #
########################################

rm sdlBasic
cd ../../src

gmake -f Makefile.morphos clean
gmake -f Makefile.morphos
cp sdlBasic ../installer/morphos

