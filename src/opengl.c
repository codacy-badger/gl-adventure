/*
 *  Copyright 2008-2010, 2018 Jerome PASQUIER
 * 
 *  This file is part of glAdventure - A little game using XGL.
 *
 *  glAdventure is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  glAdventure is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with glAdventure; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 *  02110-1301  USA
 *
 */

/** \file opengl.c
  * Implements opengl related functions
  * 
  *
  */


#include "opengl.h"

#include <assert.h>
#include <string.h>

#include "gl_window.h"

/** A general OpenGL initialization function.  
  *
  * Sets all of the initial parameters. 
  * We call this right after our OpenGL window is created.
  *
  * \param width, height The dimensions of the window
  *
  */
void init_gl(int width, int height){
  // This Will Clear The Background Color To Black
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		
  // Enables Clearing Of The Depth Buffer
  glClearDepth(1.0);				  
  // The Type Of Depth Test To Do
  glDepthFunc(GL_LESS);				 
  // Enables Depth Testing
  glEnable(GL_DEPTH_TEST);			
  // Enables Smooth Color Shading
  glShadeModel(GL_SMOOTH);

  // Reset The Projection Matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Calculate The Aspect Ratio Of The Window
  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);

}

/** Prints a centered text in the screen
  *
  * Please use the OpenGl \c glTranslatef() function to position
  * your text before calling this.
  *
  * \param font The font used to draw
  * \param text The text to draw
  *
  */
GLvoid gl_print(struct glx_font_t font, char *text){
  // if there's no text, do nothing.
  if (text == NULL) { 
    return;
  }
    
  // alert that we're about to offset the display lists with glListBase
  glPushAttrib(GL_LIST_BIT);   
  // sets the base character to 32.
  glListBase(font.gl - 32);

  // draws the display list text.
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);   
  // undoes the glPushAttrib(GL_LIST_BIT);
  glPopAttrib();                              
}

/** Draws a cntered dialog's background
  *
  * \param w, h Size of the dialog in OpenGl dimensions
  * \param z    The Z position in OpenGl value
  *
  */
GLvoid gl_draw_dialog(float w, float h, float z){
  float x1 = (0.0f - (w/2.0f));
  float x2 = (0.0f + (w/2.0f));
  float y1 = (0.0f - (h/2.0f));
  float y2 = (0.0f + (h/2.0f));

  glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, texture[0]);   /* select our  texture */
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( x1, y1, z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( x1, y2, z);	
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( x2, y2, z);	
    glTexCoord2f(1.0f, 0.0f);
   glVertex3f( x2, y1, z);	
  glEnd();			
  glBindTexture(GL_TEXTURE_2D, 0);
}

/** Change the cycling color value
  *
  * This function must be call for each frame. It change the
  * value of \ref cycling_color according to \ref cycling_color_goes_up
  * and \ref CYCLING_COLOR_STEP.
  *
  */
void advance_cycling_color(void){
  if (cycling_color_goes_up){
    cycling_color += CYCLING_COLOR_STEP;
    if (cycling_color >= 1.0f){
      cycling_color = 1.0f;
      cycling_color_goes_up = false;
    }
  }
  else{
    cycling_color -= CYCLING_COLOR_STEP;
    if (cycling_color <= 0.0f){
      cycling_color = 0.0f;
      cycling_color_goes_up = true;
    }

  }
  
}

/** Prints a centered text in the screen
  *
  * \param font The font used to draw
  * \param text The text to draw
  * \param y    The Y position in OpenGl coordonate system
  *
  */
GLvoid gl_center_print(struct glx_font_t font, char *text, float y){
  float w = (float)XTextWidth(font.x, text, strlen(text));
  //  float ww = (float)GLWin.width;
  float wf = w/(float)GLWin.width;
  float x = 0.0f - (wf / 2.0f);

  glPushMatrix();
  glTranslatef(0.0f, 0.0f, -1.0f);

  glRasterPos2f(x, y);
  gl_print(font, text);

  glPopMatrix();
}

/** Print the given string list at the given position
  *
  * The string list cannot be NULL.
  *
  * \param font The font used to draw
  * \param sl   The string list to be drawn
  * \param x, y The position of the text
  *
  */
GLvoid gl_print_string_list(struct glx_font_t font, string_list_t* sl, 
			    float x, float y){

  assert(sl && "Cannot draw NULL stringlist");

  size_t i;
  for (i=0; i<sl->size; i++){
    glRasterPos2f(x, y);
    gl_print(font, sl->data[i]);
    y -= 0.022;
  }

}

/** Print OpenGL position from a screen point
  *
  * \param x, y The screen positions in pixels
  *
  */
void gl_get_screen_pos(int x, int y){
  GLint viewport[4];
  GLdouble modelview[16];
  GLdouble projection[16];
  GLfloat winX, winY, winZ;
  GLdouble posX, posY, posZ;
  
  glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
  glGetDoublev( GL_PROJECTION_MATRIX, projection );
  glGetIntegerv( GL_VIEWPORT, viewport );
  
  winX = (float)x;
  winY = (float)viewport[3] - (float)y;
  glReadPixels( x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
  
  gluUnProject( winX, winY, winZ, modelview, projection, viewport, 
		&posX, &posY, &posZ);
  
  printf("GetOGLPos :%lf, %lf, %lf\n", posX, posY, posZ);
}
