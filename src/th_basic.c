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

/** \file th_basic.c
  *
  * Implements a basic thinker for monster's AI.
  *
  */

#include "th_basic.h"
#include "monster.h"
#include "map.h"
#include "player.h"

#include "logger.h"

#include <stdlib.h>
#include <assert.h>

/** Initialize the basic thinker memory
  *
  * \return A \ref basic_thinker_memory_t pointer structure casted to void*
  *
  */
void* basic_thinker_init_memory(void){
  basic_thinker_memory_t* mem=(basic_thinker_memory_t*)
    malloc(sizeof(basic_thinker_memory_t));
  return (void*)mem;
}

/** Initialize the basic thinker memory
  *
  * \param mem The basic_thinker_memory_t pointer structure casted to void*
  *
  */
void basic_thinker_free_memory(void* mem){
  assert(mem && "Cannot free NULL thinker memory");
  free(mem);
}


/** The basic thinker function pointer
  *
  * \param mon The monster the thinker think for
  * \param map The current map
  * \param pla The human player
  *
  */
void basic_thinker(monster_t* mon, map_t* map, player_t* pla){
  LOG("basic_thinker() called.");
  basic_thinker_memory_t* mem=(basic_thinker_memory_t*)mon->memory;

  int decision = (int)rand()%2; // from 0 to 1
  switch(decision){
  case 0:
    LOG("We are Turning to 90");
    mon->state = TURNING;
    mon->to_direction = 90;
    mon->from_direction = mon->direction;
    break;
  case 1:
    LOG("We are Moving");
    break;
  }
}
