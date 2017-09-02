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

/** \file fonts.c
  *
  * The implementation of fonts utilities.
  *
  */

#include "fonts.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logger.h"
#include "def.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext(String)

/** Creates and returns loaded fonts
  *
  * \param vDisplay The XLib display
  *
  */
fonts_t* load_fonts(Display* vDisplay){
#ifdef XA_DEBUG
  //  show_font_infos(vDisplay, "*-helvetica-*-25-*");

#endif // XA_DEBUG
  LOG(_("Loading fonts"));

  fonts_t* vFonts = (fonts_t*)malloc(sizeof(fonts_t));
  assert(vFonts && "Cannot allocated fonts memory");
  assert(vDisplay && "Display is invalid");

  LOG("  allocating ipn");
  vFonts->ipn = load_one_font(vDisplay, "-*-helvetica-bold-r-*-25-*");
  LOG("  ipn allocated");
  vFonts->ipv = load_one_font(vDisplay, "*-helvetica-*-12-*");
  LOG("  ipv allocated");
  vFonts->ipd = load_one_font(vDisplay, "*-lucida-medium-r-*-14-*");
  LOG("  ipd allocated");

  LOG("Fonts loaded");
  return vFonts;
}

/** Frees the memory allocated for fonts
  *
  * Each font is deallocated using the \c XFreeFont() function from XLib.
  * Do not use \c free(vFonts->ipn) after cause it complains about
  * double free memory corruption.
  *
  * \param vDis   The XLib display
  * \param vFonts The tont structur to be freed
  *
  */
void free_fonts(Display* vDis, fonts_t* vFonts){
  LOG("Freeing fonts");
  XUnloadFont(vDis, vFonts->ipn);
  XUnloadFont(vDis, vFonts->ipv);
  XUnloadFont(vDis, vFonts->ipd);
  
  free(vFonts);
}

/** Load the named font
  *
  * \param vDisplay  The XLib display
  * \param font_name The name of the font to load
  *
  * \return The loaded font
  *
  */
Font load_one_font( Display* vDisplay, char* font_name){
  LOGS("load_one_font called for font %s", font_name);
  Font fnt= XLoadFont(vDisplay, font_name);
  return fnt;
}

/** Draws an horizontal centered text
  *
  * Cannot use \ref graphics_t parameter here, let diasplay/window/cg ones.
  *
  * \param dis      The XLib display
  * \param win      The XLib window
  * \param xgc      The XLib Graphic Contest
  * \param font     The Xlib font used to draw
  * \param text     The text to draw
  * \param yPos     The Y position of the text (vertical position)
  *
  */
void center_text(Display* dis, Window win, GC xgc, Font font, 
		 char* text, int yPos)
{
  // Centering text
  XFontStruct* fs =XQueryFont(dis, font);
  int string_width = XTextWidth(fs, text, strlen(text));
  XFreeFontInfo(NULL, fs, 1);

  int x = (MWINW - string_width) / 2;
  LOG("Changing font");
  assert(font && "Invalid font pointer");
  XSetFont(dis, xgc, font);
  LOG("Drawing text");
  XDrawString(dis, win, xgc, x, yPos, text, strlen(text));
}

/** Draws a string
  *
  * \param dis      The XLib display
  * \param win      The XLib window
  * \param xgc      The XLib Graphic Contest
  * \param font     The Xlib font used to draw
  * \param text     The text to draw
  * \param xPos     The X position of the text (horizontal position)
  * \param yPos     The Y position of the text (vertical position)
  *
  */
void draw_text(Display*dis, Window win, GC xgc, Font font, char* text, 
	       int xPos, int yPos){

  assert(font && "Invalid font pointer");
  XSetFont(dis, xgc, font);
  XDrawString(dis, win, xgc, xPos, yPos, text, strlen(text));
}

/** Draws a string list
  *
  * \param dis      The XLib display
  * \param win      The XLib window
  * \param xgc      The XLib Graphic Contest
  * \param font     The Xlib font used to draw
  * \param sl       The string list to draw
  * \param xPos     The X position of the text (horizontal position)
  * \param yPos     The Y position of the text (vertical position)
  *
  */
void draw_string_list(Display* dis, Window win, GC xgc, 
		      Font font, string_list_t* sl, int xPos, int yPos)
{
  char* temp;
  int i;

  int y = yPos;


  for (i=0; i<sl->size; i++){
    temp = sl->data[i];
    draw_text(dis, win, xgc, font, temp, xPos, y);
    y += 20;
  }

}



/////////////////////////////////////////////
// Only DEBUG
#ifdef XA_DEBUG
  /** Logs the found font corresponding to the given pattern
    *
    * \param dis     The XLib display
    * \param pattern The pattern to search font for
    *
    */
  void show_font_infos(Display* dis, char* pattern){
    int count, i;
    char ** font_list;

    printf("Show font infos with pattern : %s\n", pattern);
    font_list=XListFonts(dis, pattern, 20, &count);

    for (i=0; i<count; i++){
      printf("  Font found : %s\n", font_list[i]);
    }
    
    XFreeFontNames(font_list);

  }
#endif // XA_DEBUG
