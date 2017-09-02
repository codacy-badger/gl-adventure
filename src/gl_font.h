/*
 *  Copyright 2008-2010 Jerome PASQUIER
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

/** \file gl_font.h
  * Defines font structure to be used with GLX
  *
  *
  */

#ifndef _XA_GL_FONT_H_
#define _XA_GL_FONT_H_

#include <GL/gl.h>
#include <X11/Xlib.h>

/** Defines a GLX font
  *
  * We must get the XFontStruct value to be able to get the text width the
  * with XTextWidth function.
  *
  */
struct glx_font_t{
  XFontStruct* x;  //!< The XLib structure
  GLuint       gl; //!< The Opengl index of the font    
};

/** All fonts available
  *
  */
typedef struct{
  struct glx_font_t h1; //!< The biggest header font
  struct glx_font_t h2; //!< The biggest header font
  struct glx_font_t p;  //!< The font used for paragraphs
}glx_fonts_t;


void init_font(struct glx_font_t*, Display*, char*);
void free_font(struct glx_font_t*, Display*);


#endif // _XA_GL_FONT_H_
