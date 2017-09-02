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

/** \file th_basic.h
  *
  * Declares a basic thinker for monster's AI.
  *
  */

#ifndef _THINKER_BASIC_H_
#define _THINKER_BASIC_H_

#include "monster.h"
#include "map.h"
#include "player.h"

/** A memory structure for the basic thinker
  *
  */
typedef struct{
  int dummy; //!< A dummy value
}basic_thinker_memory_t;

void basic_thinker(monster_t*, map_t*, player_t*);
void* basic_thinker_init_memory(void);
void basic_thinker_free_memory(void*);

#endif // _THINKER_BASIC_H_
