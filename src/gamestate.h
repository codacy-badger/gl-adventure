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

/** \file gamestate.h
  * Header file of game state handling
  *
  *
  */

#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <X11/Xlib.h>

#include "keybinding.h"
#include "colors.h"
#include "graphics.h"

/** The function pointer used for gamestate draw() function
  *
  */
typedef int (*gs_fncptr_t)(void);

/** The function pointer used for gamestate timer() function
  *
  * \return true to redraw the game
  *
  */
typedef bool (*gs_timer_fncptr_t)(void);

/** Defines a game state
  *
  */
typedef struct{
  char*               name;          //!< The name of the gamestate
  key_binding_list_t* key_list;      //!< The key binding list
  gs_fncptr_t         draw_fnc_ptr;  //!< The draw() function pointer
  gs_timer_fncptr_t   timer_fnct_ptr;//!< The timer function pointer
                                     //   Can be NULL to avoid timer handle
}game_state_t;

game_state_t* new_game_state(const char*, gs_fncptr_t);
void free_game_state(game_state_t*);

int handle_gs_event(game_state_t*, KeySym);

#endif // _GAME_STATE_H_
