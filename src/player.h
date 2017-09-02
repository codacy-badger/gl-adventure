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

/** \file player.h
  * Declares structures and functions used to handle player
  *
  *
  */

#include "inventory.h"
#include "def.h" // for bool
#include "hud.h" // for current_selected_item

#ifndef _PLAYER_H_
#define _PLAYER_H_

/** Defines how the fighter/wizard level is used in food/water down time 
  * computation.
  *
  */
#define FOOD_LEVEL_FACTOR 4

/** Define an attribute
  *
  * It is used for life or mana.
  *
  */
typedef struct{
  int value;  //!< The current amount of this attribute
  int max;    //!< The max amout you get after a rest
}player_attribute_t;

/** Defines a level
  *
  * Level can be used for jobs (fighter, wizard...)
  *
  */
typedef struct{
  unsigned short level;          //!< Your current level
  unsigned int next_level_xp;    //!< The xp number you need to reach to 
                                 //   change level up
  unsigned int current_level_xp; //!<The current amout of xp you have to
                                 // change level
				   
}player_level_t;

/** Debug the player if in debug mode
  *
  * It is used to avoid conditional compilation from the caler.
  * Does nothing if in release mode.
  *
  * \param pl The player to debug
  *
  */
#ifdef XA_DEBUG
#  define DEBUG_PLAYER(pl) (debug_player(pl))
#else // XA_DEBUG
#  define DEBUG_PLAYER(pl) //
#endif // XA_DEBUG

/** Defines a player
  *
  * The overall level is raise when fighter or wizard level raise.
  *
  */
typedef struct {
  char* name;                 //!< The player name
  unsigned int level;         //!< The overall level

  unsigned int gold;          //!< Your bank account
  unsigned int points;        //!< For the highscore
  unsigned int moves;         //!< Number of moves
  unsigned int time;          //!< The in-game time in seconds

  player_attribute_t food;    //!< The player food
  player_attribute_t water;   //!< The player water

  player_attribute_t life;    //!< The player life attribute
  player_attribute_t mana;    //!< The player mana attribute

  player_level_t fighter;     //!< Your fighter level
  player_level_t wizard;      //!< Your fighter level

  unsigned int strength;      //!< The strength of the character
  unsigned int intelligence;  //!< The intelligence of the character

  double current_charge;      //!< The maximum charge  
  double max_charge;          //!< The total weight of objcts 

  char* why_death;            //!< The reason of the death;

  inventory_t* inventory;     //!< The inventory list

  float foodDownTime;         //!< The time to loose food in seconds
  float waterDownTime;        //!< The time to loose water in seconds
}player_t;

player_t* init_player(void);
void free_player(player_t*);

void init_attribute(player_attribute_t*);
void init_level(player_level_t*);

void init_food(player_attribute_t*, unsigned int);

void player_decrease_life(player_t*, unsigned int, const char*);
bool player_decrease_food(player_t*, int);
bool player_decrease_water(player_t*, int);

void player_increase_food(player_t*, int);
void player_increase_water(player_t*, int);

void player_remove_why_death(player_t*, const char*);

void player_increase_time(player_t*);
void player_move(player_t*);

void player_use_item(player_t*);

void player_compute_charge(player_t*);

#ifdef XA_DEBUG
void debug_player(player_t*);
#endif // XA_DEBUG

#endif // _PLAYER_H_
