#####################################
#  sdlBasic Installer netbsd v1.11  #
#####################################
exec_prefix=/usr/local

chmod +x sdlBasic
gzexe sdlBasic
rm sdlBasic~
/usr/bin/install -c -m 755 -d ${exec_prefix}/bin
/usr/bin/install -c sdlBasic ${exec_prefix}/bin/sdlBasic
/usr/bin/install -c -m 755 -d /usr/share/fonts/ttf/dejavu
/usr/bin/install  ../../shared/DejaVuSans.ttf /usr/share/fonts/ttf/dejavu/DejaVuSans.ttf
/usr/bin/install  ../../shared/DejaVuSansMono-Roman.ttf /usr/share/fonts/ttf/dejavu/DejaVuSansMono-Roman.ttf
