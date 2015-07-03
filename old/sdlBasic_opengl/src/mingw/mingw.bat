
@echo now compiling sdlBasic: please wait
@cd ..

rem WARNING!!! verify the correct path of mingGw
set MINGWPATH=c:\devel\c-cpp\mingw

@path %MINGWPATH%\bin;%PATH%

del *.o
del sdlBasic.exe
gcc -Wall -g -O2 -I%MINGWPATH%/include -c SDLengine.c
gcc -Wall -g -O2 -I%MINGWPATH%/include -c sdlBasic.c
res2coff -i Script1.res -o script1.o
gcc -Wall -g -O2 -I%MINGWPATH%/include -o sdlBasic.exe sdlBasic.o SDLengine.o Script1.o -lmingw32 -lSDLmain -lSDL -lSDL_image  -lSDL_mixer -lSDL_ttf -lSDL_net -lsmpeg
strip sdlBasic.exe

rem direct install sdlBasic
copy sdlbasic.exe c:\windows
@echo compilation complete
