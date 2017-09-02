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

/** \file gs_monster.h
  * Header file for a simple monster test gamestate.
  *
  */
#ifndef _GS_MONSTER_H_
#define _GS_MONSTER_H_

#include "gamestate.h"

/// Test variable for rotation
float angle;
/// Test variable for rotation
float inc_angle;

game_state_t* init_gs_monster(void);
void free_gs_monster(game_state_t*);
int draw_gs_monster(void);
int gs_monster_quit(void);

#endif // _GS_MONSTER_H_
