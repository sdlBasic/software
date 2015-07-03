@path %PATH%;c:\programmi\gnuwin32\bin
bison -y sdlBasic_tab.y
del y_tab.c
ren y.tab.c y_tab.c
