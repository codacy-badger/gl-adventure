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

/** \file map.c
  * Implementation of the map
  *
  */

#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "logger.h"
#include "gsgame.h"
#include "def.h"


/** Initialize and return a map
  *
  * \param feed Should we feed the map with example datas
  *
  * \return The newly created map
  *
  */
map_t* init_map(bool feed){
#ifdef XA_DEBUG
  test_walls();
#endif // XA_DEBUG

  map_t* my_map = (map_t*)malloc(sizeof(map_t));
  LOGI("Declaring %d bytes for the map", sizeof(map_t));

  // Default value
  my_map->name      = NULL;
  my_map->case_list = NULL;
  my_map->width     = 0;
  my_map->height    = 0;
  my_map->depth     = 0;
  init_map_pos(&my_map->start, 0);
  init_map_pos(&my_map->finish, 0);
  init_map_pos(&my_map->player, 0);


  if (feed){
    size_t st_case = sizeof(map_case_t);
    size_t st_cases = st_case * MAP_W * MAP_H;
    my_map->case_list = (map_case_t*)malloc(st_cases);
    LOGI("Declaring %d bytes for the map's cases", st_cases);

    //   feed_case_list(my_map);
    LOGI("Case list correctly fed", st_cases);
  }
  return my_map;
}

/** Initialize a map_pos_t structure to the given value
  *
  * \param pos The map_pos_t structure pointer
  * \param val The value to set to
  *
  */
void init_map_pos(map_pos_t* pos, unsigned char val){
  pos->x = val;
  pos->y = val;
}

/** Free the given map
  *
  * The memory allocated for the map will be freed.
  *
  * \param vMap The map to destroy
  *
  */
void free_map(map_t* vMap){
  LOG("Destroying the map");
  int x, y, idx;

  // For each case, free object id list
  for (x= 0; x <  vMap->width; x++){
    for (y = 0; y < vMap->height; y++){
      idx = x + (y*vMap->width);
      if (vMap->case_list[idx].objects != NULL){
	free_string_list(vMap->case_list[idx].objects);
      }
    }
  }

  free(vMap->case_list);
  vMap->case_list = NULL;

  free(vMap->name);
  vMap->name = NULL;

  free(vMap);
  vMap = NULL;
}

/** Feed the given map with a content
  *
  * \param vMap The map to feed
  *
  */
void feed_case_list(map_t* vMap){
  // MAP_CONTENT
  int x, y, idx;

  idx=0;

  // Alloc a temp string
  char temp_c[MAP_W * MAP_H] = MAP_CONTENT;

  for (x= 0; x <  MAP_W; x++){
    for (y = 0; y < MAP_H; y++){
      idx = x + (y*MAP_W);
      vMap->case_list[idx].value = temp_c[idx];
      vMap->case_list[idx].objects = NULL;
    }
  }
  //  free(temp_c);
}

/** Adds an object to a case position
  *
  * \param pos The case position
  * \param id  The object id
  *
  */
void case_add_object(map_case_t* pos, char* id){
  LOGS("case_add_object called for id %s", id);
  if (pos->objects == NULL){
    pos->objects = create_string_list();
  }
  string_list_add(pos->objects, id);
}


//////////////////////////////////////////
// ONLY IN DEBUG
#ifdef XA_DEBUG

/** Test individual wall values
  *
  * It test values using the \ref test_one_wall() function.
  *
  * \note This function is only built in debug mode
  *
  */
void test_walls(void){
  test_one_wall('0');
  test_one_wall('1');
}

/** Test one wall value
  *
  * \param vChar The char to be tested
  *
  * \note This function is only built in debug mode
  *
  */
void test_one_wall(char vChar){

  if (IS_WALL(vChar)){
    LOGC("The character '%c' returns value 'true' for IS_WALL", vChar);
  }
  else{
    LOGC("The character '%c' returns value 'false' for IS_WALL", vChar);
  }

}

/** Debug the given map
  *
  * \param map The map to debug
  *
  * \note This function is only built in debug mode
  *
  */
void debug_map(map_t* map){
  int x, y;

  assert(map && "Debugging NULL map");
  assert(map->name && "Spurious map name. Exitting!");
  printf("Debugging map :\n");
  printf("  name   = %s\n", map->name); 
  printf("  width  = %d\n", map->width); 
  printf("  height = %d\n", map->height); 
  printf("  depth  = %d\n", map->depth); 
  printf("\n"); 
  printf("  start  = %d, %d\n", map->start.x, map->start.y); 
  printf("  finish = %d, %d\n", map->finish.x, map->finish.y); 

  printf("  data :\n");
  for (y=0; y<map->height; y++){
    for (x=0; x<map->width; x++){
      printf("%c", MAP(x,y));
    }
    printf("\n");
  }

}

#endif //XA_DEBUG
