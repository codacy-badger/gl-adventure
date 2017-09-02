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

/** \file gl_font.c
  * Implements font function to be used with GLX
  *
  *
  */

#include "gl_font.h"

#include <assert.h>

/** Load a named font
  *
  * \param font The object where the font is stored
  * \param dis  The XLib display
  * \param name The name of the font to load
  *
  */
void init_font(struct glx_font_t* font, Display* dis, char* name){
  /* storage for 96 characters */
  font->gl = glGenLists(96);
  /* load a font with a specific name in "Host Portable Character Encoding" */
  font->x = XLoadQueryFont(dis, name);
    if ( font->x== NULL)
    {
        /* this really *should* be available on every X Window System...*/
        font->x = XLoadQueryFont(dis, "fixed");
        assert(font->x && "Problems loading fonts :-(\n");
    }
    /* build 96 display lists out of our font starting at char 32 */
    glXUseXFont(font->x->fid, 32, 96, font->gl);
}

/** Destroy a font
  *
  * \param font The font object
  * \param dis  The XLin display
  *
  */
void free_font(struct glx_font_t* font, Display* dis){
  XFreeFont(dis, font->x);
  glDeleteLists(font->gl, 96);
}
