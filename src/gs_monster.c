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

/** \file gs_monster.c
  * Implementation a simple monster test gamestate.
  *
  */

#include "gs_monster.h"

//#include <X11/Xlib.h>
#define XK_MISCELLANY
#  include <X11/keysymdef.h> // For keysym

#include "gl_window.h"
#include "def.h"
#include "gsgame.h"
#include "monster.h"
#include "logger.h"

monster_t* my_monster;

bool gs_monster_timer_callback(void);

/** Initialize a monster test gamestate
  *
  */
game_state_t* init_gs_monster(void){
  game_state_t* gs = new_game_state("GS4 (Monster test)", &draw_gs_monster);
  // Setting timer callback
  gs->timer_fnct_ptr=&gs_monster_timer_callback;
  gs->key_list = add_binding(gs->key_list, XK_Escape, &gs_monster_quit);
  gs->key_list = add_binding(gs->key_list, XK_Page_Up, &game_zoom_in);
  gs->key_list = add_binding(gs->key_list, XK_Page_Down, &game_zoom_out);
  angle=0.0f;
  inc_angle=0.20f;

  my_monster=init_monster(0, 2);

  return gs;
}

void free_gs_monster(game_state_t* gs){
  free_monster(my_monster);
  free_game_state(gs);
}

int draw_gs_monster(void){
  advance_monster(my_monster, map, player);

  /*  angle += inc_angle;
  if (angle>360)
    angle=0;
  */
  draw_gs_game();
  glPushMatrix();

  /*  gl_center_print( fonts.h1, "Monster test", 0.17f);      
  gl_center_print( fonts.h1, "Monster test1", 0.0f);      
  */  
  
  glTranslatef(-1.0f,0.0f,-map_zoom+7.0f);
  draw_monster(my_monster);
  /*  glTranslatef((map->player.x * -1.0f) - 0.5f, 
	       (map->player.y * 1.0) - 0.0f , 
	       -map_zoom);
  */

  glPopMatrix();

  return 0;
}

int gs_monster_quit(void){
  return 1; //Must quit
}


bool gs_monster_timer_callback(void){
  //  LOG("gs_monster_timer_callback(void){ called");
}
