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

/** \file monster.c
  * Implementation file of a monster.
  *
  */

#include "monster.h"

#include <stdlib.h>
#include <assert.h>

#include "opengl.h"
#include "th_basic.h"
#include "time_utils.h"

/** Initialize a new monster
  *
  * \param vX, vY The monster's position in cases.
  *
  */
monster_t* init_monster(int vX, int vY){
  monster_t* my = (monster_t*)malloc(sizeof(monster_t));

  my->think_time = 600;
  my->move_time  = 800;
  my->turn_time  = 1200;
  
  my->x = vX;
  my->y = vY;

  my->direction = NORTH;

  my->to_direction = -1;
  my->from_direction = -1;
  my->to_nextcase = -1;

  my->thinker = basic_thinker;
  my->init_memory = basic_thinker_init_memory;
  my->free_memory = basic_thinker_free_memory;

  my->state = THINKING;
  my->state_time = get_game_time() * 1000;
  my->memory=NULL;
  return my;
}

/** Delete the given monster
  *
  * \param m The monster to be freed
  *
  */
void free_monster(monster_t* m){
  assert(m && "Cannot free a NULL monster pointer");

  if (m->memory != NULL)
    m->free_memory(m->memory);

  free(m);
  m=NULL;
}

/** Draws the given monster
  *
  * \param m The monster to be drawn
  *
  */
void draw_monster(monster_t* m){
  glPushMatrix();
  float z=-10.0f;

  glTranslatef(m->x, -m->y, 0.0f);
  glScalef(0.3f, 0.3f, 1.0f);
  glRotatef(m->direction, 0.0f, 0.0f, 1.0f);

  glBegin(GL_TRIANGLES);
    glColor3f(0.6f, 0.0f, 0.0f);
    glVertex3f( 0.0f, 1.0f, z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, z);	
    glVertex3f( -1.0f, -1.0f, z);	
  glEnd();			

  glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.6f);
    glVertex3f( 0.0f, 1.0f, z);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f( 0.4f, -0.8f, z);	
    glVertex3f( 0.0f, -0.5f, z);	
  glEnd();			
  glBegin(GL_TRIANGLES);
    glColor3f(0.0f, 0.0f, 0.6f);
    glVertex3f( 0.0f, 1.0f, z);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f( -0.4f, -0.8f, z);	
    glVertex3f( 0.0f, -0.5f, z);	
  glEnd();			
  glPopMatrix();

}

/** Advance one frame and move the monster
  *
  * Calls the thinker and wait to know what to do. Moves 
  * the monster if needed
  *
  * \param m   The monster
  * \param map The current map
  * \param p   The player
  *
  */
void advance_monster(monster_t* m, map_t* map, player_t* p){
  // Initialize memory if needed
  if (m->memory == NULL)
    m->memory = m->init_memory();
  
  msec time = get_game_time() * 1000;
  if (m->state == THINKING){
    if (time > m->state_time + m->think_time){
      m->thinker(m, map, p);
      m->state_time = time;
    }
  }
  else if (m->state == TURNING){
    // Time elapsed while turning
    msec t_time = time - m->state_time;

    if (t_time > m->turn_time){
      m->state = THINKING;
      return;
    }

    // exprimed in percent of total turning time
    double tt_percent = (t_time * 100) / m->turn_time;
    m->direction = m->from_direction + ((m->to_direction - m->from_direction) * tt_percent);
    printf("TURNING: from %d to %d (%d%%)\n", 
	   m->from_direction,
	   m->to_direction,
	   tt_percent);
  }
}
