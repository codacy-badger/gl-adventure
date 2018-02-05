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

/** \file hud.c
  *
  * Implementation of functions for head up display
  *
  */
#include "hud.h"

#include "gl_window.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "fonts.h"
#include "logger.h"
#include "object_list.h"
#include "gsgame.h"
#include "opengl.h"

#include "def.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext (String)

unsigned int current_selected_item=0;

/** Draws the player HUD
  *
  * This is a little box always drawn, showing player's name, life and
  * mana.
  *
  * \param pl     The player 
  *
  */
void draw_player_hud( player_t* pl ){

  // Draws background
  glBindTexture(GL_TEXTURE_2D, 0);   
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  float col_base = 0.2f;
  // Set z-order to less than -1.0 to be in foreground of the game UI
  // (obejct list and map name)
  float z_order = -1.0f;
  float x1 = -0.2f;
  float y1 = 0.40f;
  float x2 = 0.2f;
  float y2 = 0.31;

  // Light effect
  GLfloat le = 0.3f;

  glBegin(GL_QUADS);
    glColor4f(0.7f, 0.7f, 0.7f, 0.7f);
    glVertex3f( x1, y1, z_order);
    glVertex3f( x1, y2, z_order);	
 
    glColor4f(0.7f + le, 0.7f + le, 0.7f + le, 0.7f);
    glVertex3f( x2, y2, z_order);	
    glVertex3f( x2, y1, z_order);	
  glEnd();	


  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

  // draws player's name
  glColor4f(1.0f,1.0f, 1.0f, 0.9f);
  glRasterPos3f(-0.19f, 0.38f , -1.0f);
  gl_print(fonts.p, pl->name);

  // draws bars
  draw_life_bar( pl );
  draw_mana_bar( pl );
  
}

/** Draws the life bar
  *
  * \param pl     The player
  *
  */
void draw_life_bar( player_t* pl ){

  float z_order = -1.0f;
  float x1 = -0.19f;
  float y1 = 0.37f;
  float x2 = 0.19f;
  float y2 = 0.35f;

  float myHUD_BAR_WIDTH = x2*2;
  GLfloat base_col = 0.3f;
  GLfloat dark_col = 0.1f;

  // Grey background
  glBegin(GL_QUADS);
    glColor4f(dark_col, dark_col, dark_col, 1.0f);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x2, y2, z_order);	
    glColor4f(base_col, base_col, base_col, 1.0f);
    glVertex3f( x2, y1, z_order);	
    glVertex3f( x1, y1, z_order);
  glEnd();	

  double bw = (pl->life.value * myHUD_BAR_WIDTH) / pl->life.max;

  double red = 1.0f - (bw / (myHUD_BAR_WIDTH * 1.2f));

  glBegin(GL_QUADS);
    glColor4f(red - 0.8f, 0.2f - red, 0.0f, 1.0f);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x1 + bw, y2, z_order);	

    glColor4f(red, 1.0f - red, 0.0f, 1.0f);
    glVertex3f( x1 + bw, y1, z_order);	
    glVertex3f( x1, y1, z_order);
  glEnd();	

}

/** Draws the mana bar
  *
  * \param pl     The player
  *
  */
void draw_mana_bar( player_t* pl ){
  float z_order = -1.0f;
  float x1 = -0.19f;
  float y1 = 0.34f;
  float x2 = 0.19f;
  float y2 = 0.32f;

  float myHUD_BAR_WIDTH = x2*2;
  GLfloat base_col = 0.3f;
  GLfloat dark_col = 0.1f;


  // Grey background
  glBegin(GL_QUADS);
    glColor4f(dark_col, dark_col, dark_col, 1.0f);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x2, y2, z_order);	

    glColor4f(base_col, base_col, base_col, 1.0f);
    glVertex3f( x2, y1, z_order);	
    glVertex3f( x1, y1, z_order);
  glEnd();	

  double bw = (pl->mana.value * myHUD_BAR_WIDTH) / pl->mana.max;

  glBegin(GL_QUADS);
    glColor4f(0.0f, 0.0f, 0.2f, 1.0f);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x1 + bw, y2, z_order);	
    
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glVertex3f( x1 + bw, y1, z_order);	
    glVertex3f( x1, y1, z_order);
  glEnd();	
}

/** Draws the character dialog
  *
  * \param pl     The player
  *
  */
void draw_character_dialog( player_t* pl){
  char buffer[256];

  glBindTexture(GL_TEXTURE_2D, 0);   
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  float col_base = 0.2f;
  // Set z-order to less than -1.0 to be in foreground of the game UI
  // (obejct list and map name)
  float z_order = -0.99f;
  float x1 = 0.05f;
  float y1 = 0.39f;
  float x2 = 0.53f;
  float y2 = -0.39;

  glBegin(GL_QUADS);
  glColor4f(col_base, col_base, col_base, 0.7f);
    glVertex3f( x1, y1, z_order);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x2, y2, z_order);	
    glVertex3f( x2, y1, z_order);	
  glEnd();	

  // Text color
  glColor4f(1.0f, 1.0f, 1.0f, 0.9f);

  float y= 0.36f;
  float x= 0.06;
  float xr= 0.30; // Right x

  float step= 0.03f;
  float bstep= 0.05;

  // Player's name
  sprintf(buffer, _("Player : %s"), pl->name);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);


  // player's level
  sprintf(buffer, _("Level : %d"), pl->level);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);

  // Life
  y -= bstep;
  sprintf(buffer, _("Life : %d/%d"), pl->life.value, pl->life.max);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  // Mana
  sprintf(buffer, _("Mana : %d/%d"), pl->mana.value, pl->mana.max);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);

  // Food
  y -= step;
  sprintf(buffer, _("Food : %d/%d"), pl->food.value, pl->food.max);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  sprintf(buffer, _("(Down time : %4.2fs)"), pl->foodDownTime);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);

  // Water
  y -= step;
  sprintf(buffer, _("Water : %d/%d"), pl->water.value, pl->water.max);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  sprintf(buffer, _("(Down time : %4.2fs)"), pl->waterDownTime);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);


  // Gold 
  y -= bstep;
  sprintf(buffer, _("Gold : %d"), pl->gold);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  // Points 
  y -= bstep;
  sprintf(buffer, _("Points : %d"), pl->points);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  // Moves 
  sprintf(buffer, _("Moves : %d"), pl->moves);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);


  // Fighter 
  y -= bstep;
  sprintf(buffer, _("Fighter : %d"), pl->fighter.level);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  sprintf(buffer, _("Xps : %d/%d"), 
	  pl->fighter.current_level_xp,
	  pl->fighter.next_level_xp);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);

  // Wizard 
  y -= step;
  sprintf(buffer, _("Wizard : %d "), pl->wizard.level);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  sprintf(buffer, _("Xps : %d/%d"), 
	  pl->wizard.current_level_xp,
	  pl->wizard.next_level_xp);
  glRasterPos3f( xr, y , z_order);  gl_print(fonts.p, buffer);

  // Strength
  y -= bstep;
  sprintf(buffer, _("Strength : %d"), pl->strength);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);
 
  // Intelligence
  y -= step;
  sprintf(buffer,"Intelligence : %d", pl->intelligence);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);

  // Charge
  y -= bstep;
  sprintf(buffer,"charge : %4.3f/%4.3f kg", pl->current_charge, pl->max_charge);
  glRasterPos3f( x, y , z_order);  gl_print(fonts.p, buffer);
 
  // In game time
  y -= bstep;
  glRasterPos3f( x, y , z_order);
  draw_character_time( pl);
}

/** Draws the in-game time
  *
  * \param pl     The player
  *
  */
void draw_character_time( player_t* pl){
  int s,m,h;
  char buffer[100];
  
  if (pl->time < 60){
    sprintf(buffer, _("In game time : %d s"),pl->time);

  }
  else if (pl->time < 3600){
    m = pl->time / 60;
    s = pl->time % 60;
    sprintf(buffer, _("In game time : %d m %d s"), m,s);
  }
  else{
    m = pl->time / 60;
    s = pl->time % 60;

    h = m /60;
    m = m %60;

    sprintf(buffer, _("In game time : %d h %d m %d s"), h, m,s);

  }

  gl_print(fonts.p, buffer);

}

/** Draw a bar with a background
  *
  * \param vGraph The graphic object
  * \param x, y   The position of the bar
  * \param w      The width of the bar
  * \param val    The value to represent
  * \param max    The max possible value
  *
  */
void draw_background_bar(graphics_t* vGraph, int x, int y, int w, 
			 int val, int max){

  colors_t* col = vGraph->col;
  Window    win = vGraph->win;

  // draw background
  XSetForeground(_DIS, _XGC, _LGR);
  XFillRectangle(_DIS, win, _XGC, x, y, w, HUD_BORDER);
  
  double bw = (val * w) / max;
  XSetForeground(_DIS, _XGC, col->green.pixel);
  XFillRectangle(_DIS, win, _XGC, x, y, (int)bw, HUD_BORDER);

  XSetForeground(_DIS, _XGC, col->black.pixel);
}

/** Draw the player's inventory
  *
  * \param player The player
  *
  */
void draw_inventory_dialog(player_t* player){
  glBindTexture(GL_TEXTURE_2D, 0);   
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  float col_base = 0.2f;
  // Set z-order to less than -1.0 to be in foreground of the game UI
  // (obejct list and map name)
  float z_order = -0.99f;
  float x1 = -0.53f;
  float y1 = 0.39f;
  float x2 = -0.05f;
  float y2 = -0.39;

  glBegin(GL_QUADS);
  glColor4f(col_base, col_base, col_base, 0.7f);
    glVertex3f( x1, y1, z_order);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x2, y2, z_order);	
    glVertex3f( x2, y1, z_order);	
  glEnd();	


  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

  // Draw items
  glColor4f(0.0f,0.0f, 1.0f, 0.9f);
  
  unsigned int i=0;

  // header
  glRasterPos3f(-0.48f, 0.36f , -0.99f);
  gl_print(fonts.p, "Object");
  glRasterPos3f(-0.28f, 0.36f , -0.99f);
  gl_print(fonts.p, "Number");


  float ydev = 0.0f;
  for (i; i<player->inventory->size; i++){
    if (object_list,player->inventory->items[i]->number > 0){
      if (i==current_selected_item){
	glColor4f(cycling_color, 0.0f, 1.0f, 0.9f);
      }
      else{
	glColor4f(1.0f,1.0f, 1.0f, 0.9f);
      }
      
      glRasterPos3f(-0.51f, 0.32f - ydev, -0.99f);
      object_t* o=get_object_by_id(object_list,player->inventory->items[i]
				   ->name);

      assert(o && "Cannot get non-NULL object");
      gl_print(fonts.p, o->en);
      
      char num[10];
      sprintf(num, "%d", player->inventory->items[i]->number, num, 10);
      
      glRasterPos3f(-0.25f, 0.32f - ydev, -0.99f);
      gl_print(fonts.p, num);

      ydev += 0.02f;
    }
    
  }

  // reset color
  glColor4f(1.0f,1.0f, 1.0f, 1.0f);

  glDisable(GL_BLEND);

}

/** Selects the previous item in inventory
  *
  * This function is called from gsgame when the inventory is opened
  * and the user hit the UP key.
  *
  * \param pl The player
  *
  */
void previous_selected_item(player_t* pl){
  if (current_selected_item > 0){
    current_selected_item--;
  }
  else{
    current_selected_item = pl->inventory->size - 1;
  }
}

/** Selects the next item in inventory
  *
  * This function is called from gsgame when the inventory is opened
  * and the user hit the DOWN key.
  *
  * \param pl The player
  *
  */
void next_selected_item(player_t* pl){
  if (current_selected_item > pl->inventory->size - 2){
    current_selected_item=0;
  }
  else{
    current_selected_item++;
  }

}

