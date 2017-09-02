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

/** \file graphics.h
  * Declares a structure that encapsulates all graphic objects
  *
  *
  */

#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "colors.h"
#include "fonts.h"

////////////////////////////////////// GRAPHICS OBJECTS
/** Get the display of the graph */
#define _DIS (vGraph->dis)

/** Get the graphic context of the graph */
#define _XGC (vGraph->xgc)

/** Get the window of the graph */
#define _XGC (vGraph->xgc)

/** Get the window of the graph */
#define _WIN (vGraph->win)

////////////////////////////////////// COLORS
/** Get the light grey color */
#define _LGR (vGraph->col->light_grey.pixel)

/** Get the red color */
#define _RED (vGraph->col->red.pixel)

////////////////////////////////////// FONTS
/** Get the font used to draw project name */
#define _IPN (vGraph->fnt->ipn)

/** Group all objects needed to draw
  *
  * This structure does not need to be malloc'd and you don't find
  * a new_* function here. It is directly created in \ref main().
  *
  */
typedef struct{
  Display*   dis; //!< The XLib display
  Window     win; //!< The Xlib window
  GC         xgc; //!< The Xlib graphic context
  colors_t*   col; //!< The declared colors
  fonts_t*   fnt; //!< All the fonts used in application
}graphics_t;

#endif //_GRAPHICS_H_
