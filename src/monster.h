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

/** \file monster.h
  * Header file that defines a monster.
  *
  */
#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "map.h"
#include "player.h"


/// A type that defines a milliseconds time
typedef unsigned int msec;

/** Some constants definings standard directions
  *
  * These values are ready to be use with the \ref monster_t::direction
  * member and based on the OpenGL glTranslate function.
  *
  */
enum DIRECTION_CONST{
  NONE  = -1,  //!< No direction specified
  NORTH = 0,   //!< Going to the north
  WEST  = 90,  //!< Going to the west
  SOUTH = 180, //!< Going to the east
  EAST  = 270  //!< going to the south
};

/** Defines the current state of a monster
  *
  */
typedef enum{
  THINKING =0, //!< Thinking about the next move
  TURNING,     //!< Turning around to another direction
  MOVING       //!< Moving to the next case
}monster_state_t;

// Forward declaration
struct _monster_t;
// End of forward declaration

/** The thinker function pointer
  *
  * This function pointer is called after think time elapsed.
  *
  */
typedef void (*thinker_fncptr_t)(struct _monster_t*, map_t*, player_t*);

/** The memory initialization function poiner
  *
  * This function must return the custome memory pointer, casted to void*. 
  * The \ref _monster_t::memory member is initialized by this function pointer.
  *
  * \sa \ref mon_thinker_section
  *
  */
typedef void* (*thinker_initmem_fncptr_t)(void);

/** The memory freeing function poiner
  *
  * This function must free the custom memory pointer, casted to void*, passed
  * as argument.
  *
  * \sa \ref mon_thinker_section
  *
  */
typedef void (*thinker_freemem_fncptr_t)(void*);

/** A structure that define a monster
  *
  * \section mon_intro_section Introduction
  *
  * This structure handles members needed to draw the monster
  * and to compute AI decision and moves.
  *
  * \section mon_name_section Structure name
  *
  * The structure is named with an underscore while the typedef
  * do not. This difference is used to forward declaring the
  * underscore-prefixed struct.
  *
  * \section mon_thinker_section Thinker and memory
  *
  * The \ref _monster_t::thinker member is a function pointer to a thinker function. 
  * Sometimes, the thinker must keep track of some values, it is called 
  * a \ref _monster_t::memory. This member is a void*, so it can handle any type of 
  * data structure. It is initialized by the \ref _monster_t::init_memory 
  * member and freed by the \ref _monster_t::free_memory one. These two 
  * function pointer are called when needed. Do not call these yourself.
  *
  */
typedef struct _monster_t{
  msec think_time; //!< The time spent to think about next action
  msec move_time;  //!< The time needed to move to the next case
  msec turn_time;  //!< The time needed to turn 360 degrees
  
  int x;           //!< X position (in case number)
  int y;           //!< Y position (in case number)

  /** The current direction in degrees 
    *
    * \sa the \ref DIRECTION_CONST enumeration for standard values
    *
    */
  float direction;

  /** We are moving to which direction
    * 
    * The value of this member must be :
    * - one of the \ref DIRECTION_CONST values;
    * - -1 if we are not turning around;
    *
    */
  int to_direction;
  /// The direction before turning
  int from_direction;
  
  /** This values is needed to place th monster to the next case
    *
    * This can be:
    * - A number between 0 and 100 (0 is 'in previous case', 100 if 
    *   move is finished);
    * - -1 if no move started;
    *
    */
  int to_nextcase;

  monster_state_t state; //!< The state of this monster
  msec state_time;       //!< The game time the last state start
  void* memory;          //!< Teh thinker memory pointer

  /** The AI thinker function pointer */
  thinker_fncptr_t thinker;
  /// The memory initialization function pointer
  thinker_initmem_fncptr_t init_memory;
  /// The memory dealocation function pointer
  thinker_freemem_fncptr_t free_memory;

}monster_t;



monster_t* init_monster(int, int);
void free_monster(monster_t*);

void draw_monster(monster_t*);
void advance_monster(monster_t*, map_t*, player_t*);

#endif // _MONSTER_H_
