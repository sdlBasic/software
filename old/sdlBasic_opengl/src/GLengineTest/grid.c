/*
 * SDL OpenGL Tutorial.
 * (c) Michael Vance, 2000
 * briareos@lokigames.com
 *
 * Distributed under terms of the LGPL.
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
	#include <windows.h>
#endif

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GLengine.h"


int main()
{
    int x,y,z;
    GLinit();

    GLsetdisplay(1024,768,8,0);
	glTranslatef( 0.0, 0.0, -25.0);

    while( 0==0 ) {
		ProcessEvents();
		glColor3d(255,0,0);
		glBegin(GL_LINES);
		for (z=-10;z<11;z++){
		    glVertex3f(-10,z,-10);
		    glVertex3f(10,z,-10);
		}

		for (x=-10;x<11;x++){
			glVertex3f(x,10,-10);
			glVertex3f(x,-10,-10);
		}
		glEnd();


		glRotatef(0.1,0,1,0);


		GLscreenswap();
    }
    GLterminate();
    return 0;
}

