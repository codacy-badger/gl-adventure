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

/** \file gl_window.h
  * Defines some GLX window related stuff
  *
  */

#ifndef _XA_GL_WINDOW_H_
#define _XA_GL_WINDOW_H_

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>

#include "gl_font.h"
#include "load_bmp.h"


/** stuff about our window grouped together 
  *
  */
typedef struct {
  Display *dpy;                  //!< The XLib display
  int screen;                    //!< The XLib screen id for given display
  Window win;                    //!< The XLib Window structure
  GLXContext ctx;                //!< The GLX context
  XSetWindowAttributes attr;     //!< The XLib Window's attributes
  Bool fs;                       //!< Full screen mode ?
  XF86VidModeModeInfo deskMode;  //!< The XF86 modes
  int x;                         //!< X Position of the window
  int y;                         //!< Y Position of the window
  unsigned int width;            //!< Width of the window
  unsigned int height;           //!< Height of the window
  unsigned int depth;            //!< The depth
} GLWindow;

/** attributes for a single buffered visual in RGBA format with at least
  * 4 bits per color and a 16 bit depth buffer 
  *
  */
static int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE, 4, 
    GLX_GREEN_SIZE, 4, 
    GLX_BLUE_SIZE, 4, 
    GLX_DEPTH_SIZE, 16,
    None};

/** attributes for a double buffered visual in RGBA format with at least
  * 4 bits per color and a 16 bit depth buffer 
  *
  */
static int attrListDbl[] = { GLX_RGBA, GLX_DOUBLEBUFFER, 
    GLX_RED_SIZE, 4, 
    GLX_GREEN_SIZE, 4, 
    GLX_BLUE_SIZE, 4, 
    GLX_DEPTH_SIZE, 16,
    None };


/** The global window attribute variable */
GLWindow GLWin;
/** The global font list */
glx_fonts_t fonts;

int initGL(GLvoid);
GLvoid killGLWindow(GLvoid);

GLvoid buildFont(GLvoid);
Bool createGLWindow(char*, int, int, int, Bool);
void resizeGLScene(unsigned int, unsigned int);
GLvoid deleteFont(GLvoid);

#endif // _XA_GL_WINDOW_H_
