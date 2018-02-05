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

/** \file colors.h
  *
  * Defines colors related struct and function
  *
  */

#ifndef _COLORS_H_
#define _COLORS_H_

#include <X11/Xlib.h>

/** Defines a shortcut for unsigned short */
typedef unsigned short ushort;

/** Convert 0 to 255 value to 0 to 65535 value
  *
  *
  */
#define TO_X(v) ((v*65535)/255)

/** \def DEBUG_VISUAL(v)
  *
  * This define can be use instead of a call to the \ref debug_visual()
  * function to avoid conditional compilation from the caller. This
  * define calls the \ref debug_visual() in debug mode and do nothing
  * in release mode.
  *
  * \param v The visual to debug
  *
  * \sa \ref debug_visual()
  *
  */
#ifdef XA_DEBUG
#  define DEBUG_VISUAL(v) (debug_visual(v))
#else
#  define DEBUG_VISUAL(v) //
#endif // XA_DEBUG


/** A structure that define commonly used colors
  *
  * If you use XColor typed color, use it as follow
  * <code>c.red.pixel</code>.
  *
  */
typedef struct {
  XColor black;      //!< The black color
  XColor white;      //!< The white color
  XColor red;        //!< The red color
  XColor blue;       //!< The blue color
  XColor green;      //!< The green color
  XColor grey;       //!< The light grey color
  XColor light_grey; //!< The light grey color
  XColor brown;      //!< The brown color
}colors_t;

/** The color map object used to allocate XColor */
Colormap colormap;

colors_t* init_colors(Display*, Window);
void free_colors(colors_t*, Display*);

int allocate_RGB(XColor*, Display*, ushort, ushort, ushort);

#ifdef XA_DEBUG
void debug_visual(Visual*);
#endif // XA_DEBUG

#endif // _COLORS_H_
