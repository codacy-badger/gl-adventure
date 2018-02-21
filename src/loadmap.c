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

/** \file loadmap.c
  * Implamentation of the functions that loads a map
  *
  *
  */

#include "loadmap.h"

#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "object_list.h"
#include "gsgame.h"
#include "logger.h"


/** Return a random map
  *
  * \return A map
  *
  */
map_t* get_random_map(void){

  map_t* map = init_map(false);
  assert(map && "Map initialization failed");
  
  int nb = get_map_directpry_size(false, 0, NULL);
  assert(nb && "No map was found");
  
  int rand_map = rand() % nb;

  LOGI("Number of maps found : %d", nb);
  LOGI("Loading map number : %d", rand_map);
  
  return read_map_directory(rand_map);
}


/** Reads the map directory and try to load the given maps index
  *
  * \param load    If true, load the map whith index map_idx
  * \param map_idx The index of the map to load
  * \param map     The map that will contain loaded one
  *
  * \return Number of map found in the directory
  *
  */
map_t* read_map_directory(int map_idx){
  LOGI("Reading map directory '%i'", map_idx);
  int map_number=0;
  int res;
  struct dirent entry;
  struct dirent* result;
  char last_char;


  DIR* dir = opendir(MAP_DIR);
  assert(dir && "Cannot open maps/ dirctory");

  do {
    res= readdir_r(dir, &entry, &result);
    assert(res==0 && "Cannot read maps/ dirctory");
    if (result){
      // Get the last char of the filename
      last_char = result->d_name[strlen(result->d_name)-1];

      if (last_char == '~'){
	LOGS("'%s' is a backup. Do not loading.", result->d_name);
      }
      else if (strcmp(result->d_name, ".")==0){
	LOG("Do not loading '.' file.");
      }
      else if (strcmp(result->d_name, "..")==0){
	LOG("Do not loading '..' file.");
      }
      else{
	return load_map( result->d_name);
      }
    }
  }
  while (result!=NULL);
  closedir(dir);
  return NULL;
}

/** Loads a map
  *
  * \param map_name The filename of the file containing the map
  * \param map      The map that will contain loaded one
  *
  */
map_t* load_map(const char* map_name){
  map_t* map = init_map(false);

  current_data_line = 0;

  assert(map && "map pointer is null");
  LOGS("Loading map `%s`", map_name);

  map_loading_t load=0;

  // Allocate full name memory
  size_t fns = strlen(MAP_DIR) + strlen(map_name) + 1;
  char* full_name = (char*)malloc(fns * sizeof(char));
  assert(full_name && "Cannot allocate memory for full file name");

  // Creates full name
  strncpy (full_name, MAP_DIR, strlen(MAP_DIR));
  strncpy (full_name+strlen(MAP_DIR), map_name, strlen(map_name));
  full_name[fns-1] = '\0';
  assert(full_name && "Cannot concat full file name");
  LOGS("Reading : %s", full_name);

  // Open file
  FILE* f = fopen(full_name, "r");
  assert(f && "Cannot open map file");

  char content_nl[MAP_MAX_LINE];
  char content[MAP_MAX_LINE];
  while(fgets(content_nl, MAP_MAX_LINE, f)){
    memset(content, 0, MAP_MAX_LINE);
    // strips new lines
    if (content_nl[strlen(content_nl)-1]==10){
      strncpy (content, content_nl, strlen(content_nl)-1);
    }

    if (content[0] == '#'){
      // strips comments
    }
    else if (strlen(content)==0){
      // empty line
    }
    else{
      switch (load){
      case ML_MAGN:
	if (strcmp(content, MAGIC_NUMBER)!=0){
	  LOGS("Cannot load map '%s'. Spurious magic number.", map_name);
	  return NULL;
	}
	else{
	  load++;
	}
	break;

      case ML_NAME:
	LOGS("Map's name is %s", content);
	map->name = strdup(content);
	assert(map->name && "strfup() failed for map->name");
	
	LOGS("Map's name copied to '%s'", map->name);
	load++;
	break;
      
      case ML_SIZE:
	handle_map_size(content, map);
	load++;
	break;

      case ML_SPOS:
	handle_start_pos(content, map);
	load++;
 	break;

      case ML_EPOS:
	handle_end_pos(content, map);
	load++;
 	break;

      case ML_DATA:
	if (handle_map_data(content, map)){ 
	  LOG("Advancing to ML_OBJS");
	  load++;
	}
	break;
      case ML_OBJS:
	LOG("Handling objects position");
	handle_objects_pos(content, map);
	//	load++;
	break;
     }
    }
  }
  fclose(f);
  free(full_name);
  return map;
}

/** Get the map size from the given text
  *
  * This function also allocate memory for map
  *
  * \param val The text
  * \param map The map that will contain loaded one
  *
  */
void handle_map_size(char* val, map_t* map){
  char* map_width;
  char* map_height;
  char* map_depth;

  int imap_width;
  int imap_height;
  int imap_depth;

  printf("handle_map_size receive : %s\n", val);

  // The man page said "do not use strtopk"... so I use it :)
  map_width = strtok(val, " ");
  map_height = strtok(NULL, " ");
  map_depth = strtok(NULL, " ");
  assert (map_width && map_height && map_depth && 
	  "Cannot get correct map size");

  imap_width = atoi(map_width);
  imap_height = atoi(map_height);
  imap_depth = atoi(map_depth);

  printf ("map width = %d\n", imap_width);
  printf ("map height = %d\n", imap_height);
  printf ("map depth = %d\n", imap_depth);

  // Allocate memory for case list
  size_t st_case = sizeof(map_case_t);
  size_t st_cases = st_case *  imap_width * imap_height ;
  map->case_list = malloc(st_cases);
  assert(map->case_list && "Cannot allocate memory for case list");
  LOGI("Declaring %d bytes for the map's cases", st_cases);

  map->width = imap_width;
  map->height= imap_height;
  map->depth = imap_depth;
}

/** Get the start pos from the given text
  *
  * \param val The text
  * \param map The map that will contain loaded one
  *
  */
void handle_start_pos(char* val, map_t* map){
  char* start_x;
  char* start_y;

  int istart_x;
  int istart_y;

  printf("handle_start_pos receive : %s\n", val);

  start_x = strtok(val, " ");
  start_y = strtok(NULL, " ");
  assert (start_x && start_y && "Cannot get correct map size");

  istart_x = atoi(start_x);
  istart_y = atoi(start_y);

  printf ("start x = %d\n", istart_x);
  printf ("start y = %d\n", istart_y);
  
  map->start.x = istart_x;
  map->start.y = istart_y;

  map->player.x = istart_x;
  map->player.y = istart_y;
}

/** Get the end pos from the given text
  *
  * \param val The text
  * \param map The map that will contain loaded one
  *
  */
void handle_end_pos(char* val, map_t* map){
  char* end_x;
  char* end_y;

  int iend_x;
  int iend_y;

  printf("handle_end_pos receive : %s\n", val);

  end_x = strtok(val, " ");
  end_y = strtok(NULL, " ");
  assert (end_x && end_y && "Cannot get correct map size");

  iend_x = atoi(end_x);
  iend_y = atoi(end_y);

  printf ("end x = %d\n", iend_x);
  printf ("end y = %d\n", iend_y);

  map->finish.x = iend_x;
  map->finish.y = iend_y;
}

/** Loads and set map's data in map structure and return when it s finished
  *
  * \param l   The line of text from the file
  * \param map The map
  *
  * \return true if all lines of data was loaded
  *
  */
bool handle_map_data(char* l, map_t* map){
  int i;

  size_t l_len = strlen(l);
  assert(l_len == map->width && "Map's data lenght has wrong length.");

  printf("%s\n", l);

  for (i=0; i< l_len; i++){
    MAP(i, current_data_line) = l[i];
    LOBJ(i, current_data_line) = NULL;
  }
  current_data_line++;

  return (current_data_line >= map->height);
}

/** Handle an object position in the map file
  *
  * \param val The current map file content
  * \param map The current map
  *
  */
void handle_objects_pos(char* val, map_t* map){
  LOG("handle_objects_pos called");
  char *cx, *cy, *id;
  int x, y, idx;

  cx = strtok(val, " ");
  cy = strtok(NULL, " ");
  id = strtok(NULL, " ");

  x = atoi(cx);
  y = atoi(cy);

  idx = x + (y * map->width);
  if (strcmp(id, "RANDOM")==0){
    object_t* obj=get_random_object(object_list);
    assert (obj&& "Cannot get random object");
    case_add_object(&map->case_list[idx], obj->id);
  }
  else{
    case_add_object(&map->case_list[idx], id);
  }
}

int
get_map_directpry_size()
{
  int map_number=0;
  int res;
  struct dirent entry;
  struct dirent* result;
  char last_char;

  DIR* dir = opendir(MAP_DIR);
  assert(dir && "Cannot open maps/ dirctory");

  do {
    res= readdir_r(dir, &entry, &result);
    assert(res==0 && "Cannot read maps/ dirctory");
    if (result){
      // Get the last char of the filename
      last_char = result->d_name[strlen(result->d_name)-1];

      if (last_char == '~'){
	LOGS("'%s' is a backup. Do not loading.", result->d_name);
      }
      else if (strcmp(result->d_name, ".")==0){
	LOG("Do not loading '.' file.");
      }
      else if (strcmp(result->d_name, "..")==0){
	LOG("Do not loading '..' file.");
      }
      else{
	map_number++;
      }
    }
  }
  while (result!=NULL);

  closedir(dir);

  return map_number;
}
