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

/** \file gslist.c
  * Implements the function used when managing game state list
  *
  *
  */
#include "gslist.h"

#include <stdlib.h>
#include <assert.h>

#include "gs_monster.h"

#include "gslist.h"
#include "gsgame.h"
#include "gshelp.h"
#include "gsintro.h"

/** Allocates and return the game state list
  *
  * The game states are created and added to the list.
  *
  * The list size is defined by \ref GS_LIST_SIZE macro.
  *
  */
game_state_t** init_gs_list(void){
  // To test list integrity with an assert
  int nbstate=0;

  size_t size = sizeof(game_state_t*) * GS_LIST_SIZE;
  game_state_t** gsl =(game_state_t**)malloc(size);

  gsl[GS_INTRO] = (game_state_t*)init_gs_intro();   
  nbstate++; // To add after each new game_state addition

  gsl[GS_HELP] = (game_state_t*)init_gs_help();   
  nbstate++; // To add after each new game_state addition

  gsl[GS_GAME] = (game_state_t*)init_gs_game();   
  nbstate++; // To add after each new game_state addition

  gsl[GS_MONST] = (game_state_t*)init_gs_monster();   
  nbstate++; // To add after each new game_state addition

  assert(nbstate == GS_LIST_SIZE && 
	 "gs_list creation error, please check number of states.");

  return gsl;
}

/** Free the given game state list
  *
  * Each state is destroyed before deallocate the list.
  *
  * \param list The list to be freed
  *
  */
void free_gs_list(game_state_t** list){
  free_gs_intro(list[GS_INTRO]);
  free_gs_help(list[GS_HELP]);
  free_gs_game(list[GS_GAME]);

  free(list);
}
