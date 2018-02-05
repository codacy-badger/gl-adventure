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

/** \file gsintro.c
  * Implementation of the introduction game state
  *
  */
#include "gsintro.h"

/** Get the XLib keys */
#define XK_MISCELLANY
#  include <X11/keysymdef.h> // For keysym

#include <stdio.h>
#include <string.h>

#include "logger.h"
#include "def.h"

#include "gl_window.h"
#include "gslist.h"
#include "opengl.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext(String)

/** Creates and return an introduction game state
  *
  * \return The new state
  *
  */
game_state_t* init_gs_intro(void){
  LOG("Initializing intro game state");
  intro_current_menu = 0;

  game_state_t* gs = new_game_state("GS0 (Intro)", &draw_gs_intro);
  gs->key_list = add_binding(gs->key_list, XK_Escape, &exit_gs_intro);
  gs->key_list = add_binding(gs->key_list, XK_Return, &intro_enter_pressed);
  gs->key_list = add_binding(gs->key_list, XK_Up, &intro_move_up);
  gs->key_list = add_binding(gs->key_list, XK_Down, &intro_move_down);
  return gs;
}

/** Free the memory allocated 
  *
  * \param gs The introduction game state to free
  *
  */
void free_gs_intro(game_state_t* gs){
  free_game_state(gs);
}

/** Draws the intro game state
  *
  */
int draw_gs_intro(void){

  glPushMatrix();
  gl_draw_dialog(1.4f, 1.0f, -2.0f);
  glColor3f(1.0f, 1.0f, 1.0f);
 

  // Position The Text On The Screen
  glColor3f(1.0f, 0.0f, 0.0f);
  gl_center_print( fonts.h1, "glAdventure", 0.17f);        
    
  handle_intro_menu_color(0);
  gl_center_print(fonts.h2, _("Play"), 0.0f);

  handle_intro_menu_color(1);
  gl_center_print( fonts.h2, _("Help") , -0.05f);        

  handle_intro_menu_color(2);
  gl_center_print( fonts.h2, _("Exit"), -0.15);        

  glPopMatrix();

  return 0;
}

/** Exits the game
  *
  * \return The MUST_QUIT value
  *
  */
int exit_gs_intro(void){
  LOG("gs_intro::exit called");
  return 1; // MUST QUIT
}

/** Go to help viewer
  *
  * \return The change state value with gshelp as parameter
  *
  */
int change_to_help(void){
  LOG("change_to_help called");
  return ((GS_HELP << GS_CHST_BIT_SHIFT) +6);
}

/** Go to the game gamestate
  *
  * \return The value needed to change state and passing \ref GS_GAME as
  *         parameter
  *
  */
int change_to_game(void){
  LOG("change_to_game called");
  return ((GS_GAME << GS_CHST_BIT_SHIFT) +6);

}

/** Moves to the previous menu item
  *
  * It changes to \c intro_current_menu-- with a out of bound checking.
  *
  * \return always 0.
  *
  */
int intro_move_up(void){
  intro_current_menu--;
  if (intro_current_menu < 0){
    intro_current_menu = INTRO_LAST_MENU;
  }
  return 0;
}

/** Moves to the next menu item
  *
  * It changes to \c intro_current_menu++ with a out of bound checking.
  *
  * \return always 0.
  *
  */
int intro_move_down(void){
  intro_current_menu++;
  if (intro_current_menu > INTRO_LAST_MENU){
    intro_current_menu = 0;
  }
  return 0;
}

/** Handle the menu item color
  *
  * Set a standard color for all items but the one with same index
  * as \ref intro_current_menu. This one takes a cycling color
  * designed by \ref cycling_color.
  *
  * \param id The id of the menu we are currently drawing
  *
  */
void handle_intro_menu_color(short id){
  if (id == intro_current_menu){
    glColor3f(cycling_color, 0.0f, 1.0f);
  }
  else{
    glColor3f(1.0f, 0.0f, 1.0f); 
  }
}

/** The enter key was pressed in the intro game state
  *
  * \return A change state value according to the \ref intro_current_menu
  *         value.
  *
  */
int intro_enter_pressed(void){
  switch(intro_current_menu){
  case 0:
    return change_to_game();

  case 1:
    return change_to_help();

  case 2:
    return exit_gs_intro();
  }
}
