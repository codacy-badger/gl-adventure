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

/** \file options.c
  * Implements functions that handle command-line options.
  * 
  *
  */

#include "options.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

char* program_name;
char* program_desc;
unsigned int help_indent=28;
unsigned int term_width;

/** Creates an empty option list
  *
  * \return The new list
  *
  */
option_list_t* option_list_create(void){
  // Get terminal width
  char* ge = getenv("COLUMNS");
  
  // Assume 80 columns if getenv failed
  term_width = (ge) ? atoi(ge) : 80;

  option_list_t* tmp = (option_list_t*)malloc(sizeof(option_list_t));
  tmp->size = 0;
  tmp->options = NULL;
  return tmp;
  tmp->options = NULL;
  return tmp;
}

/** Adds an option to the option list
  *
  * \param vList  The option list to add the option to
  * \param vBrief The brief option form
  * \param vLong  The lobg option form
  * \param vArg   Is an argument required
  * \param vFnptr The function pointer launched if the option is found
  * \param vDesc  The description text
  *
  */
void option_list_add(option_list_t* vList,char* vBrief, char* vLong, 
		     bool vArg, option_fncptr_t vFnptr, char* vDesc){

  assert(vList && "Cannot add option to NULL option list");
  vList->size++;
  unsigned int size_of = sizeof(option_t*) * vList->size;
  vList->options = (option_t**)realloc(vList->options, size_of);

  vList->options[vList->size - 1] = (option_t*)malloc(sizeof(option_t));
  vList->options[vList->size - 1]->brief       = vBrief;
  vList->options[vList->size - 1]->longf       = vLong;
  vList->options[vList->size - 1]->arg         = vArg;
  vList->options[vList->size - 1]->fptr        = vFnptr;
  vList->options[vList->size - 1]->description = vDesc;
}

/** Shows the help message
  *
  * \sa \ref program_name, \ref program_desc.
  *
  * \param vList The list of options
  *
  */
void option_list_show_help(option_list_t* vList){
  printf("\n\033[1m");
  printf("%s",  program_name);
  printf("\033[0m");
  printf(" - %s\n\n", program_desc);

  unsigned int i;
  for (i=0; i< vList->size; i++){
    option_list_show_opt(vList->options[i]);
  }
}

/** Print an option
  *
  * It prints the brief and long format of the option then
  * use \ref option_list_print_indented() to print its description.
  *
  * \param vOpt The option to be printed
  *
  */
void option_list_show_opt(option_t* vOpt){
  char str[256];
  char str2[256];

  memset(str, '\0', sizeof(str));
  memset(str2, '\0', sizeof(str2));

  if (vOpt->arg){
    sprintf(str, "%s, %s [arg]", vOpt->brief, vOpt->longf);
  }
  else{
    sprintf(str, "%s, %s", vOpt->brief, vOpt->longf);
  }
  strncpy (str2, str, help_indent);

  // Add null-char if needed
  if (strstr(str2, "\0") == NULL){
    str2[help_indent] = '\0';
   printf("%s", str2);
  }
  else{
   printf("%s", str2);
   unsigned int i;
   for (i=strlen(str2); i< help_indent; i++){
      printf(" ");
   }
  }

  option_list_print_indented(vOpt->description);
}

/** Print an option list with an indentation
  *
  * The indentation length is stored in the \ref help_indent variable.
  *
  * \param vText The text to be printed
  *
  */
void option_list_print_indented(char* vText){
  assert(vText && "Cannot print an indented NULL string");

  char* buffer;

  int x=help_indent;
  char* word = option_list_next_word(vText);
  while(word && vText){

    x += (strlen(word) + 1); // +1 is the space char
    if (x >= term_width){
      printf("\n\033[%dC", help_indent);
      printf("%s ", word);
      x = help_indent + strlen(word);
    }
    else{
      printf("%s ", word);
    }

    vText = option_list_remove_word(vText);
    word = option_list_next_word(vText);

  }
  printf("\n");

}

/** Get the next word in a C string
  *
  * \param vText The text we search for the next word in.
  *
  * \return The word we found or vText if not found.
  *
  */
char* option_list_next_word(char* vText){

  if (vText == NULL){
    return NULL;
  }

  char* delim_pos = strchr(vText, ' ');

  if (delim_pos == NULL){
    return vText;
  }
  else{
    // Get the forst word
    unsigned int index = delim_pos - vText;
    char* ret = (char*)malloc(sizeof(char) * (index + 1));
    strncpy(ret, vText, index);
    ret[index] = '\0';

    return ret;

  }
}

/** Remove the first word in a C string
  *
  * \param vText The string to be modified
  *
  * \return The modified string
  *
  */
char* option_list_remove_word(char* vText){
  unsigned int index = 0;
  char* delim_pos = NULL;

  if (vText == NULL){
    return NULL;
  }

  delim_pos = strchr(vText, ' ');
 
  if (delim_pos == NULL){
    return NULL;
  }
  else{
    index = delim_pos - vText;
 
    // Removes first word from vText
    unsigned int size = (strlen(vText) - index);
    char* new_vtext = (char*)malloc(sizeof(char) * size);
    strncpy(new_vtext, vText + index + 1, size - 1);
    return new_vtext;
  }
}

/** Changes some program-related informations
  *
  * \param vName The new program name
  * \param vDesc The new description
  *
  * \sa \ref program_name, \ref program_desc
  *
  */
void option_list_set_proginfos(char* vName, char* vDesc){
  program_name = vName;
  program_desc = vDesc;
}

/** Handles the command-line arguments
  *
  * \param vOpts The options list
  * \param argc Number of command-line arguments
  * \param argv The command-line arguments vector
  *
  */
void option_list_handle_args(option_list_t* vOpts, int argc, char** argv){
  int i; // argv index
  int o; // opts index
  bool found; // Option correctly handled ?
  bool ret; // The returned boolean of the function pointer

  // Starting from 1 to skip program name
  for (i=1; i< argc; i++){
    found = false;
    for (o=0; o < vOpts->size; o++){
      if (strcmp(argv[i], vOpts->options[o]->brief)==0 ||
	  strcmp(argv[i], vOpts->options[o]->longf)==0){
	found=true;
	assert(vOpts->options[o]->fptr!=NULL && "NULL function pointer for command-line argument");
	// Calling function pointer (with null argument if end of argv)
	if (i+1 < argc) ret = (*vOpts->options[o]->fptr)(argv[++i]);
	else          ret = (*vOpts->options[o]->fptr)(NULL);
	
	if (ret==true){
	  exit(0); // Quit program if func ptr return true
	}
      }
    }
    if (!found){
      printf("Unknown option '%s'\n", argv[i]);
      exit(1);
    }
  }
}
