/*
    Name:       datafunction.h
    Purpose:    data function for sdlBasic
    Author:     __vroby__
    Copyright:  (c) 2004 Viola Roberto <__vroby__@libero.it>
    Licence:    LGPL
*/

extern char *dataarray[10000];
extern int dataenter,datapointer;

void init_data();
void loaddata(char *datavalue);
char *readdata();
