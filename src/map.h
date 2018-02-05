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

/** \file map.h
  * The header file defining a map
  *
  *
  */

#ifndef _MAP_H_
#define _MAP_H_

#include <stdlib.h>
#include <string.h>

#include "stringlist.h"
#include "def.h"

/** \def DEBUG_MAP
  *
  * \param map The map to debug
  *
  * Calls the \ref debug_map function if in debug mode. Does nothing in 
  * release mode. It is defined to avoid conditional compilation of caller.
  *
  */
#ifdef XA_DEBUG
#  define DEBUG_MAP(map) debug_map(map)
#else // XA_DEBUG
#  define DEBUG_MAP(map) //
#endif // XA_DEBUG
/** A case of the map
  *
  * Value is as follow :
  * - bit 1 : 1=wall, 0=non-wall
  *
  */
typedef struct {
  char value;             //!< The case value
  string_list_t* objects; //!< Object id list
                          //!<
                          //!< If no objects are present, its NULL.
}map_case_t;

/** Defines a position in the map
  *
  *
  *
  */
typedef struct{
  unsigned char x; //!< The X position
  unsigned char y; //!< The Y position
}map_pos_t;

/** A map
  *
  *
  *
  */
typedef struct {
  char* name;            //!< The map name
  unsigned char width;   //!< The width of the map in cases 
  unsigned char height;  //!< The height of the map in cases 
  unsigned char depth;   //!< The depth of the map

  map_pos_t start;       //!< The start position
  map_pos_t finish;      //!< The end of level position
  map_pos_t player;      //!< The position of the player

  map_case_t* case_list; //!< The case list
}map_t;


/** The width of the map*/
#define MAP_W 11
/** The height of the map*/
#define MAP_H 11

/** The content of the map
  *
  * Please see \ref map_case_t documentation for meanings.
  *
  */
#define MAP_CONTENT 				\
    "11101111111"				\
    "10001000011"				\
    "10001000011"				\
    "10001000011"				\
    "10111000011"				\
    "10000001111"				\
    "10000111011"				\
    "10111100011"				\
    "10000000011"				\
    "10000000011"				\
    "11111111111"    

/** Used to test the `value` member of map_case_t */
//#define IS_WALL(c) (c & 0x01)
#define IS_WALL(c) ((c) == '1')

// Functions
map_t* init_map(bool);
void feed_case_list(map_t*);
void free_map(map_t*);

void init_map_pos(map_pos_t* , unsigned char );
void case_add_object(map_case_t*, char*);

#ifdef XA_DEBUG
  void test_walls(void);
  void test_one_wall(char);
  void debug_map(map_t*);
#endif // XA_DEBUG

#endif // _MAP_H_
