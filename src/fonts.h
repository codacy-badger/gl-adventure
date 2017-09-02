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

/** \file fonts.h
  *
  * The header file declaring fonts utilities.
  *
  */

#ifndef _FONTS_H_
#define _FONTS_H_

#include <X11/Xlib.h>

#include "stringlist.h"

/** A structure that contains all the fonts used in game
  *
  * Whan setting a font, call this :
  * <code>XSetFont(dis, xgc, font->fid);</code>
  *
  * \c fid is a member of the \c XFontStruct structure and it is needed.
  *
  */
typedef struct{
  Font ipn; //!< The intro project name font
  Font ipv; //!< The intro project version
  Font ipd; //!< The intro project description
}fonts_t;

#include "graphics.h" // Must be included after fonts_t declaration

fonts_t* load_fonts(Display*);
void     free_fonts(Display*, fonts_t*);

Font load_one_font( Display*, char*);
void center_text(Display*, Window, GC, Font, char*, int);
void draw_text(Display*, Window, GC, Font, char*, int, int);
void draw_string_list(Display*, Window, GC, Font, string_list_t*, int, int);

#ifdef XA_DEBUG
void show_font_infos(Display*, char*);
#endif // XA_DEBUG

#endif // _FONTS_H_
