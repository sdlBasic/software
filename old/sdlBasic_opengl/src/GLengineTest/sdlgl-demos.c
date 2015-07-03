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
#include <GL/glut.h>

#include "../GLengine.h"
#include "../SDLengine.h"



int main(int argc,char *argv[])
{
       GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    int i;
    int x,y,z;
    float angle;
    static GLint fogMode;

    initialize();
    GLinit();

    loadimage("sdlBasic.png",1);
    loadimage("sdlnow.gif",2);
    copyimage (2,3);
    //zoomimage(3,0.2,0.2);

    GLsetdisplay(800,600,32,1);
    //GLscreen2D(0);
    //directscreen();
    paper(rgb(255,255,255));
	   cls();




    //screenopen(7,800,600,0,0,800,600,2);
    //pastebob(530,50,1);
    //screen(0);
    //curson();
    //prints("sdlBasic now work on opengl");
    //box(100,100,560,560);
    //ink(rgb(0,255,255));
    //bar (250,230,550,550);
    //screen(7);
    //ink(rgb(0,0,255));
    //text(360,260,20,"ciao");
    //screen(0);

    GLmaketexture(1,0);
    GLmaketexture(2,0);
    GLmaketexture(3,1);


    //angle=0.0;

    //GLvertex( 1, 10, 20, -20);
    //GLvertex( 2,-10, 20, -20);
    //GLvertex( 3,-20,-20, -20);
    //GLvertex( 4, 20,-20, -20);

    while( inkey()==0 ) {

   GLfloat position1[] = { 0.5,0.5, 3.0, 0.0 };



//*
   glEnable(GL_DEPTH_TEST);

//*
   glLightfv(GL_LIGHT0, GL_POSITION, position1);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
/*
   {
      GLfloat mat[3] = {0.1745, 0.01175, 0.01175};
      glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
      mat[0] = 1.61424; mat[1] = 1.04136; mat[2] = 1.04136;
      glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
      mat[0] = 1.727811; mat[1] = 1.626959; mat[2] = 1.626959;
      glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
      glMaterialf (GL_FRONT, GL_SHININESS, 1.6*128.0);
   }
//*/








    GLtexture(3);

    //GLmakesurfacefv( 1, 2, 3, 4);




	glTranslatef( 0.0, 0.0, -7.0);

		

    glRotatef( 1, 0, 0, 0 );

    glColor3f(255,255,255);
    GLtexture(1);
    GLmakecubef( 0, 0,-2, 1);

    GLtexture(2);
    GLmakecubef( 0, 0, 2, 1);

    GLtexture(1);
    //GLtexture(0);
    //glColor3f(255,0,0);
    //glutSolidSphere(1.0, 20, 20);
    glutSolidTeapot(1.0);


    //glTranslatef (1.25, 0.0, 0.0);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    //glutSolidSphere(1.0, 16, 16);



/*

	GLtexture(1);

	glBegin(GL_QUADS);
	// Front Face
	glNormal3f( 0.0f, 0.0f, 1.0f);// Normal Pointing Towards Viewer
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);// Point 1 (Front)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);// Point 2 (Front)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);// Point 3 (Front)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);// Point 4 (Front)
	// Back Face
	glNormal3f( 0.0f, 0.0f,-1.0f);// Normal Pointing Away From Viewer
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// Point 1 (Back)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);// Point 2 (Back)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);// Point 3 (Back)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);// Point 4 (Back)
	// Top Face
	glNormal3f( 0.0f, 1.0f, 0.0f);// Normal Pointing Up
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);// Point 1 (Top)
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);// Point 2 (Top)
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);// Point 3 (Top)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);// Point 4 (Top)
	glEnd();

	GLtexture(2);

	glBegin(GL_QUADS);
	// Bottom Face
	glNormal3f( 0.0f,-1.0f, 0.0f);// Normal Pointing Down
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// Point 1 (Bottom)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);// Point 2 (Bottom)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);// Point 3 (Bottom)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);// Point 4 (Bottom)
	// Right face
	glNormal3f( 1.0f, 0.0f, 0.0f);// Normal Pointing Right
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);// Point 1 (Right)
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);// Point 2 (Right)
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);// Point 3 (Right)
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);// Point 4 (Right)
	// Left Face
	glNormal3f(-1.0f, 0.0f, 0.0f);// Normal Pointing Left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// Point 1 (Left)
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);// Point 2 (Left)
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);// Point 3 (Left)
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);// Point 4 (Left)
	glEnd();
//*/


	//sprite(1,290,angle*2,2);
	glRotatef( angle, 1.0, 1.0, 1.0 );
    if( ++angle > 360.0f )angle = 0.0f;

	GLscreenswap();
    }
    GLterminate();
    terminate();
    return 0;
}

