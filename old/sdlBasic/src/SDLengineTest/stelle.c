// stelle.c
// make using SDLengine

#include "../SDLengine.h"
#include <stdio.h>
int rnd(int upper)
{
    return (rand() % (upper-1))+1;
}

char *str(int i)
{
    char *txt;
    txt=(char *)malloc(16);
    sprintf(txt,"%d",i);
    return txt;

}


int main(int argc,char **argv)
{
    #define numstars 2500
    int x[numstars],y[numstars],v[numstars];
	int i;
	char *titolo;

	int serversock,clientsock;
	char *tmp;
    initialize(1,1);
    serversock=opensock(1024);
    fps(1);
    setdisplay(800,600,32,1);
	setcaption("le stelle");
	caption(&titolo);

    directscreen();
    autoback(0);

    for(i=0;i<numstars;i++){
	x[i]=rnd(800);
	y[i]=rnd(600);
	v[i]=rnd(9)+1;
	}

    ink(rgb(255,255,255));
	clientsock=0;
    while(key(27)==0){

	if (clientsock==0){
	    clientsock=acceptsock(serversock);
	}
	else
	{
	    if (isclientready(clientsock)!=0){
		tmp = readsock(clientsock,1024);
		printf("%s\n",tmp);
		if (tmp[0]=='Q' )exit(1);
	    }
	}
	cls();
	screenlock(0);
	for(i=0;i<numstars;i++){
	    x[i]-=v[i];
	    if (x[i]<0)x[i]+=800;
	    dot(x[i],y[i]);
	}
	screenunlock(0);
	locate(0,0);
	prints(str(fps(-1)));
	locate(10,0);
	prints(titolo);

	//SDL_SetGamma(255,0,0);
	screenswap();
    }
    closesock(1);

    return 0;
}
