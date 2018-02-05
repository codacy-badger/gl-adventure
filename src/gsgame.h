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

#ifndef _GS_GAME_H_
#define _GS_GAME_H_

/** \file gsgame.h
  * The declaration of the game gamestate.
  *
  *
  */

#include "load_objects.h"
#include "gamestate.h"
#include "graphics.h"
#include "player.h"
#include "map.h"

/** The visibility in number of cases */
#define X_VISIB 11
/** The visibility in number of cases */
#define Y_VISIB 11
/** The map height in pixels of deviation */
#define MAP_HEIGHT 30
/** Acces to the foreground X axis */
#define FX(idx) (fore_axe_x[idx])
/** Acces to the foreground Y axis */
#define FY(idx) (fore_axe_y[idx])
/** Acces to the background X axis */
#define BX(idx) (back_axe_x[idx])
/** Acces to the background Y axis */
#define BY(idx) (back_axe_y[idx])

////// DEATH REASON MESSAGES
/** The walk in wall death message */
#define DRM_WIW "Walking in wall"

/** \def MAP(x, y)
  * Shortcut access to a map case 
  *
  * \warning x and y must be inside round brackets '()' to avoid
  *          heavy bugs when calling with x+1 or y+1...
  *
  * \param x, y Position in the map
  *
  */
#define MAP(x,y) (map->case_list[(x) + ((y)*map->width)].value)

/** \def LOBJ(x,y)
  * Shortcut access to a map case's objects
  *
  * \warning x and y must be inside round brackets '()' to avoid
  *          heavy bugs when calling with x+1 or y+1...
  *
  * \param x, y Position in the map
  *
  */
#define LOBJ(x,y) (map->case_list[(x) + ((y)*map->width)].objects)


/** \typedef axe_pos_t
  * Defines a value in an axe plan
  *
  * It is defined as \c int by default but with the presence of the 
  * \c XA_MAP_USHORT, added through makefile, we can type it as 
  * <code>unsigned short</code>.
  *
  */
#ifdef XA_MAP_USHORT
  typedef unsigned short axe_pos_t;
#else 
  typedef int axe_pos_t;
#endif

/** The type of an plan axis */
typedef axe_pos_t* axe_pos_list_t;

/** The foreground X axis */
axe_pos_list_t fore_axe_x;

/** The foreground Y axis */
axe_pos_list_t fore_axe_y;

/** The background X axis */
axe_pos_list_t back_axe_x;

/** The background Y axis */
axe_pos_list_t back_axe_y;

/** The current map */
map_t* map;
/** The current player */
player_t* player;

/** The object list loaded by gsgame
  *
  * \warning This must be accessible from load_map.
  *
  */
object_list_t* object_list;

extern float map_zoom;

/** Are we showing the character dialog ? */
bool showing_character_dialog;
/** Are we showing the inventory dialog ? */
bool showing_inventory_dialog;

game_state_t* init_gs_game(void);
void free_gs_game(game_state_t*);

int draw_gs_game(void);
int game_back_to_intro(void);

void draw_map(void);
void draw_message(void);

void feed_axe_plans(void);
void draw_grid(graphics_t*, XColor, int, int, int, int);

void draw_player(void);
void draw_finish(void);
void draw_floor(void);
void draw_case_object(void);
void draw_north_wall(void);
void draw_west_wall(void);
void draw_south_wall(void);
void draw_east_wall(void);

int game_move_left(void);
int game_move_right(void);
int game_move_up(void);
int game_move_down(void);

void walk_in_wall(void);
void dont_walk_in_wall(void);

int game_toggle_char_dlg(void);
int game_toggle_inv_dlg(void);

bool game_timer_callback(void);

void show_death_message(graphics_t*);
void show_object_message(graphics_t*);

int game_collect_items(void);

int game_zoom_in(void);

int game_zoom_out(void);

void draw_objects_name(void);

#ifdef XA_DEBUG
  /** Cycle toggles from 0 (no debug to GAME_MAX_DEBUG_MODE) */
# define GAME_MAX_DEBUG_MODE 3

  /** The current debug mode in the cycle 
    *
    * This value should never be greater than \ref GAME_MAX_DEBUG_MODE.
    *
    */
  int current_debug_mode;

  int game_toggle_debug(void);
  void draw_game_debug(graphics_t*);

  void draw_game_debug_mode_1(graphics_t*);
  void draw_game_debug_mode_2(graphics_t*);
  void draw_game_debug_mode_3(graphics_t*);
  int game_log_player(void);
#endif //XA_DEBUG

#endif // _GS_GAME_H_
