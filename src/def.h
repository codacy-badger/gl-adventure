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

/** \file def.h
  * Global definitions for xAdventure
  *
  *
  */

#ifndef _DEF_H_
#define _DEF_H_

// Includes for gettext
#include <libintl.h>
#include <locale.h>

/** The version of the program */
#define XA_PROGNAME "glAdventure"

/** The version of the program */
#define XA_VERSION "0.1.3-39"

/** \def NULL
  * Defines NULL macro
  *
  * NULL is sometimes undefined
  *
  */
#ifndef NULL
# define NULL 0
#endif // NULL

/** defines booleans 
  *
  * C standard do not define it.
  *
  */
#ifdef XA_BOOL
  typedef enum {
    false = 0,
    true  = 1
  }bool;
#endif

/** Defines the max lenght in character of FILE:LINE concatenation in logger
  *
  */
#define LOG_CONCAT_SIZE "16"



/** Used with XLib t create display and graphic context */
#define NIL (0)

/** The main window width */
#define MWINW 800
/** The main window hight */
#define MWINH 600

/** The width of the external border of the gui */
#define GUIXB 20

/** The value of the bitshift for change state 
  *
  * This value is used to bit shift the index of the new state when 
  * changing state.
  *
  */
#define GS_CHST_BIT_SHIFT 3

#endif //_DEF_H_
