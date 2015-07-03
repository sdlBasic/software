/*
    Name:       dir.h
    Purpose:    dir support for wxBasic/sdlBasic
    Author:     Viola Roberto __vroby__
    Copyright:  (c) 2003 Viola Roberto <__vroby__@libero.it>
    Licence:    LGPL
*/


 char *dir();
 char *dirfirst(char *path);
 char *dirnext();
 int direxists(char *path);
 int isdir(char *path);
 int filecopy(char *source,char *dest);
 int filemove(char *source,char *dest);
 int filerename(char *source,char *dest);

