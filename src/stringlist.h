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

/** \file stringlist.h
  *
  * Definition of a array of C strings
  *
  */

#ifndef _STRING_LIST_H_
#define _STRING_LIST_H_

#include <string.h> // for size_t

/** \def DEBUG_STRING_LIST
  *
  * This macro is used to log the content of a list but the function
  * used to debug the list, called string_list_debug(), is only built
  * in debug mode. 
  *
  * When using this macro, you assumed that in production mode, ther will
  * not undefined reference to string_list_debug().
  *
  * \param sl The pointer to string_list_t
  *
  */
#ifdef XA_DEBUG
#  define DEBUG_STRING_LIST(sl) string_list_debug(sl)
#else // XA_DEBUG
#  define DEBUG_STRING_LIST(sl) //
#endif // XA_DEBUG

/** The string list type
  *
  */
typedef struct{
  char** data;  //!< The data
  size_t size;  //!< The number of strings
}string_list_t;

string_list_t* create_string_list(void);
void           free_string_list(string_list_t*);

void string_list_add(string_list_t*, const char*);

#ifdef XA_DEBUG
  void string_list_debug(string_list_t*);
#endif // XA_DEBUG

#endif // _STRING_LIST_H_
