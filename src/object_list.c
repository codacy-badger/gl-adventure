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

/** \file object_list.c
  * Implementation of the loaded object list
  *
  *
  */

#include "object_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "logger.h"

/** Creates and return an object list
  *
  * It creates a new object list and return it. The given object list must 
  * be freed with \ref free_object_list().
  *
  * \return The newly created object list
  *
  */
object_list_t* create_object_list(){
  object_list_t* tmp = (object_list_t*)malloc(sizeof(object_list_t));
  assert(tmp && "Cannot allocate object list");
  tmp->objects = NULL;
  tmp->size    = 0;
  tmp->find_total = 0;
  return tmp;
}

/** Free memory used by the given object list and its objects
  *
  * \param l The list to be free
  *
  */
void free_object_list(object_list_t* l){
  if (l!=NULL){
    size_t i;
    for (i=0; i< l->size; i++){
      if (l->objects[i] != NULL){
	free_object(l->objects[i]);
      }
    }

    if (l->objects != NULL){
      free(l->objects);
    }

    free(l);
  }
}

/** Add an object in an object list
  *
  * This function tests for duplicate object identifier and
  * uses an assert if a duplicate was found. It also test
  * if the given object is non-NULL.
  *
  * \param ol The object list
  * \param o  The object to add
  *
  */
void add_object(object_list_t* ol, object_t* o){
  bool duplicate = id_exists(ol, o->id);
  if (duplicate){
    printf("Error : duplicate object id : '%s'\n", o->id);
    assert(!duplicate && "Duplicate object id are forbidden");
  }

  assert(o && "Cannot add NULL object");

  ol->size++;
  ol->find_total += o->find_percent;
  size_t new_size = (ol->size) * sizeof(object_t*);
  ol->objects = realloc(ol->objects, new_size);
  assert(ol->objects && "Cannot reallocate object list");

  ol->objects[ol->size-1] = o;
  
}

/** Create an empty object
  *
  * \return An empty object
  *
  */
object_t* create_object(void){
  object_t* obj = (object_t*)malloc(sizeof(object_t));
  obj->type         = OT_UNSET;
  obj->id           = NULL;
  obj->en           = NULL;
  obj->fr           = NULL;
  obj->en_desc      = NULL;
  obj->fr_desc      = NULL;
  obj->weight       = 0;
  obj->find_percent = 0.0f;
  obj->val_int      = 0;
}

/** Free an object
  *
  * You should better use \ref free_object_list() as this function
  * destroy the list and all its objects.
  *
  * \param obj The object to free
  *
  */
void free_object(object_t* obj){
  if (obj != NULL){
    if (obj->id != NULL) free(obj->id);
    if (obj->en != NULL) free(obj->en);
    if (obj->fr != NULL) free(obj->fr);
    if (obj->en_desc != NULL) free(obj->en_desc);
    if (obj->fr_desc != NULL) free(obj->fr_desc);

    free(obj);
  }
}

/** Is the given id already exist
  *
  * \param ol The object list
  * \param id The id to search
  *
  * \return true if the given id already exists in the list
  *
  */
bool id_exists(object_list_t* ol,char* id){
  size_t i;

  for (i=0; i< ol->size; i++){
    if (strcmp(id, ol->objects[i]->id)==0){
      return true;
    }
  }
  return false;
}

/** Get a random object from the given list
  *
  * \param ol The list the object is given from
  *
  * \return A random object or NULL if function failed
  *
  */
object_t* get_random_object(object_list_t* ol){
  // Get a random floating point number between
  // zero and find_total (this requires the C math library)
  double r=(double)rand();
  double val = fmod(r, ol->find_total);

  // Get the object with this 
  double find_percent_total = 0;
  size_t i;
  for (i=0; i< ol->size; i++){
    find_percent_total += ol->objects[i]->find_percent;
    if (val < find_percent_total){
      return ol->objects[i];
    }
  }
  return NULL;
}

/** Get an object by its identifier
  *
  * \param ol The list to search for object
  * \param id The alphanumeric identifier of the object
  *
  * \return The object or NULL if not found
  *
  */
object_t* get_object_by_id(object_list_t* ol,char* id){
  unsigned int i;
  for (i=0; i< ol->size; i++){
    if (strcmp(ol->objects[i]->id, id)==0)
      return ol->objects[i];
  }
  return NULL;
}

/** Remove the last element of the list
  *
  * \bug Freeing ol cause a segfault
  *
  * \param ol The list we remove the last element from
  *
  */
object_list_t* object_list_pop_back(object_list_t* ol){
  assert(ol && "Object list is NULL");

  if (ol->size > 1){
    object_t* o = ol->objects[ol->size - 1];
    assert(o && "Last object is NULL");
    
    ol->size--;
    ol->find_total -= o->find_percent;
    size_t new_size = (ol->size) * sizeof(object_t*);
    ol->objects = realloc(ol->objects, new_size);
    assert(ol->objects && "Cannot reallocate object list");

    return ol;
  }
  else{
    // Cause a segfault
    //    free_object_list(ol);
    ol = NULL;
    return NULL;
  }
}

#ifdef XA_DEBUG
  /** Logs an object list in stdout
    *
    * \param ol The object list to log
    *
    * \note This function is only built in debug mode. You can use
    *       \ref DEBUG_OBJLST to avoid contidionnal compilation from
    *       the caller.
    *
    */
  void debug_object_list(object_list_t* ol){
    LOG("Debugging Object list");

    size_t size;
    size_t i;

    // Compute size of list in bytes
    size_t sc = sizeof(char);
    size = sizeof(object_list_t);
    for (i=0; i< ol->size; i++){
      size += sizeof(object_t);
      if (ol->objects[i]->id) size += (strlen(ol->objects[i]->id) * sc);
      if (ol->objects[i]->en)size += (strlen(ol->objects[i]->en) * sc);
      if (ol->objects[i]->fr)size += (strlen(ol->objects[i]->fr) * sc);
      if (ol->objects[i]->en_desc)size +=(strlen(ol->objects[i]->en_desc)* sc);
      if (ol->objects[i]->fr_desc)size += (strlen(ol->objects[i]->fr_desc)* sc);
      size += sizeof(unsigned int); // For the weight
      size += sizeof(float);        // For the find percent
    }

    printf(" ol size is %d (%d bytes)\n", ol->size, size);
    printf("   find percent total = %f", ol->find_total);
    for (i=0; i< ol->size; i++){
    printf(" object #%d\n", i);
      debug_object(ol->objects[i]);
    }
  }

  // Only if debug
  /** Logs an object in stdout
    *
    * \param obj The pbject to log
    *
    * \note This function is only built in debug mode.
    *
    */
  void debug_object(object_t* obj){
    printf("----id   = %s\n", obj->id);
    printf("    name = %s (%s)\n", obj->en, obj->fr);
    printf("    english desc = %s\n", obj->en_desc);
    printf("    french desc  = %s\n", obj->fr_desc);
    printf("    weight = %dg\n", obj->weight);
    printf("    find percent = %f\n", obj->find_percent);
    printf("    Type : %s", object_type_string(obj->type));

    if (obj->type == OT_FOOD || obj->type == OT_WATER){
      printf(" (+%d)\n", obj->val_int);
    }
   
  }

  /** Return human-readable string from an object type
    *
    * \param t The type
    *
    * \return The string
    *
    */
  const char* object_type_string(const object_type_t t){
    switch(t){
    case OT_FOOD:
      return "Food";
      break;
    case OT_WATER:
      return "Water";
      break;
    default:
      return "Unknown";
    }
  }
#endif // XA_DEBUG
