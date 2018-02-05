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

/** \file opengl.h
  * Defines opengl related functions
  * 
  *
  */

#ifndef _GLA_OPEN_GL_H_
#define _GLA_OPEN_GL_H_

#include "stringlist.h"
#include "def.h"

#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/glx.h>     // Header file fot the glx libraries.
#include <unistd.h>     // Header file for sleeping.
#include <math.h>       // Header file for sin/cos functions.
#include <stdio.h>      // Header file for printf debugging.

#include "gl_font.h"

/** The step used to increment/decrement \ref cycling_color */
#define CYCLING_COLOR_STEP 0.02f

/** The cycling color value */
float cycling_color;
/** The direction of the cycling color */
bool cycling_color_goes_up;

void init_gl(int, int);

GLvoid gl_print(struct glx_font_t, char *);
GLvoid gl_center_print(struct glx_font_t, char *, float );
GLvoid gl_print_string_list(struct glx_font_t,string_list_t*, float, float);
GLvoid gl_draw_dialog(float, float, float);

void gl_get_screen_pos(int x, int y);

void advance_cycling_color(void);

#endif // _GLA_OPEN_GL_H_
