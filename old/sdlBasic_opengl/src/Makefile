###############################
#  sdlBasic Makefile v1.00    #
###############################

exec_prefix=/usr/local

SRC = GLengine.c SDLengine.c sdlBasic.c
OBJECT=GLengine.o SDLengine.o sdlBasic.o
TARGET= sdlBasic

CC= gcc
CFLAG= -g -O2 -I/usr/include/SDL -D_REENTRAT -DPLAY_MOD -DUNIX 
LIBS= -lSDL_mixer -lSDL_image -lSDL_ttf -lGL -lGLU -lglut
COMPILE=$(CC) $(CFLAG) $(LIBS)


all: 		GLengine.o SDLengine.o sdlBasic.o
	$(COMPILE) -o$(TARGET) $(OBJECT)
	strip $(TARGET)

install:	all
	/usr/bin/install -c -m 755 -d ${exec_prefix}/bin
	/usr/bin/install -c sdlBasic ${exec_prefix}/bin/sdlBasic
	/usr/bin/install -c -m 755 -d /usr/share/fonts/ttf/vera
	/usr/bin/install  VeraMono.ttf /usr/share/fonts/ttf/vera/VeraMono.ttf
	/usr/bin/install  Vera.ttf /usr/share/fonts/ttf/vera/Vera.ttf

GLengine.o:	GLengine.c
	$(CC) $(CFLAG) -c GLengine.c

SDLengine.o:	SDLengine.c
	$(CC) $(CFLAG) -c SDLengine.c

sdlBasic.o:	sdlBasic.c
	$(CC) $(CFLAG) -c sdlBasic.c

clean:
	rm *.o
	rm $(TARGET)
