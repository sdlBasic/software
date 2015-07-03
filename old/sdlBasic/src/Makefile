###############################
#  sdlBasic Makefile v1.01    #
###############################

exec_prefix=/usr/local

SRC = SDLengine.c sdlBasic.c
OBJECT=SDLengine.o sdlBasic.o
TARGET= sdlBasic

CC= gcc
CFLAG= -g -O2 -I/usr/include/SDL -D_REENTRAT -DPLAY_MOD -DUNIX -DLINUX
LIBS=-lSDL_mixer -lSDL_image -lSDL_ttf -lSDL_net
COMPILE=$(CC) $(CFLAG) $(LIBS)


all: 		SDLengine.o sdlBasic.o
	$(COMPILE) -o$(TARGET) $(OBJECT)
	strip $(TARGET)
	gzexe $(TARGET)

install:	all
	/usr/bin/install -c -m 755 -d ${exec_prefix}/bin
	/usr/bin/install -c sdlBasic ${exec_prefix}/bin/sdlBasic
	/usr/bin/install -c -m 755 -d /usr/share/fonts/ttf/vera
	/usr/bin/install  ../bin/shared/VeraMono.ttf /usr/share/fonts/ttf/vera/VeraMono.ttf
	/usr/bin/install  ../bin/shared/Vera.ttf /usr/share/fonts/ttf/vera/Vera.ttf

SDLengine.o:	SDLengine.c
	$(CC) $(CFLAG) -c SDLengine.c

sdlBasic.o:	sdlBasic.c
	$(CC) $(CFLAG) -c sdlBasic.c

clean:
	rm *.o
	rm $(TARGET)
