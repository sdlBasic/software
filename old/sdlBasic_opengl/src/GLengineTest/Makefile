


SRC = ../GLengine.c ../SDLengine.c sdlgl-demos.c
OBJECT= ../sdlgl-demos.o ../SDLengine.o GLengine.o
TARGET= sdlgl-demos

CC= gcc
CFLAG=-g -O2 -I/usr/include/SDL -D_REENTRAT -DPLAY_MOD -DUNIX
COMPILE=$(CC) $(CFLAG)
LIBS= -lSDL_mixer -lSDL_image -lSDL_ttf -lGL -lGLU -lglut

all:
	$(COMPILE) $(SRC) $(LIBS) -o$(TARGET)


clean:
	rm $(TARGET)
	rm *.o
