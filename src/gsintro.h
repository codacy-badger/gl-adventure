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

/** \file gsintro.h
  * Declaration of the introduction game state
  *
  */

#ifndef _GS_INTRO_H_
#define _GS_INTRO_H_

#include "gamestate.h"

/** Defines the last menu id
  *
  * It is used as a 'out of bound' test.
  *
  */
#define INTRO_LAST_MENU 2

/** The current menu item of the intro game state */
short intro_current_menu;

game_state_t* init_gs_intro(void);
void free_gs_intro(game_state_t*);

int draw_gs_intro(void);
int exit_gs_intro(void);

int change_to_help(void);
int change_to_game(void);

int intro_move_up(void);
int intro_move_down(void);

void handle_intro_menu_color(short);
int intro_enter_pressed(void);

#endif // _GS_INTRO_H_
