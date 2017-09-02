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

/** \file gshelp.c
  * Implementation of multiple help viewer state
  * 
  * Modifications :
  * - 22 nov 2008 : Help strings upate
  *
  */
#include "gshelp.h"

/** Get the XLib keys */
#define XK_MISCELLANY
#  include <X11/keysymdef.h> // For keysym

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "logger.h"
#include "gslist.h"
#include "gl_window.h"
#include "load_bmp.h"

#include "def.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext (String)

/** Initialize the help viewer
  *
  * \return The game state containing the help viewer
  *
  */
game_state_t* init_gs_help(void){
  int i;

  game_state_t* gs = new_game_state("GS1 (Help)", &draw_gs_help);
  gs->key_list = add_binding(gs->key_list, XK_Escape, &back_to_intro);
  gs->key_list = add_binding(gs->key_list, XK_Up, &help_page_up);
  gs->key_list = add_binding(gs->key_list, XK_Down, &help_page_down);

  sl_index = create_string_list();
  string_list_add( sl_index, _("Introduction"));
  string_list_add( sl_index, _("Concept"));
  string_list_add( sl_index, _("Key bindings"));
  string_list_add( sl_index, _("Page 4"));
  string_list_add( sl_index, _("Page 5"));

  sl_content = malloc(sizeof(string_list_t) * NB_PAGE);

  for (i=0; i<NB_PAGE; i++){
    sl_content[i] = create_string_list();
  }

  P1(_("You're in the glAdventure help viewer. It is a little browser"));
  P1(_("that help you to understand how the program should work."));
  P1("");
  P1(_("You can see in the upper right corner a light grey box containing"));
  P1(_("the index of the help. Feel free to use UP and DOWN arrow keys"));
  P1(_("to navigate in this index. You can also use the ESC key to exit"));
  P1(_("the help viewer."));

  P2(_("glAdventure intends to reach some goals :"));
  P2(_("- learning the C language;"));
  P2(_("- learning to use OpenGL;"));
  P2(_("- create a very small Role Playing Game;"));
  P2("");
  P2(_("For these reasons, glAdventure is a very small RPG, built in C")); 
  P2(_("and using OpenGL. glAdventure requires only the OpenGL software"));
  P2(_("library."));

  P3(_("This help page present in game key bindings :"));
  P3("");
  P3(_("In map mode :"));
  P3KB(_("ARROWS - Move the character in the map."));
  P3KB(_("ESC - Go back to the introduction."));
  P3KB(_("I - Opens the inventory dialog."));
  P3KB(_("C - Opens/Close the character dialog."));
  P3KB(_("S - Opens/Close the spells dialog."));
#ifdef XA_DEBUG
  P3KB(_("D - Toggles between different debug modes."));
  P3KB(_("L - Log player informations in console."));
#endif // XA_DEBUG
  P3("");
  P3(_("In fight mode:"));
 
  DEBUG_STRING_LIST(sl_index);
  return gs;
}

/** Free the given help viewer
  *
  * \param gs The help viewer to free
  *
  */
void free_gs_help(game_state_t* gs){
  int i;

  for (i=0; i<NB_PAGE; i++){
    free_string_list(sl_content[i]);
  }

  free(sl_content);

  free_string_list(sl_index);

  free_game_state(gs);
}

/** The help viewer draw() callback
  *
  * \return Always 0
  *
  */
int draw_gs_help(void){
  // Drawing dialog
  glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, texture[0]);   /* select our  texture */
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( -0.9f, -0.7f, -2.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( -0.9f, 0.7f, -2.0f);	
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 0.9f, 0.7f, -2.0f);	
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 0.9f, -0.7f, -2.0f);	
  glEnd();			
  glBindTexture(GL_TEXTURE_2D, 0);
  
  // Drawing ttle
  glColor3f(1.0f, 0.0f, 0.0f);
  glTranslatef(0.0f, 0.27f, 0.0);
  gl_center_print( fonts.h1, _("Help viewer"), 0.0f);      

  glTranslatef(-0.4f, -0.1f, -1.0);

  // Draws page content if no-NULL
  if (sl_content[current_page]){
    gl_print_string_list( fonts.p, sl_content[current_page], 0.0, 0.0);
  }

  // Draw page number
  char page_number[20];
  sprintf(page_number, "Page %d/%d", current_page+1, NB_PAGE);
  glRasterPos2f(0.7f, -0.48f);
  gl_print( fonts.p, page_number);

  return 0;
}

/** The callback used to go back to introduction game state
  *
  * \return The change state value with GS_INTRO as parameter
  *
  */
int back_to_intro(void){
  return ((GS_INTRO << GS_CHST_BIT_SHIFT) +6);
}

/** The PAGE UP callback
  *
  * \return MUST REDRAW
  *
  */
int help_page_up(void){
  LOG("help_page_up");
  current_page--;
  if (current_page < 0 )
    current_page = (sl_index->size - 1);
  return 2; // must redraw
}

/** The PAGE DOWN callback
  *
  * \return MUST REDRAW
  *
  */
int help_page_down(void){
  LOG("help_page_down");

  assert(current_page <= NB_PAGE - 1  && "current page out of bound. "
	 "Please check the NB_PAGE macro value.");

  assert(sl_content[current_page] && "Current is NULL, please check content.");

  current_page++;
  if (current_page > (sl_index->size - 1))
    current_page = 0;

  return 2; // must redraw
}
