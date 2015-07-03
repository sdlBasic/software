set prg=stelle
path c:\devel\c-cpp\mingw\bin;%PATH%
cd ..
gcc -g -O2 -Ic:/devel/c-cpp/mingw/include -c ../SDLengine.c
gcc -g -O2 -Ic:/devel/c-cpp/mingw/include -c ../inflate.c
gcc -g -O2 -Ic:/devel/c-cpp/mingw/include -c ../unzip.c
gcc -g -O2 -Ic:/devel/c-cpp/mingw/include -c %prg%.c
gcc -g -O2 -Ic:/devel/c-cpp/mingw/include -o %prg%.exe %prg%.o inflate.o unzip.o SDLengine.o -lmingw32 -lSDLmain -lSDL -lSDL_image  -lSDL_mixer -lSDL_ttf -lSDL_net -lsmpeg
strip %prg%.exe

