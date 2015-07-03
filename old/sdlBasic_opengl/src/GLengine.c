/*
_________________________________________________________________________________________________________________________

    GLengine opengl support  of sdlBasic

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    __vroby__
    __vroby__@libero.it

_________________________________________________________________________________________________________________________
*/

#include <stdio.h>
#include <stdlib.h>

#ifndef UNIX
	#include <windows.h>
#endif

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "SDLengine.h"

//error print
#define SDLerr fprintf



    const SDL_VideoInfo* info = NULL;
    int	SDLscreenBack;
    int width;
    int height;
    float ratio;

    int flag_screen2D;


    GLuint textur[1024];
    int    w,h;
    char *data;


    double GLcamera_x;
    double GLcamera_y;
    double GLcamera_z;

    double GLcamera_angx;
    double GLcamera_angy;
    double GLcamera_angz;





    GLfloat GL_vertex[4096][3];
//____________________________________________________________________________________________________________________________________
//
//external
//____________________________________________________________________________________________________________________________________

void GLinit()
{
    /* First, initialize SDL's video subsystem. */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        exit(2);
	}

    /* Let's get some video information. */
    info = SDL_GetVideoInfo( );

    if( !info ) {
        /* This should probably never happen. */
        fprintf( stderr, "Video query failed: %s\n",
             SDL_GetError( ) );
        exit(2);
    }
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);


}
//____________________________________________________________________________________________________________________________________

void GLterminate()
{
    SDL_Quit();
    exit(2);
}
//____________________________________________________________________________________________________________________________________
//
// screen
//____________________________________________________________________________________________________________________________________

// GLsetdisplay(w,h,bpp,mode)	: open the sdldisplay in opengl blit mode (0 full screen | 1 windows 2 resizable
int GLsetdisplay(int w,int h,int bpp,int mode)
{
	int flags;

    width = w;
    height = h;
    //bpp = info->vfmt->BitsPerPixel;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    switch(mode){
	case 0:
	    flags = SDL_OPENGLBLIT | SDL_FULLSCREEN |SDL_ANYFORMAT;//SDL_OPENGL|
	    break;
	case 1:
	    flags = SDL_OPENGLBLIT | SDL_ANYFORMAT  ;//SDL_OPENGL|
	    break;
	case 2:
	    flags = SDL_OPENGLBLIT | SDL_RESIZABLE|SDL_ANYFORMAT;//SDL_OPENGL|
	    break;
	default:
	    fprintf( stderr, "Video mode not valid %s\n",
             SDL_GetError( ) );
	    exit( 2 );
		break;

    }

     SDLdisplay= SDL_SetVideoMode( width, height, bpp, flags );
    if( SDLdisplay == 0 ) {
	 fprintf( stderr, "Video mode set failed: %s\n",
             SDL_GetError( ) );
			exit( 2 );
    }

	SDL_FillRect(SDLdisplay,NULL,0);

	switch (SDLdisplay->format->BytesPerPixel){
	    case 1:
			getpixel=getpixel8;
			putpixel=putpixel8;
			xrgb=xrgb8;
			rrgb=rrgb8;
			break;
	    case 2:
			getpixel=getpixel16;
			putpixel=putpixel16;
			xrgb=xrgb16;
			rrgb=rrgb16;
			break;
	    case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
				getpixel=getpixel24BE;
				putpixel=putpixel24BE;
				xrgb=xrgb24;
				rrgb=rrgb24;
			}
			else{
				getpixel=getpixel24;
				putpixel=putpixel24;
				xrgb=xrgb24;
				rrgb=rrgb24;
			}
			break;
	    case 4:
			getpixel=getpixel32;
			putpixel=putpixel32;
			xrgb=xrgb32;
			rrgb=rrgb32;
			break;
	    default:
			SDLerr(stderr, "SDLengine:video bpp not  valid: %d\n",bpp);
			return -1;
			break;
	}



	screenopen(0,SDLdisplay->w,SDLdisplay->h,0,0,SDLdisplay->w,SDLdisplay->h,2);
	dualplayfield(1);
	SDLscreenBack=0;
	autoback(0);
	spriteclip(0,0,SDLdisplay->w,SDLdisplay->h);


    flag_screen2D=1;

    ratio = (float) width / (float) height;

    glShadeModel( GL_SMOOTH );

    /* Culling. */
    glEnable( GL_CULL_FACE );
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    /* Set the clear color. */
    glClearColor( 0, 0, 0, 0 );




    /* Setup our viewport. */
    glViewport( 0, 0, width, height );

    //glMatrixMode( GL_PROJECTION );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    gluPerspective( 100.0, ratio, 1.0, 1024.0 );

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    return 0;

}
//____________________________________________________________________________________________________________________________________

// GLscreenswap()			: swap the buffers of display
int GLscreenswap()
{
    SDL_Rect s;

    if (flag_screen2D==1){
	if (SDLscreen_live[7]==1){
	    SDL_FillRect(SDLdisplay,NULL,paper_color);
	    s.x=SDLoffx[7];
	    s.y=SDLoffy[7];
	    s.w=SDLscreen_rect[7].w;
	    s.h=SDLscreen_rect[7].h;
	    SDL_BlitSurface( SDLscreen[7], &s, SDLdisplay, &SDLscreen_rect[7]);
	    SDL_Flip(SDLdisplay);

	    SDLscreen_live[7]=2;
	}
    }
    SDL_GL_SwapBuffers( );
    glLoadIdentity( );
    gluPerspective( 100.0, ratio, 1.0, 1024.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);


    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    if (flag_screen2D==1){
	screenswap();
	if (SDLscreen_live[7]==2)SDLscreen_live[7]=1;
    }
    return 0;
}
//____________________________________________________________________________________________________________________________________

// GLscreen2D(m)			: enable(1) disable(0) the updating of 2d sdl screen in GLdisplay
int GLscreen2D(int m)
{
    if (m==-1)
	return flag_screen2D;
    else
	flag_screen2D=m;
    return 0;
}
//____________________________________________________________________________________________________________________________________

//*GLscreenrect (x,y,w,h)		: define the glviewport
int GLscreenrect( int x, int y, int w, int h)
{
    /* Setup our viewport. */
    glViewport( x, y, w, h );
    return 0;
}

//____________________________________________________________________________________________________________________________________
//
// GLcamera
//____________________________________________________________________________________________________________________________________

// *GLcameraposition(x,y,z)
int GLcameraposition(double x,double y,double z)
{

    GLcamera_x=-x;
    GLcamera_y=-y;
    GLcamera_z=-z;

    glTranslatef( -x, -y, -z);
    return 0;
}



//____________________________________________________________________________________________________________________________________

// *GLcameraangle(x,y,z)
int GLcameraangle(double x,double y,double z)
{
    GLcamera_angx=x;
    GLcamera_angy=y;
    GLcamera_angz=z;

    glTranslatef( -GLcamera_x, -GLcamera_y, -GLcamera_z);
    glRotatef(y,1,0,0);
    glRotatef(x,0,1,0);
    glRotatef(z,0,0,1);
    glTranslatef( GLcamera_x, GLcamera_y, GLcamera_z);
    return 0;
}
//____________________________________________________________________________________________________________________________________

// *GLcameralook(x,y,z)
int GLcameralook(double x,double y,double z)
{
    gluLookAt(GLcamera_x, GLcamera_y, GLcamera_z,0,0,0,-x,-y,-z);
    return 0;
}
//____________________________________________________________________________________________________________________________________

//____________________________________________________________________________________________________________________________________
//
// GLligth
//____________________________________________________________________________________________________________________________________

// *GLmakelight(n,x,y,z,r,g,b,pow)
//____________________________________________________________________________________________________________________________________

//____________________________________________________________________________________________________________________________________
//
// GLtextures
//____________________________________________________________________________________________________________________________________

// *GLloadtexture(file,n)
int GLloadtexture(char *file,int n)
{
    SDL_Surface *temp;

    temp=Load_Image(file,1);
    glGenTextures(1,&textur[n]);
    w=temp->w;
    h=temp->h;
    data=temp->pixels;
    glBindTexture(GL_TEXTURE_2D,textur[n]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    return 0;
}
//____________________________________________________________________________________________________________________________________

// GLmaketexture(n)
int GLmaketexture(int n,int m)
{

    glGenTextures(1,&textur[n]);
    w=imagewidth(n);
    h=imageheight(n);
    data=SDLimage[n]->pixels;

    glBindTexture(GL_TEXTURE_2D,textur[n]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);
    if (m==0)glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (m==0)glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (m==0)glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    if (m==1)glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    if (m==1)glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (m==1)glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    return 0;
}
//____________________________________________________________________________________________________________________________________

// GLtexture(n)
int GLtexture(int n)
{
    glBindTexture(GL_TEXTURE_2D,textur[n]);
}
//____________________________________________________________________________________________________________________________________

//____________________________________________________________________________________________________________________________________
//
// GLgraphics
//____________________________________________________________________________________________________________________________________

// GLcolor(r,g,b)
//____________________________________________________________________________________________________________________________________


//____________________________________________________________________________________________________________________________________
//
// GLobject
//____________________________________________________________________________________________________________________________________

// Glmakecube(x,y,z,size)
int GLmakecubef( double x, double y, double z, double l)
{
    GLfloat x1,y1,z1;
    GLfloat x2,y2,z2;

    x1=x-(l/2);
    y1=y-(l/2);
    z1=z-(l/2);

    x2=x+(l/2);
    y2=y+(l/2);
    z2=z+(l/2);

	glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);// Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x1,  y2,  z2);// Point 1 (Front)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x2,  y2,  z2);// Point 2 (Front)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x2, y1,  z2);// Point 3 (Front)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x1, y1,  z2);// Point 4 (Front)
	// Back Face
	glNormal3f( 0.0f, 0.0f,-1.0f);// Normal Pointing Away From Viewer
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x2, y1, z1);// Point 1 (Back)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x2, y2, z1);// Point 2 (Back)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x1, y2, z1);// Point 3 (Back)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x1, y1, z1);// Point 4 (Back)
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);// Normal Pointing Up
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x2, y2, z1);// Point 1 (Top)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x2, y2, z2);// Point 2 (Top)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x1, y2, z2);// Point 3 (Top)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x1, y2, z1);// Point 4 (Top)
	// Bottom Face
	glNormal3f( 0.0f,-1.0f, 0.0f);// Normal Pointing Down
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x1, y1, z2);// Point 1 (Bottom)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x2, y1, z2);// Point 2 (Bottom)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x2, y1, z1);// Point 3 (Bottom)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x1, y1, z1);// Point 4 (Bottom)
	// Right face
	glNormal3f( 1.0f, 0.0f, 0.0f);// Normal Pointing Right
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x2, y1, z2);// Point 1 (Right)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x2, y2, z2);// Point 2 (Right)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x2, y2, z1);// Point 3 (Right)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x2, y1, z1);// Point 4 (Right)
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);// Normal Pointing Left
	glTexCoord2f(0.0f, 1.0f); glVertex3f( x1, y2, z1);// Point 1 (Left)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( x1, y2, z2);// Point 2 (Left)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( x1, y1, z2);// Point 3 (Left)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( x1, y1, z1);// Point 4 (Left)
	//
	glEnd();

    return 0;
}
//____________________________________________________________________________________________________________________________________

//Glmakesphere(x,y,z,size)
int GLmakesphere(double x,double y,double z,double size)
{
    glTranslatef(x,y,z);
    glutSolidSphere(size,40,40);
    glTranslatef(-x,-y,-z);
    return 0;
}
//____________________________________________________________________________________________________________________________________


//____________________________________________________________________________________________________________________________________
//
// GLmatrix
//____________________________________________________________________________________________________________________________________

// GLvertex (n,x,y,z)
int GLvertex( int n, double x, double y, double z)
{
   GL_vertex[n][0]=x;
   GL_vertex[n][1]=y;
   GL_vertex[n][2]=z;
   return 0;
}
//____________________________________________________________________________________________________________________________________

// GLmakesurface(v1,v2,v3,v4)
int GLmakesurfacefv( int v1, int v2, int v3, int v4 )
{
	glBegin(GL_QUADS);
	glTexCoord2f( 1, 0); glVertex3fv(GL_vertex[v1]);
	glTexCoord2f( 0, 0); glVertex3fv(GL_vertex[v2]);
	glTexCoord2f( 0, 1); glVertex3fv(GL_vertex[v3]);
	glTexCoord2f( 1, 1); glVertex3fv(GL_vertex[v4]);
	glEnd();
    return 0;
}
//____________________________________________________________________________________________________________________________________

//____________________________________________________________________________________________________________________________________

//____________________________________________________________________________________________________________________________________
//
// GLlowlevel
//____________________________________________________________________________________________________________________________________

//GLbegin(type)
//____________________________________________________________________________________________________________________________________

//GLend
//____________________________________________________________________________________________________________________________________

//GLnormal(x,y,z)
//____________________________________________________________________________________________________________________________________

//GLtexcoord(x,y)
//____________________________________________________________________________________________________________________________________

// GLsetvertex(v OR x,y,z)
int GLsetvertexfv(int v)
{
    glVertex3fv(GL_vertex[v]);
    return 0;
}
//____________________________________________________________________________________________________________________________________

//GLpushmatrix
//____________________________________________________________________________________________________________________________________

//GLpopmatrix
//____________________________________________________________________________________________________________________________________

//GLflush
//____________________________________________________________________________________________________________________________________

































