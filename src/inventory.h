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

/** \file inventory.h
  * Declares an inventory list. It is a list where objects are stacked.
  *
  *
  */

#include "def.h" // for bool

#ifndef _INVENTORY_LIST_H_
#define _INVENTORY_LIST_H_

/** \def INVENTORY_TEST
  * Launch the inventory list self test if in debug mode
  *
  */
#ifdef XA_DEBUG
#  define INVENTORY_TEST inventory_list_self_test()
#else
#  define INVENTORY_TEST //
#endif // XA_DEBUG

/** An inventory item
  *
  */
typedef struct{
  char * name;         //!< The object ID
  unsigned int number; //!< The number of item
}inventory_item_t;

/** An inventory list
  *
  */
typedef struct{
  inventory_item_t** items; //!< The item array
  unsigned int size;        //!< The size of the inventory
}inventory_t;

inventory_t* create_inventory_list(void);
void free_inventory_list(inventory_t*);

void inventory_list_add(inventory_t*, char*);
void inventory_list_remove(inventory_t*, char*);
bool inventory_list_exists(inventory_t*, char*);

#ifdef XA_DEBUG
  void inventory_list_self_test(void);
  void inventory_list_print(inventory_t*);
#endif // XA_DEBUG


#endif // _INVENTORY_LIST_H_
