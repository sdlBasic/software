i haven't macOsx and can't verify if work
Still i think there are no problems.

WARNING: if not install sdlBasic it not work because needed the DejaVu fonts in fonts path!!

for work you must had installed all sdl library.
the library needed are

sdl 1.2 or later
sdl_image
sdl_mixer
sdl_ttf
sdl_net
smpeg

for compiling remember to install all devel package for sdl library

Many tank's at Tim Douglas for very good collaboration!!!

This is the Tim's istruction for compiling and installing sdlBasic on mac OSX

How to get sdlBasic working on Mac OS X
First of all, download and install SDL, SDL_mixer, SDL_image, SDL_ttf, and SDL_net from libsdl.org.
Make sure to get the source and compile it yourself, else the UNIX versions of the libraries won't be installed
and sdlBasic won't compile.
Make sure to follow the very helpful assistance at osxgamepatches.sf.net; If you don't, a couple important
dependencies won't be installed.
Follow all the directions for installing the libraries except for the last one, SDL_gfx.
SDL_net, not listed on that site, is just a normal "./configure ; make ; sudo make install" affair.
You could always use Fink to install them too, but I have no idea whether or not that would work.

Next, download the newest snapshot of the sdlBasic source from http://sdlbasic.sf.net
(I used 2004-07-24, called "sdlBasic.tar.gz"), and put this patch in the same folder as the tarball.
Then follow these steps on the command line:

tar xvzf sdlBasic.tar.gz
patch -p0 <sdlBasic-OSX.patch
cd sdlBasic/src/
make
sudo make install
:::enter root/admin password, then hit enter:::

The make install is very important; without it, the fonts will not be installed,
and sdlBasic will crash when it can't find them.
So with all of this, sdlBasic should be installed and working!
Test it out with "sdlBasic clickme.sdlbas" (while in the sdlBasic root folder).


For all problems concerning sdlBasic please send me a mail (__vroby__@libero.it)
or write in forum (http://sdlbasic.sf.net)


last time:
in this version is missing the executable because my version is too much old.
Please if you had compiled succesfull the binary please send me a copy.
Tank you !!!!

					    __vroby__
