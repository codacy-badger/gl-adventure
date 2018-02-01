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

/** \file loadmap.h
  * Declaration of the functions that loads a map
  *
  *
  */

#include "def.h"
#include "map.h"

/** The map loading state
  *
  */
typedef enum{
  ML_MAGN = 0, // We are getting the magic number
  ML_NAME,     // We are getting the map's name
  ML_SIZE,     // We are getting the map's size
  ML_SPOS,     // We are getting the player's starting position
  ML_EPOS,     // We are getting the End of Level position
  ML_DATA,     // We are getting the map data
  ML_OBJS      // We are getting object placement
}map_loading_t;

/** The directory where the maps are loaded */
#define MAP_DIR "../maps/"
/** The max lenght of a line in the map file */
#define MAP_MAX_LINE 200

/** The magic number describing a map*/
#define MAGIC_NUMBER "3B"

/** The current line of data we are reading when loading map data */
char current_data_line;

map_t* get_random_map(void);
int read_map_directory(bool, int, map_t*);
void load_map(const char*, map_t*);

void handle_map_size(char*, map_t*);
void handle_start_pos(char*, map_t*);
void handle_end_pos(char*, map_t*);
bool handle_map_data(char*, map_t*);
void handle_objects_pos(char*, map_t*);
