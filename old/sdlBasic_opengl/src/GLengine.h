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

#ifdef WIN32
	#include <windows.h>
#endif

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


extern const SDL_VideoInfo* info;
extern     int	SDLscreenBack;
extern    int width ;
extern    int height ;
extern    float ratio;

extern    int flag_screen2D;

extern    GLuint textur[1024];
extern    int    w,h;
extern    char * data;

extern    double GLcamera_x;
extern    double GLcamera_y;
extern    double GLcamera_z;

extern    double GLcamera_angx;
extern    double GLcamera_angy;
extern	  double GLcamera_angz;


extern    GLfloat GL_vertex[4096][3];

#ifdef __cplusplus
extern "C" {
#endif

//____________________________________________________________________________________________________________________________________
// GLexternal
void GLinit();
void GLterminate();



//-- GLscreen -------------------------------------------------------------------------------------------------------------

// GLsetdisplay(w,h,bpp,mode)	: open the sdldisplay in opengl blit mode (0 full screen | 1 windows 2 resizable
int GLsetdisplay(int w,int h,int bpp,int mode);

// GLscreenswap()			: swap the buffers of display
int GLscreenswap();

// GLscreen2D(m)			: enable(1) disable(0) the updating of 2d sdl screen in GLdisplay
int GLscreen2D(int m);

// *GLscreenrect (x,y,w,h)		: define the glviewport
int GLscreenrect( int x, int y, int w, int h);

//-- GLcamera -------------------------------------------------------------------------------------------------------------

// *GLcameraposition(x,y,z)
int GLcameraposition(double x,double y,double z);

// *GLcameraangle(x,y,z)
int GLcameraangle(double x,double y,double z);

// *GLcameralook(x,y,z)
int GLcameralook(double x,double y,double z);
//-- GLlight --------------------------------------------------------------------------------------------------------------

// *GLmakelight(n,x,y,z,r,g,b,pow)

//-- GLgraphics ----------------------------------------------------------------------------------------------------------

// GLcolor(r,g,b)




//-- GLtexture ------------------------------------------------------------------------------------------------------------

// *GLloadtexture(file,n)
int GLloadtexture(char *file,int n);
// GLmaketexture(n)
int GLmaketexture(int n, int m);

// GLtexture(n)
int GLtexture(int n);

//-- GLobject -------------------------------------------------------------------------------------------------------------

// Glmakecube(x,y,z,size)
int GLmakecubef( double x, double y, double z, double l);

// Glmakesphere(x,y,z,size)
int GLmakesphere(double x, double y, double z, double size);

//-- GLmatrix -------------------------------------------------------------------------------------------------------------

// GLvertex (n,x,y,z)
int GLvertex( int n, double x, double y, double z);

// GLmakesurface(v1,v2,v3,v4)
int GLmakesurfacefv( int v1, int v2, int v3, int v4 );

//-- GLlowlevel -----------------------------------------------------------------------------------------------------------

// GLbegin(type)

// GLend

// GLnormal(x,y,z)

// GLtexcoord(x,y)

// GLsetvertex(v OR x,y,z)
int GLsetvertexfv(int v);

// GLpushmatrix

// GLpopmatrix

// GLflush

#ifdef __cplusplus
    }
#endif

