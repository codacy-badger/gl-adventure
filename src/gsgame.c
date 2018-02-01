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

/** \file gsgame.c
  * The implementation of the game gamestate.
  *
  *
  */

#include "gsgame.h"

/** The map's zoom factor */
float map_zoom = 10.0f;


#include "gl_window.h"
#include "load_bmp.h"

#  include <X11/keysym.h> // For keysym

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "time_utils.h"
#include "gslist.h"
#include "logger.h"
#include "player.h"
#include "map.h"
#include "hud.h"

#include "def.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext(String)


/** Initialize the game gamestate
  *
  * \return An allocated and usable game gamestate
  *
  */
game_state_t* init_gs_game(void){
  showing_character_dialog = false;
  showing_inventory_dialog = false;
  map = NULL;

  // The player
  player = init_player();
  DEBUG_PLAYER(player);

  // The objects
  object_list = load_objects();

#ifdef XA_DEBUG
  current_debug_mode= 0;
#endif // XA_DEBUG

#ifdef XA_MAP_USHORT
  LOG("Map is using unsigned short values");
#else 
  LOG("Map is using integer values");
#endif

  game_state_t* gs = new_game_state("GS2 (Game)", &draw_gs_game);

  // Setting timer callback
  gs->timer_fnct_ptr=&game_timer_callback;

  gs->key_list = add_binding(gs->key_list, XK_Escape, &game_back_to_intro);

  gs->key_list = add_binding(gs->key_list, XK_Left, &game_move_left);
  gs->key_list = add_binding(gs->key_list, XK_Right, &game_move_right);
  gs->key_list = add_binding(gs->key_list, XK_Up, &game_move_up);
  gs->key_list = add_binding(gs->key_list, XK_Down, &game_move_down);
  gs->key_list = add_binding(gs->key_list, XK_c, &game_toggle_char_dlg);
  gs->key_list = add_binding(gs->key_list, XK_i, &game_toggle_inv_dlg);
  gs->key_list = add_binding(gs->key_list, XK_Return, &game_collect_items);
  gs->key_list = add_binding(gs->key_list, XK_Page_Up, &game_zoom_in);
  gs->key_list = add_binding(gs->key_list, XK_Page_Down, &game_zoom_out);



#ifdef XA_DEBUG
  gs->key_list = add_binding(gs->key_list, XK_d, &game_toggle_debug);
  gs->key_list = add_binding(gs->key_list, XK_l, &game_log_player);
#endif // XA_DEBUG

  map = get_random_map();
  assert(map && "Can't get a usable map");
  
  DEBUG_MAP(map);
  // Compute axe plan size
  int axe_x_size_plan = (map->width + 1) * sizeof(axe_pos_t);
  int axe_y_size_plan = (map->height + 1) * sizeof(axe_pos_t);
  
  int tot_size = (axe_x_size_plan * 2) + (axe_y_size_plan * 2);
  LOGI("Total allocated memory for axes plans is %d bytes.", tot_size);

  // Allocating memory
  fore_axe_x = (axe_pos_list_t)malloc(axe_x_size_plan);
  assert( fore_axe_x && "Cannot allocate memory for axe plan");

  fore_axe_y = (axe_pos_list_t)malloc(axe_y_size_plan);
  assert( fore_axe_y && "Cannot allocate memory for axe plan");

  back_axe_x = (axe_pos_list_t)malloc(axe_x_size_plan);
  assert( back_axe_x && "Cannot allocate memory for axe plan");

  back_axe_y = (axe_pos_list_t)malloc(axe_y_size_plan);
  assert( back_axe_y && "Cannot allocate memory for axe plan");

  feed_axe_plans();

  return gs;
}

/** Free the game state
  *
  * \param gs The gamestate to free
  *
  */
void free_gs_game(game_state_t* gs){
  
  unload_objects(object_list);
  free_player(player);

  free_game_state(gs);

  free(fore_axe_x);
  free(fore_axe_y);
  free(back_axe_x);
  free(back_axe_y);

  free_map(map);
}

/** The player has walk in a wall 
  *
  */
void walk_in_wall(void){
  player_decrease_life(player, 1, DRM_WIW);
}

/** The player do not walk in a wall 
  *
  * It moves the player and remove the death reason.
  *
  */
void dont_walk_in_wall(void){
  player_move(player);
  player_remove_why_death(player, DRM_WIW);
}

/** Moves the player to the left
  *
  * \return Change state if moved
  *
  */
int game_move_left(void){
  if (showing_inventory_dialog){
    //
  }
  else {
    int x =map->player.x - 1;
    int y = map->player.y;
    
    if (!IS_WALL(MAP(x,y))){
      map->player.x--;
      dont_walk_in_wall();
    }
    else{
      walk_in_wall();
    }
  }
  return 2; // must redraw    
}

/** Moves the player to the right
  *
  * \return Change state if moved
  *
  */
int game_move_right(void){
  if (showing_inventory_dialog){
    //
  }
  else{
    int x =map->player.x + 1;
    int y = map->player.y;
    
    if (!IS_WALL(MAP(x,y))){
      map->player.x++;
      dont_walk_in_wall();
    }
    else{
      walk_in_wall();
    }
  }
  return 2; // must redraw    
}

/** Moves the player to the up
  *
  * \return Change state if moved
  *
  */
int game_move_up(void){
  if (showing_inventory_dialog){
    previous_selected_item(player);
  }
  else{
    int x =map->player.x;
    int y = map->player.y - 1;
  
    if (!IS_WALL(MAP(x,y))){
      map->player.y--;
      dont_walk_in_wall();
    }
    else{
      walk_in_wall();
    }
  }
  return 2; // must redraw    
}

/** Moves the player to the down
  *
  * \return Change state if moved
  *
  */
int game_move_down(void){
  if (showing_inventory_dialog){
    next_selected_item(player);
  }
  else{
    int x =map->player.x;
    int y = map->player.y + 1;
    
    if (!IS_WALL(MAP(x,y))){
      map->player.y++;
      dont_walk_in_wall();
    }
    else{
      walk_in_wall();
    }
  }
  return 2; // must redraw    
}

/** Zoom the game in 
  *
  * It simply changes the value of \ref map_zoom.
  *
  */
int game_zoom_in(void){
  map_zoom -= 1.0f;
  return 2;
}

/** Zoom the game out 
  *
  * It simply changes the value of \ref map_zoom.
  *
  */
int game_zoom_out(void){
  map_zoom += 1.0f;
  return 2;
}

/** The draw callback for gsgame
  *
  */
int draw_gs_game(){
  glPushMatrix();

  // Drawing text now, before the translation
  draw_message();

  // Reset color and texture
  glColor3f(1.0f, 1.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, texture[1]);

  // Translate before drawing map
  glTranslatef((map->player.x * -1.0f) - 0.5f, 
	       (map->player.y * 1.0) - 0.0f , 
	       -map_zoom);

  draw_map();

  glPopMatrix();

  // Draws blended HUD
  draw_player_hud(player);

  if (LOBJ(map->player.x,map->player.y) != NULL){
    draw_objects_name();
  }

  if (showing_inventory_dialog){
    draw_inventory_dialog(player);
  }

  if (showing_character_dialog){
    draw_character_dialog(player);
  }
}

/** Go back to intro
  *
  * \return The value needed to go back to \ref GS_INTRO
  *
  */
int game_back_to_intro(void){
  return ((GS_INTRO << GS_CHST_BIT_SHIFT) +6);
}

/** Draws the grid
  *
  * \param vGraph The graphic object
  * \param col    The color to draw the grid with
  * \param x1, x2 The X axis parameers
  * \param y1, y2 The Y axis parameers
  *
  */
void draw_grid(graphics_t* vGraph, XColor col, int x1, int y1, int x2, int y2){
  Display*  dis = vGraph->dis;
  GC        xgc = vGraph->xgc;
  Window    win = vGraph->win;

  XSetForeground(dis, xgc, col.pixel);

  int x_val = (x2 - x1) / X_VISIB;
  int y_val = (y2 - y1) / Y_VISIB;
  int x, y;


  for (x=0; x< X_VISIB; x++){
    XDrawLine(dis, win, xgc, x1 + (x * x_val), y1, x1 + (x * x_val), y2);
  }
  for (y=0; y< Y_VISIB; y++){
    XDrawLine(dis, win, xgc, x1 , y1 + (y * y_val), x2, y1 + (y * y_val));
  }

}

/** Feed the plan axis
  *
  * Use this function in order to use the \ref fore_axe_x, \ref fore_axe_y,
  * \ref back_axe_x, \ref back_axe_x values.
  *
  */
void feed_axe_plans(){
  int x1;             // The left side of the grid
  int y1;             // The top side of the grid
  double x_val, y_val;   // The space between 2 cases
  int x, y;           // The index of for statements

  // check X_VISIB and Y_VISIB
  if (MWINW % map->width != 0){
    printf("map width value doesn't return round value with current "
	   "window's size\n");
  }
  if (MWINW % map->height != 0){
    printf("map height value doesn't return round value with current "
	   "window's size\n");
  }

  // Foreground X axe
  x1 = 0;
  x_val = MWINW / map->width;
  for (x=0; x< map->width + 1; x++){    FX(x) = x1 + (x * x_val);  }
    
  // Foreground Y axe
  y1 = 0;
  y_val = MWINH / map->height;
  for (y=0; y< map->height + 1; y++){    FY(y) = y1 + (y * y_val);  }

 // Background X axe
  x1 = 0 + map->depth;
  x_val = (MWINW - (map->depth*2)) / map->width;
  for (x=0; x< map->width + 1; x++){    BX(x) = x1 + (x * x_val);  }
    
  // Background Y axe
  y1 = 0 + map->depth;
  y_val = (MWINH - (map->depth*2)) / map->height;
  for (y=0; y< map->height + 1; y++){    BY(y) = y1 + (y * y_val);  }

}

/** Draws the player
  *
  */
void draw_player(){
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);   

  glColor4f(0.0f,1.0f,0.0f, 0.5f);
  glBegin(GL_QUADS);
    glVertex3f( 0.2f, 0.2f, -3.0f);
    glVertex3f( 0.2f, 0.8f, -3.0f);	
    glVertex3f( 0.8f, 0.8f, -3.0f);	
    glVertex3f( 0.8f, 0.2f, -3.0f);	
  glEnd();	
  // Restore neutral color
  glColor3f(1.0f,1.0f,1.0f);
  glDisable(GL_BLEND);

}

/** Draws the end of level
  *
  */
void draw_finish(){
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);   

  glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
  glBegin(GL_QUADS);
    glVertex3f( 0.2f, 0.2f, -3.0f);
    glVertex3f( 0.2f, 0.8f, -3.0f);	
    glVertex3f( 0.8f, 0.8f, -3.0f);	
    glVertex3f( 0.8f, 0.2f, -3.0f);	
  glEnd();	
  // Restore neutral color
  glColor3f(1.0f,1.0f,1.0f);
  glDisable(GL_BLEND);
}


/** Draws a case floor
  *
  */
void draw_floor(){
  glBindTexture(GL_TEXTURE_2D, texture[2]);   /* select our  texture */
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f( 0.0f, 0.0f, -3.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f( 0.0f, 1.0f, -3.0f);	
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f( 1.0f, 1.0f, -3.0f);	
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f( 1.0f, 0.0f, -3.0f);	
  glEnd();
};

/** Draws an object in this case
  *
  *
  *
  */
void draw_case_object(){
  glEnable(GL_BLEND);
  glBindTexture(GL_TEXTURE_2D, 0);   

  glColor4f(0.6f,0.3f,0.0f, 0.5f);
  glBegin(GL_QUADS);
    glVertex3f( 0.2f, 0.2f, -3.0f);
    glVertex3f( 0.2f, 0.8f, -3.0f);	
    glVertex3f( 0.8f, 0.8f, -3.0f);	
    glVertex3f( 0.8f, 0.2f, -3.0f);	
  glEnd();	
  // Restore neutral color
  glColor3f(1.0f,1.0f,1.0f);
  glDisable(GL_BLEND);

}


/** Draws a case north wall
  *
  */
void draw_north_wall(void){
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);    glVertex3f( 0.0f, 1.0f, -2.0f);
    glTexCoord2f(1.0f, 1.0f);    glVertex3f( 1.0f, 1.0f, -2.0f);	
    glTexCoord2f(1.0f, 0.0f);    glVertex3f( 1.0f, 1.0f, -3.0f);	
    glTexCoord2f(0.0f, 0.0f);    glVertex3f( 0.0f, 1.0f, -3.0f);	
  glEnd();
};

/** Draws a case west wall
  *
  */
void draw_west_wall(void){
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
    glTexCoord2f(1.0f, 1.0f);    glVertex3f( 0.0f, 1.0f, -2.0f);
    glTexCoord2f(0.0f, 1.0f);    glVertex3f( 0.0f, 0.0f, -2.0f);	
    glTexCoord2f(0.0f, 0.0f);    glVertex3f( 0.0f, 0.0f, -3.0f);	
    glTexCoord2f(1.0f, 0.0f);    glVertex3f( 0.0f, 1.0f, -3.0f);	
  glEnd();
}

/** Draws a case south wall
  *
  *
  */
void draw_south_wall(void){
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
    glTexCoord2i(1, 1);    glVertex3f( 0.0f, 0.0f, -2.0f);
    glTexCoord2i(0, 1);    glVertex3f( 1.0f, 0.0f, -2.0f);	
    glTexCoord2i(0, 0);    glVertex3f( 1.0f, 0.0f, -3.0f);	
    glTexCoord2i(1, 0);    glVertex3f( 0.0f, 0.0f, -3.0f);	
  glEnd();
}

/** Draws a case east wall
  *
  */
void draw_east_wall(void){
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
    glTexCoord2i(0, 1);    glVertex3f( 1.0f, 1.0f, -2.0f);
    glTexCoord2i(1, 1);    glVertex3f( 1.0f, 0.0f, -2.0f);	
    glTexCoord2i(1, 0);    glVertex3f( 1.0f, 0.0f, -3.0f);	
    glTexCoord2i(0, 0);    glVertex3f( 1.0f, 1.0f, -3.0f);	
  glEnd();

}

/** Draws the currently played map
  *
  */
void draw_map(){
  int x,y;

  assert(map && "Current map pointer is NULL");

  // How many translate to get original y
  GLfloat orig_x = -1.0 * map->width;

  for (y=0; y < map->height; y++){
    for (x=0; x<map->width; x++){
      if (!IS_WALL(MAP(x,y))){

	// Draw case
	draw_floor();

	if (x==map->player.x && y==map->player.y){
	  draw_player();
	}

	if (x==map->finish.x && y==map->finish.y){
	  draw_finish();
	}

	if (LOBJ(x,y) != NULL){
	  draw_case_object();
	}

	// Draw north
	if (y == 0 || (IS_WALL(MAP(x,y-1)))){
	  draw_north_wall();
	}
	// Draw south
	if (y == map->height-1 || (y < map->height && IS_WALL(MAP(x,y+1)))){
	  draw_south_wall();
	}

	// Draw west
	if (x == 0 || (x > 0 && IS_WALL(MAP(x-1,y)))){
	  draw_west_wall();
	}

	// Draw east
	if (x == map->width || (x < map->width && IS_WALL(MAP(x+1,y)))){
	  draw_east_wall();
	}

      }
      glTranslatef(1.0f, 0.0f, 0.0f); // One case to the est
    }
    glTranslatef(orig_x, -1.0f, 0.0f); // One case to the south
  }
}

/** You're in a case that contains object
  *
  * \param vGraph The graphical object
  *
  */
void show_object_message(graphics_t* vGraph){
  Display*  dis  = vGraph->dis;
  GC        xgc  = vGraph->xgc;
  colors_t* col  = vGraph->col;
  Window    win  = vGraph->win;

  if (LOBJ(map->player.x, map->player.y) != NULL){
    // We must show a dialog
    int y, idx;
    int h = (LOBJ(map->player.x, map->player.y)->size + 2 ) * 17;
    int w = 200;
    int x = (MWINW / 2) - (w/2);
    // Y position according to the player position
    if (map->player.y > map->height/2){
      y = 10;
    }
    else{
      y = MWINH - h - 10;
    }


    // Drawing dialog
    XSetFillStyle(dis, xgc, FillSolid);

    XSetForeground(dis, xgc, col->black.pixel);
    XFillRectangle(dis, win, xgc, x-1, y-1, w+2, h+2);

    XSetForeground(dis, xgc, col->white.pixel);
    XFillRectangle(dis, win, xgc, x, y, w, h);



    // Printing header
    XSetForeground(dis, xgc, col->red.pixel);
    y += 15;
    if (LOBJ(map->player.x, map->player.y)->size > 1){
      center_text(dis, win, xgc, vGraph->fnt->ipd,"You find these objects", y);
    }
    else{
      center_text(dis, win, xgc, vGraph->fnt->ipd,"You find this object",y);
    }
    y += 15;

    // Printing object names
    LOG("Printing object names");
    XSetForeground(dis, xgc, col->green.pixel);
    printf("There is %d objects\n", LOBJ(map->player.x, map->player.y)->size);
    for (idx = 0; idx <LOBJ(map->player.x, map->player.y)->size; idx++){
      char* on = LOBJ(map->player.x, map->player.y)->data[idx];
      printf("Finding object with name %s\n", on);
      object_t* obj =get_object_by_id( object_list, on);
      assert(obj && "get_object_by_id() has returned a NULL object");
      center_text(dis, win,xgc,vGraph->fnt->ipd,obj->en,y);
      y += 15;
  }
    // Printing footer
    XSetForeground(dis, xgc, col->red.pixel);
    if (LOBJ(map->player.x, map->player.y)->size > 1){
      center_text(dis, win,xgc,vGraph->fnt->ipd,"Press ENTER to collect it",y);
    }
    else{
      center_text(dis,win,xgc,vGraph->fnt->ipd,"Press ENTER to collect them",y);
    }
  }

}


/** Toggle the visibility of the character dialog
  *
  * \return must redraw
  *
  */
int game_toggle_char_dlg(void){
  showing_character_dialog = !showing_character_dialog;
  return 2; // must redraw
}

/** Toggle the visibility of the inventory dialog
  *
  * \return must redraw
  *
  */
int game_toggle_inv_dlg(void){
  showing_inventory_dialog = !showing_inventory_dialog;
  return 2; // must redraw
}

/** The timer callback
  *
  * Called every seconds by a POSIX timer.
  *
  * \return true if graphic must be redraw.
  *
  */
bool game_timer_callback(void){
  player_increase_time(player);

  if(player_decrease_food(player, 1)){
    player_decrease_life(player, 2, "Out of food");
  }

  if (player_decrease_water(player, 1)){
    player_decrease_life(player, 2, "Out of water");
  }
  
  return true;
}

/** Show a message telling why player loose life if needed
  *
  * \param vGraph The graphic object
  *
  */
void show_death_message(graphics_t* vGraph){
  if (player->why_death){
    XSetForeground(_DIS, _XGC, _RED);
    center_text(_DIS, _WIN, _XGC, _IPN, player->why_death, MWINH - 40);
  }
}

/** Collect object found in the case you are in
  *
  * \return always 2.
  *
  */
int game_collect_items(void){
  if (showing_inventory_dialog){
    player_use_item(player);
  }
  else{
    LOG("Collecting objects");
    int idx = (map->player.x) + ((map->player.y)*map->width);
    string_list_t* objlst = map->case_list[idx].objects;
    
    if (objlst){
      char* item_name = objlst->data[objlst->size-1];
      inventory_list_add(player->inventory, item_name);
      // Casts avoid compilation warnings
      map->case_list[idx].objects = (string_list_t*)object_list_pop_back
	((object_list_t*)objlst);
    }
  }
  player_compute_charge(player);


  return 2; //must redraw
}

/** Draws the object name in a top-left grey box
  *
  * You are in a case that contains objects and we should draw the
  * objects name
  *
  * It is called from \ref draw_gs_game() if needed but is drawn after
  * the map.
  *
  */
void draw_objects_name(void){
  glBindTexture(GL_TEXTURE_2D, 0);   
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  float col_base = 0.6f;


  int idx = (map->player.x) + ((map->player.y)*map->width);
  string_list_t* objlst = map->case_list[idx].objects;

  float height = (0.02f * objlst->size) + 0.02;

  float z_order = -1.0f;
  float x1 = -0.53f;
  float y1 = 0.39f;
  float x2 = -0.25f;
  float y2 = y1 - height;

  glBegin(GL_QUADS);
  glColor4f(col_base, col_base, col_base, 0.7f);
    glVertex3f( x1, y1, z_order);
    glVertex3f( x1, y2, z_order);	
    glVertex3f( x2, y2, z_order);	
    glVertex3f( x2, y1, z_order);	
  glEnd();	


  glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

  // reset color
  glColor4f(1.0f, 0.0f, 0.0f, 0.7f);

  unsigned int i=0;

  for (i=0; i< objlst->size; i++){
    glRasterPos3f(-0.51f, 0.36f - (i * 0.02), -1.0f);
    object_t* o=get_object_by_id(object_list,objlst->data[i]);
    assert(o && "Cannot get non-NULL object");
    gl_print(fonts.p, o->en);
  }
  glDisable(GL_BLEND);
}

/** Show the map name or a message
  *
  * This is called from the \ref draw_gs_game() function.
  *
  */
void draw_message(void){
  glColor4f(0.0f,1.0f,0.0f, 1.0f);
  glBindTexture(GL_TEXTURE_2D, 0);

  float z_order = -1.0f;
  float x = 0.4f;
  float y1 = -0.38f;
  float y2 = -0.405f;
  glBegin(GL_QUADS);
    glVertex3f( -x, y1, z_order);
    glVertex3f( -x, y2, z_order);	
    glVertex3f( x, y2, z_order);	
    glVertex3f( x, y1, z_order);	
  glEnd();	
  
  glTranslatef(0.0f, -0.40f, 0.0f);

  // Drawing text
  if (LOBJ(map->player.x,map->player.y) == NULL){
    glColor3f(0.0f, 0.0f, 0.0f);
    gl_center_print(fonts.p, map->name, 0.0f);
  }
  else{
    glColor3f(1.0f, 0.0f, 0.0f);
    gl_center_print(fonts.p, "There is an object here! Please hit Enter "
		    "to loot it.", 0.0f);
  }

}


//////////////////////////////////////////////
// ONLY IN DEBUG MODE
#ifdef XA_DEBUG

  /** Toggles between different debug modes
    *
    * The debug mode cycle toggles between 0 and \ref GAME_MAX_DEBUG_MODE.
    *
    * \return A value for MUST_REDRAW
    *
    */
  int game_toggle_debug(void){

    current_debug_mode++;
  
    if (current_debug_mode > GAME_MAX_DEBUG_MODE)
      current_debug_mode = 0;
    
    return 2; //must redraw
  }

 /** Draw the current debug mode
   *
   * According to the value of \ref current_debug_mode, it will
   * call the debug function and draw a message.
   *
   * \param vGraph The object used to draw
   *
   */
  void draw_game_debug(graphics_t* vGraph){
    if (current_debug_mode==0) return;
    
    Display*  dis = vGraph->dis;
    GC        xgc = vGraph->xgc;
    colors_t* col = vGraph->col;
    Window    win = vGraph->win;
    
    char* debug_text;
    switch(current_debug_mode){
    case 1:
      draw_game_debug_mode_1(vGraph);
      debug_text = "[Debug mode] Showing grids";
      break;

    case 2:
      draw_game_debug_mode_2(vGraph);
      debug_text = "[Debug mode] Showing empty map";
      break;

    case 3:
      draw_game_debug_mode_3(vGraph);
      debug_text = "[Debug mode] Drawing a real test map";
      break;
    }
    
    XSetForeground(dis, xgc, col->red.pixel);
    center_text(dis, win, xgc, vGraph->fnt->ipd, debug_text, MWINH - 20);
  }

  /** Draw the debug mode 1
    *
    * \param vGraph The object used to draw
    *
    */
  void draw_game_debug_mode_1(graphics_t* vGraph){
    Display*  dis = vGraph->dis;
    GC        xgc = vGraph->xgc;
    colors_t* col = vGraph->col;
    Window    win = vGraph->win;

    int x, y;
    int x1, x2;
    int y1, y2;

    // Drawing foreground grid X axe values
    XSetForeground(dis, xgc, col->black.pixel);
    y1 = 0;
    y2 = MWINH;
    for (x=0; x< X_VISIB+1; x++){
      XDrawLine(dis, win, xgc, FX(x), y1, FX(x),y2);
    }

    // Drawing foreground grid Y axe values
    x1 = 0;
    x2 = MWINW;
    for (y=0; y< Y_VISIB+1; y++){
      XDrawLine(dis, win, xgc, x1, FY(y), x2, FY(y));
    }
 
    // Drawing background grid X axe values
    XSetForeground(dis, xgc, col->blue.pixel);
    y1 = MAP_HEIGHT;
    y2 = MWINH - MAP_HEIGHT;
    for (x=0; x< X_VISIB+1; x++){
      XDrawLine(dis, win, xgc, BX(x), y1, BX(x),y2);
    }

    // Drawing background grid Y axe values
    x1 = MAP_HEIGHT;
    x2 = MWINW - MAP_HEIGHT;
    for (y=0; y< Y_VISIB+1; y++){
      XDrawLine(dis, win, xgc, x1, BY(y), x2, BY(y));
    }

    // Reset painting color
    XSetForeground(dis, xgc, col->black.pixel);
   
  }

  /** Draw the debug mode 2
    *
    * \param vGraph The object used to draw
    *
    */
  void draw_game_debug_mode_2(graphics_t* vGraph ){
    Display*  dis = vGraph->dis;
    GC        xgc = vGraph->xgc;
    colors_t* col = vGraph->col;
    Window    win = vGraph->win;

    int x, y;
    int x1, x2;
    int y1, y2;

    // Drawing background grid X axe values
    XSetForeground(dis, xgc, col->black.pixel);
    y1 = MAP_HEIGHT;
    y2 = MWINH - MAP_HEIGHT;
    for (x=0; x< X_VISIB+1; x++){
      XDrawLine(dis, win, xgc, BX(x), y1, BX(x),y2);
    }

    // Drawing background grid Y axe values
    x1 = MAP_HEIGHT;
    x2 = MWINW - MAP_HEIGHT;
    for (y=0; y< Y_VISIB+1; y++){
      XDrawLine(dis, win, xgc, x1, BY(y), x2, BY(y));
    }

    // Drawing top and bottom walls
    y = Y_VISIB;
    for (x=0; x< X_VISIB+1; x++){
      XDrawLine(dis, win, xgc, FX(x), FY(0), BX(x), BY(0));
      XDrawLine(dis, win, xgc, FX(x), FY(y), BX(x), BY(y));
    }

    // Drawing right and left walls
    x = X_VISIB;
    for (y=0; y< Y_VISIB+1; y++){
      XDrawLine(dis, win, xgc, FX(0), FY(y), BX(0), BY(y));
      XDrawLine(dis, win, xgc, FX(x), FY(y), BX(x), BY(y));
    }


  }

  /** Draw the debug mode 3
    *
    * \param vGraph The object used to draw
    *
    */
  void draw_game_debug_mode_3(graphics_t* vGraph ){
    //    draw_game_debug_mode_2(vGraph);
    Display*  dis = vGraph->dis;

    int x,y;

    map=init_map(true);

    for (y=0; y<Y_VISIB; y++){
      for (x=0; x<X_VISIB; x++){
	printf("%c", MAP(x,y));
	if (!IS_WALL(MAP(x,y))){


	  // Draw case
	  //	  draw_case(vGraph, x, y);

	  // Draw north
	  if (y == 0 || (IS_WALL(MAP(x,y-1)))){
	    draw_north_wall();
	    
	  }
	  // Draw south
	  if (y == Y_VISIB || (y < Y_VISIB && IS_WALL(MAP(x,y+1)))){
	    draw_south_wall();
	  }

	  // Draw west
	  if (x == 0 || (x > 0 && IS_WALL(MAP(x-1,y)))){
	    draw_west_wall();
	  }

	  // Draw east
	  if (x == X_VISIB || (x < X_VISIB && IS_WALL(MAP(x+1,y)))){
	    draw_east_wall();
	  }


	}
      }
      printf("\n");
    }

    XFlush(dis);
    free_map(map);
  };

  /** Log the player in std out */
  int game_log_player(void){
    DEBUG_PLAYER(player);
    return 0;
  }



#endif //XA_DEBUG
