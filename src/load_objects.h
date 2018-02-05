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

/** \file load_objects.h
  * Declares functions that load object list from file
  *
  *
  */
#ifndef _LOAD_OBJECTS_H_
#define _LOAD_OBJECTS_H_

#include "object_list.h"

/** The buffer size used to load object file
  *
  * Set a high value because of the comments
  *
  */
#define OBJ_MAX_LINE 200

/** The load state used by the file parser
  *
  * Each line in the file is used in one of these states.
  *
  */
typedef enum{
  LO_ID = 1, // We are parsing the alphanumeric identifier
  LO_EN,     // We are getting the english name
  LO_FR,     // We are getting the french name
  LO_END,    // We are getting the english description
  LO_FRD,    // We are getting the french description
  LO_WGT,    // We are getting the weight
  LO_FPC,    // We are getting the find percent
  LO_CARACS, // Wa are parsing the object caracteristics

  LO_LAST  //!< Special value should go to LO_ID;
}load_objects_t;

object_list_t* load_objects(void);
void unload_objects(object_list_t*);

void handle_id(object_t*, char*);
void handle_en(object_t*, char*);
void handle_fr(object_t*, char*);

void handle_en_desc(object_t*, char*);
void handle_fr_desc(object_t*, char*);

void handle_weight(object_t*, char*);
void handle_find_percent(object_t*, char*);

void handle_caracs(object_t*, char*);

#endif // _LOAD_OBJECTS_H_
