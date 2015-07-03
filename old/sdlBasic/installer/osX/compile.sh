#############################################
# sdlBasic compiler script for MacOSX V1.00 #
#############################################

cd ../../src

make -f Makefile.osx clean
make -f Makefile.osx
cp sdlBasic ../installer/osX

