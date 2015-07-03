cd ..
path c:\devel\c-cpp\borland\bcc55\bin;%PATH%
bcc32 -g255 -I"C:\devel\c-cpp\visual cpp\SDL\INCLUDE" -c SDLengine.c
bcc32 -g255 -I"C:\devel\c-cpp\visual cpp\SDL\INCLUDE" -c sdlBasic.c
brc32  Script1.rc
ilink32 -L"C:\devel\c-cpp\visual cpp\SDL\LIB"  sdlBasic.obj SDLengine.obj ,sdlBasic.exe,,SDLmain.lib SDL.lib SDL_image.lib  SDL_mixer.lib SDL_ttf.lib SDL_net.lib
