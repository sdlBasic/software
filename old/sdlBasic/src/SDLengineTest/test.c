//test SDLlengine
//test minimale


#include "../SDLengine.h"

int main(int argc,char **argv)
{
	double x;
	

	initialize(1,1);
	
	setdisplay(640,480,8,1);
	screenopen(0,2000,500,0,0,640,480,0);
	mousehide();
	loadimage("alien.gif",1);
	loadimage("bg.gif",2);
	//loadmusic("bladrmx.mod");
	loadsound("shot.wav",0);
	playmusic(1);

	blt(2,0,0,185,480,0,0);
	blt(2,0,0,185,480,185,0);
	blt(2,0,0,185,480,320,0);
	blt(2,0,0,185,480,320+185,0);

	text( 10,10,10,"ciao mondo");

	//pastebob(50,50,1);
	ink(rgb(0,0,0));
	circle (100,100,50);
	fillcircle(300,100,50);
	fillellipse (100,100,50,75);
	screenswap();
	x=key(27);
	while (x==0)
	{
		x=key(27);
		printf("  xmouse:%d",xmouse());
		printf("  ymouse:%d",ymouse());
		printf("  bmouse:%d",bmouse());
		if (bmouse())playsound(0,0,0);
		printf(" - 27:%d  \n",x);

		sprite(1,xmouse()-16,ymouse()-16,1);
		if ((xmouse())>600)offset(xoffset()+10,yoffset());
		if ((xmouse())<40)offset(xoffset()-10,yoffset());
		screenswap();
		
		}
	terminate();
	return 0;
}

