
@echo now compiling sdlBasic: please wait
@cd ..

rem WARNING!!! verify the correct path of mingGw
set MINGWPATH=c:\devel\c-cpp\mingw

@path %MINGWPATH%\bin;%PATH%

gcc -g -O2 -I%MINGWPATH%/include -c SDLengine.c
ld -L%MINGWPATH%/lib -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lSDL_net -lsmpeg -a sdlEngine.o

