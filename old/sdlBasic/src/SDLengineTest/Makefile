
prg=stelle

SRC = ../SDLengine.c ../inflate.c ../unzip.c $(prg).c
#test.c stelle.c
OBJECT=SDLengine.o inflate.o unzip.o $(prg).o
#test.0
TARGET= $(prg)
#test

CC= gcc
CFLAG=-g -O2 -I/usr/include/SDL -D_REENTRAT -DPLAY_MOD -DUNIX
COMPILE=$(CC) $(CFLAG)
LIBS= -lSDL -lSDL_mixer -lSDL_image -lSDL_ttf -lSDL_net -lsmpeg

all:
	$(COMPILE) $(SRC) $(LIBS) -o$(TARGET)
	strip $(TARGET)
	gzexe $(TARGET)


clean:
	rm $(TARGET)
	rm *.o
