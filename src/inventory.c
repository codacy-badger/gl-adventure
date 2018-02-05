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

/** \file inventory.c
  * Implements an inventory list.
  *
  *
  */

#include "inventory.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logger.h"

/** Creates an inventory list
  *
  * \return The new list
  *
  */
inventory_t* create_inventory_list(void){
  inventory_t* sl = (inventory_t*)malloc(sizeof(inventory_t));
  sl->items = NULL;
  sl->size = 0;
  return sl;
}

/** Free the given inventory
  *
  * \param iv The inventory to be freed
  *
  */
void free_inventory_list(inventory_t* iv){
  unsigned int i;
  for (i=0; i<iv->size; i++){
    free(iv->items[i]->name);
    free(iv->items[i]);
  }

  free(iv->items);
  free(iv);
}

/** Tests if an item already exist in the list
  *
  * \param iv   The inventory list
  * \param text The item to be searched for
  *
  * \return \c true if found
  *
  */
bool inventory_list_exists(inventory_t* iv, char* text){
  unsigned int i;
  for (i=0; i<iv->size; i++){
    if (strcmp(iv->items[i]->name, text)==0){
      return true;
    }
  }
  return false;
}

/** Adds a new item in the list
  *
  * \param vList The inventory list
  * \param vText The item to be added
  *
  */
void inventory_list_add(inventory_t* vList, char* vText){

  if (inventory_list_exists(vList, vText)){
    unsigned int i;
    for (i=0; i<vList->size; i++){
      if (strcmp(vList->items[i]->name, vText)==0){
	vList->items[i]->number++;
      }
    }
  }
  else{
    // Adds a new item
    vList->size++;
    size_t s = (vList->size)  * sizeof(inventory_item_t*);
    vList->items = (inventory_item_t**)realloc(vList->items, s);
    
    vList->items[vList->size-1]=
      (inventory_item_t*)malloc(sizeof(inventory_item_t));
    
    vList->items[vList->size-1]->name = (char*)malloc(strlen(vText)+1);
    strcpy ( vList->items[vList->size-1]->name, vText );
    
    vList->items[vList->size-1]->number=1;
  }
}

/** Removes an object from an inventory
  *
  * \param iv   The inventory to remove the object from
  * \param text The name of the object to be removed
  *
  */
void inventory_list_remove(inventory_t* iv, char* text){
  unsigned int i;
  for (i=0; i<iv->size; i++){
    if (strcmp(iv->items[i]->name, text)==0){
      iv->items[i]->number--;
      return true;
    }
  }

}


#ifdef XA_DEBUG
  /** Test the inventory list integrity
    *
    */
  void inventory_list_self_test(void){
    LOG("inventory_list_self_test called");
    inventory_t* i = create_inventory_list();

    inventory_list_add(i, "Chicken");
    inventory_list_add(i, "Chorizo");
    inventory_list_add(i, "Chicken");

    inventory_list_print(i);

    free_inventory_list(i);
  }

  /** Prints the given list's content
    *
    * \param iv The inventory list
    *
    */
  void inventory_list_print(inventory_t* iv){
    LOG("inventory_list_print called")
    assert(iv && "Cannot print NULL inventory");

    if (iv->size==0 && iv->items==NULL){
      printf("Inventory list is empty\n");
    }
    else{
      unsigned int i;
      for (i=0; i<iv->size; i++){
	printf("%s => %d\n", iv->items[i]->name, iv->items[i]->number);
      }
    }
  }
#endif // XA_DEBUG
