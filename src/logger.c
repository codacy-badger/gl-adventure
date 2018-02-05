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

/** \file logger.c
  * Implementation of logging related functions
  *
  */
#include "logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef XA_DEBUG
  /** The char array used in to concatenate strings
    *
    * This variable is used in the \ref concat() function.
    *
    * \note This variable exists only in debug mode
    *
    */
  char* concat_temp_string = NULL;

  /** Concatenate file name and line
    *
    * \param vFile The file name
    * \param vLine The line number
    *
    * \return The concatenation of the strings
    *
    * \note This function is built only in debug mode
    *
    */
  char* concat(const char* vFile,const char* vLine){
    size_t temp_len = strlen(vFile) + strlen(vLine) + 2;

    if (concat_temp_string == NULL){
      concat_temp_string = malloc(temp_len * sizeof(char));
    }
    else{
      concat_temp_string = realloc(concat_temp_string, temp_len * sizeof(char));
    }

    memcpy(concat_temp_string, vFile, strlen(vFile)+1);
    strcat(concat_temp_string, ":");
    strcat(concat_temp_string, vLine);
    return ;
  }

  /** Free the memory allocated for concatenation
    *
    * Free the member \ref concat_temp_string id it was used.
    *
    * \note This function is built only in debug mode
    *
    */
  void free_concat(void){
    if (concat_temp_string != NULL){
      free(concat_temp_string);
      concat_temp_string = NULL;
    }
  }

  /** Print a line header
    *
    * \param vConc The value returned by the \ref concat() function
    *
    */
  void print_header(char* vConc){
    printf("%-"LOG_CONCAT_SIZE"s ", vConc);
  }

  /** Prints a message footer 
    *
    * Internal function that should always be called after a printf command.
     *
    * \note This function is built only in debug mode
   *
    */
  void print_footer(void){
    printf("\n");
  }


  /** Log a single message
    *
    * \param vConc The value of the \ref concat() function
    * \param vText The message to log
    *
    * \note This function is built only in debug mode
    *
    */
  void xa_log(char* vConc, const char* vText){
    print_header(vConc);
    printf(vText);
    print_footer();
    free_concat();
  }

  /** Log a message containing an integer
    *
    * \param vConc The value of the \ref concat() function
    * \param vText The message to log
    * \param vInt  The integer to be logged
    *
    * \note This function is built only in debug mode
    *
    */
  void xa_log_int(char* vConc, const char* vText,  int vInt){
    print_header(vConc);
    printf(vText, vInt);
    print_footer();
    free_concat();
  }

  /** Log a message containing a C-string
    *
    * \param vConc The value of the \ref concat() function
    * \param vText The message to log
    * \param vStr  The string to be logged
    *
    * \note This function is built only in debug mode
    *
    */
  void xa_log_str(char* vConc, const char* vText, const char* vStr){
    print_header(vConc);
    printf(vText, vStr);
    print_footer();
    free_concat();
  }

  /** Log a message containing a single character
    *
    * \param vConc The value of the \ref concat() function
    * \param vText The message to log
    * \param vChar The single character to be logged
    *
    * \note This function is built only in debug mode
    *
    */
  void xa_log_chr(char* vConc, const char* vText, char vChar){
    print_header(vConc);
    printf(vText, vChar);
    print_footer();
    free_concat();
  }


#endif // XA_DEBUG
