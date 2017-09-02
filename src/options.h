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

/** \file options.h
  * Defines functions that handle command-line options.
  * 
  *
  */

#ifndef OPTIONS_H
#define OPTIONS_H

#include "def.h" // For bool

/** Defines a function pointer called if an option is found
  *
  * The return value should return true to quit the game.
  * The char* argument hold the command-line option parameter if found.
  *
  */
typedef bool (*option_fncptr_t)(char*);

/** Defines an option to be parsed
  *
  * 
  *
  */
typedef struct{
  char*           brief; //!< Brief form starting with '-'
  char*           longf; //!< Brief form starting with '--'
  bool            arg;   //!< Is an argument required
  option_fncptr_t fptr;  //!< The function pointer
  char*           description; //!< The description for the help
}option_t;

/** The list of options
  *
  */
typedef struct{
  option_t** options; //!< The option objects array
  unsigned int size;  //!< The size of the list
}option_list_t;

/** The program name
  *
  * The value of this variable is changed using the 
  * \ref option_list_set_proginfos() function.
  *
  */
extern char* program_name;

/** The program description
  *
  * The value of this variable is changed using the 
  * \ref option_list_set_proginfos() function.
  *
  */
extern char* program_desc;

/** The indentation we use when we print thehelp message
  *
  */
extern unsigned int help_indent;

/** The width of the terminal in columns
  *
  * This value is initialized in the \ref option_list_create() function.
  *
  */
extern unsigned int term_width;

option_list_t* option_list_create(void);

void option_list_add(option_list_t*,char*, char*, bool,option_fncptr_t, char*);

void option_list_show_help(option_list_t*);
void option_list_show_opt(option_t*);
char* option_list_next_word(char*);
char* option_list_remove_word(char*);
void option_list_handle_args(option_list_t*, int, char**);

void option_list_print_indented(char*);

void option_list_set_proginfos(char*, char*);

#endif // !OPTIONS_H
