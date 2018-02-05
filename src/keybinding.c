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

/** \file keybinding.c
  * Implementation of the functions handling the keybindings
  *
  */
#include "keybinding.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h> // For memset

#include "logger.h"
#include "def.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext(String)

/** Init a new key binding list
  * 
  * \warning The returned list contains no bindings. The corresponding
  *          \ref key_binding_list_t::bindings pointer is NULL.
  *
  * \return The newly created list
  *
  */
key_binding_list_t* init_key_binding_list(void){
  LOG("Initializing key binding structure");
  key_binding_list_t* kb_list = malloc(sizeof(key_binding_list_t));
  kb_list->nb_binding = 0;
  kb_list->bindings   = NULL;

#ifdef XA_DEBUG
  test_fncptr_returned_value();
#endif // XA_DEBUG
  return kb_list;
}

/** Add a key binding to an existing list
  *
  * \param vList   The list
  * \param vKeysym The XLib keysym
  * \param vFunct  The function pointer
  *
  * \return A pointer to the modified list.
  *
  */
key_binding_list_t* add_binding(key_binding_list_t* vList, int vKeysym, fncptr_t vFunct){

  assert(vList && "vList is NULL. Cannot add binding.");
  LOG("Adding new binding");

  // Realloc memory for bindings
  vList->nb_binding++;
  size_t new_bindings_size = vList->nb_binding * sizeof(key_binding_t*);
  LOGI("  New bindings size is %d bytes", new_bindings_size);
  vList->bindings =(key_binding_t**)realloc(vList->bindings, new_bindings_size);
  assert(vList->bindings && "Failed to realloc bindings size");
  LOG("  New bindings allocated");
  

  key_binding_t* new_binding = malloc(sizeof(key_binding_t));
  vList->bindings[vList->nb_binding - 1]= new_binding;
  assert(new_binding && "Failed to get new binding's address");
  LOGI("  Setting keysym 0x%X to new binding", (int)vKeysym);
  new_binding->x_keysym = vKeysym;
  LOG("  Setting function pointer to new binding");
  //  assert(new_binding->fnc_ptr 
  // && gettext("function pointer of new binding is NULL"));

  new_binding->fnc_ptr = vFunct;

  LOGI("Binding list now contains %d bindings.", vList->nb_binding);
  return vList;
}

/** Detroy the given list
  *
  * \param vList The list to be freed
  *
  */
void destroy_key_binding_list(key_binding_list_t* vList){
  unsigned int i;

  LOG("Destroying key binding structure");
  if (vList->bindings && vList->nb_binding > 0){
    LOG("Destroying binding lists");

    for (i=0; i < vList->nb_binding; i++){
      free(vList->bindings[i]);
    }

    free(vList->bindings);
  }
  free(vList);
};

/** Get a binding by its index
  *
  * \param vList  The list of binding
  * \param vIndex The index of the binding
  *
  * \return The binding
  *
  */
key_binding_t* get_addr_by_index(key_binding_list_t* vList, 
				 unsigned int vIndex){

  assert(vIndex <= vList->nb_binding && 
	 "Index of binding out of bound");

  return vList->bindings[vIndex];
}

/** Handle XLib event 
  *
  * \param vList The key binding list to test
  * \param key   The X keysim to test
  *
  * \return -1 if no binding was find, otherwise, return the
  *         result of the function pointer of the first binding 
  *         we found.
  *
  */
int handle_kb_event(key_binding_list_t* vList, KeySym key){
  assert(vList && "Cannot handle event on NULL binding list pointer");
  LOG("handle_kb_event() called");
  unsigned int i;
  key_binding_t* binding;

  if (vList->nb_binding > 0){
    for (i=0; i < vList->nb_binding; i++){
      binding = get_addr_by_index(vList, i);
      LOGI("  Binding address is %d", (int)binding);
      LOGI("  Testing keysim 0x%X", (int)binding->x_keysym);
      assert(binding && "Cannot get binding address by index");
      
      // If key ok, execute function pointer
      if (key == binding->x_keysym){
	LOG("Key binding found. Calling function pointer.");
	assert(binding->fnc_ptr && "Cannot call NULL function pointer");
	int ret =  (*binding->fnc_ptr)();
	return ret;
      }
      binding += sizeof( key_binding_t );
    }
  }
  return -1;
}


// ------------------------------------------
// UNIQUEMENT POUR DEBUG
// ------------------------------------------
#ifdef XA_DEBUG
  /** Tests all returned values of the function pointer
    *
    */
  void test_fncptr_returned_value(void){
    test_one_fncptr_returned_value(-1);

    test_one_fncptr_returned_value(0);
    test_one_fncptr_returned_value(1);
    test_one_fncptr_returned_value(2);
    test_one_fncptr_returned_value(3);

    // Changing state + New state = 12 + redraw
    test_one_fncptr_returned_value((12 << 3) + 6);
    // Changing state + New state = 2500 + redraw
    test_one_fncptr_returned_value((2500 << 3) + 4);
  }

  /** Tests a single returned values of the function pointer
    *
    * \param vInt The value to test
    *
    */
  void test_one_fncptr_returned_value(int vInt){
    const char* quit;
    const char* redraw;
    const char* ch_st;

    quit   = MUST_QUIT(vInt) ? "true" : "false";
    redraw = MUST_DRAW(vInt) ? "true" : "false";
    ch_st  = MUST_CHST(vInt) ? "true" : "false";
 
    LOGI("Testing fncptr returned value : %d", vInt);
    LOGS("  MUST_QUIT : %s", quit);
    LOGS("  MUST_DRAW : %s", redraw);
    LOGS("  MUST_CHST : %s", ch_st);
    LOGI("    new state= %d", GET_STATE(vInt));
  }

  /** Debug the key binding list
    *
    * \param vList The list to debug
    *
    */
  void debug_key_binding_list(key_binding_list_t* vList){
    unsigned int i;
    key_binding_t* bnd; 

    LOG("Debugging key binging list :");
    LOGI("  contains %d bindings",vList->nb_binding);

    for (i=0; i < vList->nb_binding; i++){
      bnd = get_addr_by_index(vList, i);
      assert(bnd && "Cannot get key binding");
      LOGI("  Binding address is %d", (int)bnd);
      LOGI("    Contains binding #%d",i);
      LOGI("      Keysym=0x%X", bnd->x_keysym);
      LOGI("      Fncptr=%p", (int)bnd->fnc_ptr);
    }
  }

#endif // XA_DEBUG
