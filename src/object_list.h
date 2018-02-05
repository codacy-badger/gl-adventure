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

/** \file object_list.h
  * Declaration of the loaded object list
  *
  * These types anf function are used with objects file (i.e. data/objects.dat).
  *
  */

#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_

#include "def.h"

/** Defines the type of object
  *
  * This must be the same in \c data/objects.dat.
  *
  */
typedef enum {
  OT_FOOD  = 001, //!< An object providing food
  OT_WATER = 002, //!< An object providing water

  OT_UNSET = 999  //!<Object type was not set
}object_type_t;

/** Get or set the Food object value */
#define V_FOOD(o)  o->val_int 
/** Get or set the Water object value */
#define V_WATER(o) o->val_int 

/** \def DEBUG_OBJLST
  * Debug the given object list if in debug mode
  *
  * \param l The list to log out
  *
  * \note In release mode, this define does nothing.
  *
  */
#ifdef XA_DEBUG
#  define DEBUG_OBJLST(l) debug_object_list(l)
#else // XA_DEBUG
#  define DEBUG_OBJLST(l) //
#endif // XA_DEBUG


/** Defines an object 
  *
  * This structure defines attributes common to all objects (\ref type, 
  * \ref id ...) as an anonymous union that contains specific attributes
  * depending of object's type.
  *
  */
typedef struct{
  object_type_t type; //!< The object's type
                      //!< \sa \ref object_type_t
			
  char*         id;   //!< The alpha numeric identifier
                      //!< Objects are identified by this id. It cannot
                      //!< contains whitespaces.
  char*         en;   //!< The english name of the object
  char*         fr;   //!< The french name of the object */
  char*      en_desc; //!< English description
  char*      fr_desc; //!< French description

  unsigned int weight;//!< The weight of the object in gramms
  double find_percent;//!< Percentage of chance to find

  /** The anonymous union used for specific object's type values
    *
    * The usage of these values depends on the type of the object.
    *
    */
  union{
    int    val_int;   //!< The value used for single integer defined types
                      //!<
                      //!< The object of type \ref OT_FOOD or \ref OT_WATER
                      //!< use this values to contain the raising level of
                      //!< the object when used.
  };

}object_t;

/** The list of object
  *
  */
typedef struct{
  object_t** objects; //!< The object array
  unsigned int size;  //!< The size of the list
  double find_total;  //!< The total of find_percent of all objects
}object_list_t;

object_list_t* create_object_list();
void free_object_list(object_list_t*);

object_t* create_object(void);
void free_object(object_t*);

void add_object(object_list_t*, object_t*);
object_t* get_random_object(object_list_t*);
object_t* get_object_by_id(object_list_t*,char*);

bool id_exists(object_list_t*,char*);

object_list_t* object_list_pop_back(object_list_t*);

#ifdef XA_DEBUG
  void debug_object_list(object_list_t*);
  void debug_object(object_t*);
  const char* object_type_string(const object_type_t);
#endif // XA_DEBUG


#endif // _OBJECT_LIST_H_
