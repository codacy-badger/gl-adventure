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

/** \file load_objects.c
  * Implements functions that load object list from file
  *
  *
  */

#include "load_objects.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logger.h"

/** Loads object and returns an object list containing them
  *
  * \return The list contaning loaded objets
  *
  */
object_list_t* load_objects(void){
  object_list_t* ol = create_object_list();

  FILE* f = fopen("../data/objects.dat", "r");
  assert(f && "Cannot open objects file");

  char content_nl[OBJ_MAX_LINE];
  char content[OBJ_MAX_LINE];

  load_objects_t status = LO_ID;

  object_t* obj = NULL;

  while(fgets(content_nl, OBJ_MAX_LINE, f)){
    memset(content, 0, OBJ_MAX_LINE);
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
      // Real datas, can handle it
      switch(status){
      case LO_ID:
	obj= create_object();;
	handle_id(obj, content);
	status++;
	break;

      case LO_EN:
	handle_en(obj, content);
	status++;
	break;

      case LO_FR:
	handle_fr(obj, content);
	status++;
	break;
      
      case LO_END:
	handle_en_desc(obj, content);
	if (content[strlen(content)-1] != '\\')
	  status++;
	break;
	  
      case LO_FRD:
	handle_fr_desc(obj, content);
	if (content[strlen(content)-1] != '\\')
	  status++;
	break;

      case LO_WGT:
	handle_weight(obj, content);
	status++;
	break;
	
      case LO_FPC:
	handle_find_percent(obj, content);
	status++;
	break;

      case LO_CARACS:
	handle_caracs(obj, content);
	status++;
	break;
      }

      if (status == LO_LAST){
	status = LO_ID;
	if (obj){
	  add_object(ol, obj);
	}
      }
    }
  }
  assert(status == LO_ID && "Please verify newline at end of data/objects.dat");

  fclose(f);

  DEBUG_OBJLST(ol);

  return ol;
}

/** Free the given object list
  *
  * \param ol The object list to be destroyed
  *
  */
void unload_objects(object_list_t* ol){
  free_object_list(ol);

}

/** Handles the identifier of the object
  *
  * \param obj The object to feed
  * \param id  The string that contains the identifier
  *
  */
void handle_id(object_t* obj, char* id){
  assert(obj && "Cannot feed NULL object.");
  obj->id = (char*)malloc(sizeof(char) * strlen(id) + 1);
  strcpy(obj->id, id);
}

/** Handles the english name of the object
  *
  * \param obj The object to feed
  * \param en  The string that contains the name
  *
  */
void handle_en(object_t* obj, char* en){
  assert(obj && "Cannot feed NULL object.");
  obj->en = (char*)malloc(sizeof(char) * strlen(en) + 1);
  strcpy(obj->en, en);

}


/** Handles the french name of the object
  *
  * \param obj The object to feed
  * \param fr  The string that contains the name
  *
  */
void handle_fr(object_t* obj, char* fr){
  assert(obj && "Cannot feed NULL object.");
  obj->fr = (char*)malloc(sizeof(char) * strlen(fr) + 1);
  strcpy(obj->fr, fr);
}

/** Handle an object description
  *
  * It is a special function that need to handle continuation char.
  *
  * \param obj The single line of description
  * \param en  The description string
  *
  */
void handle_en_desc(object_t* obj, char* en){
  if (obj->en_desc == NULL){
    // Treat it as every char* (alloc and cpy)
    obj->en_desc = (char*)malloc(sizeof(char) * strlen(en) + 1);
    strcpy(obj->en_desc, en);
  }
  else{
    // We copy current desc in temp_char (adding null char for future strlen)
    size_t temp_char_s = (strlen(obj->en_desc)-1) * sizeof(char);
    printf("Allocating %d bytes for temp_char\n", temp_char_s);
    char* temp_char = (char*)malloc(temp_char_s);
    strncpy( temp_char, obj->en_desc, strlen(obj->en_desc)-1);
    temp_char[strlen(obj->en_desc)-1]='\0';

    // realloc en_des
    size_t i = sizeof(char) * (strlen(temp_char) + strlen(en) + 1);
    obj->en_desc = (char*)realloc(obj->en_desc, i);

    // concat the new string
    memcpy(obj->en_desc, temp_char, strlen(temp_char));
    memcpy(obj->en_desc + strlen(temp_char), en, strlen(en));
    obj->en_desc[i-1] = '\0';

    // Freeing memory
    free(temp_char);
  }
}

/** Handle an object description
  *
  * It is a special function that need to handle continuation char.
  *
  * \param obj The single line of description
  * \param fr  The description string
  *
  */
void handle_fr_desc(object_t* obj, char* fr){
  if (obj->fr_desc == NULL){
    // Treat it as every char* (alloc and cpy)
    obj->fr_desc = (char*)malloc(sizeof(char) * strlen(fr) + 1);
    strcpy(obj->fr_desc, fr);
  }
  else{
    // We copy current desc in temp_char (adding null char for future strlen)
    char* temp_char = (char*)malloc(strlen(obj->fr_desc)-1);
    strncpy( temp_char, obj->fr_desc, strlen(obj->fr_desc)-1);
    temp_char[strlen(obj->fr_desc)-1]='\0';

    // realloc en_des
    size_t i = sizeof(char) * (strlen(temp_char) + strlen(fr) + 1);
    obj->fr_desc = (char*)realloc(obj->fr_desc, i);

    // concat the new string
    memcpy(obj->fr_desc, temp_char, strlen(temp_char));
    memcpy(obj->fr_desc + strlen(temp_char), fr, strlen(fr));
    obj->fr_desc[i-1] = '\0';

    // Freeing memory
    free(temp_char);
    temp_char = NULL;
  }
  
}

/** Handles the weight of an object
  *
  * \param obj The object structure
  * \param str The weight string
  *
  */
void handle_weight(object_t* obj, char* str){
  obj->weight = atoi(str);
}

/** Handles the find percent of an object
  *
  * \param obj The object structure
  * \param str The chance string
  *
  */
void handle_find_percent(object_t* obj, char* str){
  obj->find_percent = atof(str);
}

/** Handle type and caracteristics of the object
  *
  * \param obj The object
  * \param str The caracteristics string
  *
  */
void handle_caracs(object_t* obj, char* str){
  char* id = strtok(str, " ");
  assert(id && "Cannot get object id.");
  obj->type = atoi(id);

  // One integer value
  if (obj->type == OT_FOOD || obj->type == OT_WATER){
    char* val = strtok(NULL, " ");
    obj->val_int = atoi(val);
  }
}
