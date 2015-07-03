

for run sdlBasic you must install
sdl libraries

sdl 1.2 or later
sdl_image
sdl_mixer
sdl_ttf
sdl_net
smpeg

in normal installation of linux there are all library.
in redHat(now fedora) mandrake and suse you can use rpm utility for installation
if missing. In Debian base (like knoppix)you can use apt-get install or dselect.

for the compilation you must install the devel packages.

In Mandrake 10.1 are in official disk. If missing you must download from a ftp
mirror source (ex ftp://ftp.belnet.be).


In linux i had found many problems concerning the change of resolution and
performance. The speed is subject at the quality of graphics driver.
If you have a nvidia card the best way is the official driver nvidia.
The open driver are more slow but works. If you want tested opengl
version (warning it's very unstable) there are no chance of use with
open driver (at this time).

WARNING: if not install sdlBasic it not work because needed the DejaVu fonts 
in fonts path!!

Another know problems it's that in debian are missing the unversion simbolic 
links at the version libraries. sdlBasic need that in /usr/lib there are 
libSDL_image.so libSDL_mixer.so libSDL_ttf.so and libSDL_net.so 
if missing simple make a symbolic link of installed version.

Many Tanks to Zoiba and Giulio Canevari for extensive testing support and suggestions.


For all problems concerning sdlBasic please send me a mail (__vroby__@libero.it)
or write in site forum (http://sdlbasic.sf.net).

					__vroby__
