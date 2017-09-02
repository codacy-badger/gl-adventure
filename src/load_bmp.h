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

/** \file load_bmp.h
  * Defines functions to load BMP images and convert it to OpenGL textures
  *
  * Modifications :
  * - 28 nov 2008 : Modif for 64-bits CPU
  *
  */

#ifndef _XA_LOAD_BMP_H_
#define _XA_LOAD_BMP_H_

#include <GL/gl.h>

#include "def.h"

/** The structure used to load BMP image
  *
  *
  *
  */
typedef struct {
  int width;           //!< The width of the image
  int height;          //!< The height of the image
  unsigned char *data; //!< The image's data
} textureImage;

/** Storage for our textures 
  *
  */
GLuint  texture[3];  

int loadBMP(char *, textureImage *);
bool loadGLTextures();
bool load_one_textures(GLuint*, char* filename);

#endif // _XA_LOAD_BMP_H_
