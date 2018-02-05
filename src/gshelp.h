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

/** \file gshelp.h
  * Header file for a multiple pages help viewer
  *
  */

#ifndef _GS_GAME_H_
#define _GS_GAME_H_

#include "gamestate.h"
#include "stringlist.h"

/** Adds a string to the first page */
#define P1(x) (string_list_add( sl_content[0], x))

/** Adds a string to the second page */
#define P2(x) (string_list_add( sl_content[1], x))

/** Adds a string to the third page */
#define P3(x) (string_list_add( sl_content[2], x))

/** Indent the text used to explain key bindings*/
#define KB_INDENT "       "

/** Adds a key binding help */
//#define P3KB(text) P3(KB_INDENT""+text);
#define P3KB(text) P3(text);

/** The number of pages of help
  *
  * This value is used in \ref help_page_down() to test the integrity
  * of the help.
  *
  */
#define NB_PAGE 5

/** The string list containing the index */
string_list_t* sl_index;

/** The page index we are currently viewing */
int current_page = 0;

/** An array of string list containing the content pages */
string_list_t** sl_content;

game_state_t* init_gs_help(void);
void free_gs_help(game_state_t*);

int draw_gs_help(void);
int back_to_intro(void);

int help_page_up(void);
int help_page_down(void);

#endif // _GS_GAME_H_
