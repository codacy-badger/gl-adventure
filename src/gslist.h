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

/** \file gslist.h
  * Defines clares the game state list
  *
  */

#ifndef _GS_LIST_H_
#define _GS_LIST_H_

#include "gamestate.h"

/**  Defines the gamestates indexes
  *
  */
enum gs_index_t{
  GS_INTRO = 0,    //!< The Introduction game state
  GS_HELP  = 1,    //!< The help game state
  GS_GAME  = 2,    //!< The game game state
  GS_MONST = 3,    //!< The monster test game state
};

/** Number of gamestate we should allocate */
#define GS_LIST_SIZE 4


game_state_t** init_gs_list(void);
void free_gs_list(game_state_t**);

#endif // _GS_LIST_H_
