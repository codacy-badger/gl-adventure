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

/** \file stringlist.c
  * Implementation of a C-string list
  *
  *
  */

#include "stringlist.h"

#include <stdlib.h>
#include <stdio.h>

#include "logger.h"

/** Creates and returns a strin list
  * 
  * \return A pointer to the newly created list
  *
  */
string_list_t* create_string_list(void){
  string_list_t* sl = (string_list_t*)malloc(sizeof(string_list_t));
  //  sl->data = (char**)malloc(sizeof(char**));
  sl->data = NULL;
  sl->size = 0;
  return sl;
}

/** Free the given stringlist
  *
  * \param vList The list to free
  *
  */
void free_string_list(string_list_t* vList){
  int i=0;
  for (i=0; i< vList->size; i++){
    free(vList->data[i]);
  }

  if (vList->data) free(vList->data);
  free(vList);
}

/** Adds a text to a list
  *
  * \param vList The list
  * \param vText The text to add
  *
  */
void string_list_add(string_list_t* vList, const char* vText){
  vList->size++;
  size_t s = (vList->size)  * sizeof(char*);
  vList->data = (char**)realloc(vList->data, s);
  
  vList->data[vList->size-1] = (char*)malloc(strlen(vText)+1);
  strcpy ( vList->data[vList->size-1], vText );
}

#ifdef XA_DEBUG
  /** Log in std out the content of the given list
    *
    * \param vList The list to debug
    *
    * \note This function is built in debug mode only.
    *
    */
  void string_list_debug(string_list_t* vList){
    int i;

    LOG("Debugging string list :");
    printf("  list size : %d\n", vList->size);
    printf("  content   :\n");

    for (i=0; i<vList->size; i++){
      printf("    %d:%s\n", i, vList->data[i]);
    }
  }
#endif // XA_DEBUG
