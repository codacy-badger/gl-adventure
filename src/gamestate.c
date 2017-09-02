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

/** \file gamestate.c
  * Implementation of game state handling
  *
  *
  */


#include "gamestate.h"
#include "keybinding.h"
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/** Creates and return a new game state
  *
  * \param vName The name of the game state
  * \param vDraw The draw() function pointer
  *
  */
game_state_t* new_game_state(const char* vName, gs_fncptr_t vDraw){
  game_state_t* gs = (game_state_t*)malloc(sizeof(game_state_t));
  gs->name = (char*)malloc(strlen(vName)+1);
  strcpy( gs->name, vName );
  gs->key_list = init_key_binding_list();
  gs->draw_fnc_ptr = vDraw;
  gs->timer_fnct_ptr = NULL;
  assert(gs->key_list);

  return gs;
}

/** Free the memory allocated for the given game state
  *
  * \param gs The game state to destroy
  *
  */
void free_game_state(game_state_t* gs){
  destroy_key_binding_list(gs->key_list);
  free(gs->name);
  free(gs);
}

/** Handles an event for a game state
  *
  * \param gs  The game state
  * \param key The Xlib keysym to handle
  *
  * \return The value returned by the key binding handle if found or
  *         -1 if no binding was found.
  *
  */
int handle_gs_event(game_state_t* gs, KeySym key){
  LOG("handle_gs_event() called");

  assert(gs && "Cannot handle events on NULL game state.");
  assert(gs->key_list && "Cannot get valid key binding list for this game state.");
  int ret = handle_kb_event( gs->key_list, key);

  if (ret == -1){
    printf("GameState '%s' do not handle '%d' key\n", gs->name, key);
  }

  return ret;
}
