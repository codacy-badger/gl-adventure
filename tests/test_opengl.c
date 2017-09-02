#include "../def.h"
#include "../opengl.h"
#include "../logger.h"

#include <assert.h>
/* ascii code for the escape key */
#define ESCAPE 27

/* The number of our GLUT window */
int window; 


/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) 
    { 
	/* get rid of font display lists */
	kill_gl_font();

	/* shut down our window */
	glutDestroyWindow(window); 
	
	/* exit the program...normal termination. */
	exit(0);                   
    }
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
  glLoadIdentity();				// Reset The View

  

 gl_draw_dialog(0.8f, 0.8f, -2.0f);
 
 // move 1 unit into the screen.
 glTranslatef(-0.25f, 0.0f, -1.0f);


  // Pulsing Colors Based On Text Position
  glColor3f(1.0f, 0.0f, 0.0f);

  // Position The Text On The Screen
  glRasterPos2f(0,0.20);

  gl_print("OpenGL With NeHe");                  // print gl text to the screen.

  // since this is double buffered, swap the buffers to display what just got drawn.
  glutSwapBuffers();
}


int main(int argc, char** argv){
  
  glutInit(&argc, argv);  

  /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffer */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* get a 640 x 480 window */
  glutInitWindowSize(640, 480);  

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  window = glutCreateWindow("glAdventure v"XA_VERSION);  

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);  

  /* Go fullscreen.  This is as soon as possible. */
  glutFullScreen();

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  init_gl(640, 480);

  /* Start Event Processing Engine */  
  glutMainLoop();  

  return 0;
}
