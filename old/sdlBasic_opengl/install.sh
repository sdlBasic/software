###############################
#  sdlBasic Installer v1.00   #
###############################
exec_prefix=/usr/local

/usr/bin/install -c -m 755 -d ${exec_prefix}/bin
/usr/bin/install -c sdlBasic ${exec_prefix}/bin/sdlBasic
/usr/bin/install -c -m 755 -d /usr/share/fonts/ttf/vera
/usr/bin/install  VeraMono.ttf /usr/share/fonts/ttf/vera/VeraMono.ttf
/usr/bin/install  Vera.ttf /usr/share/fonts/ttf/vera/Vera.ttf

