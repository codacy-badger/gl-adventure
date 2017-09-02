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

/** \file logger.h
  * Declaration of logging related functions
  *
  */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "def.h"


/** A macro used to fix a \c __LINE__ bug
  *
  */
#define STRINGIFY(x) #x

/** A macro used to fix a \c __LINE__ bug
  *
  */
#define TOSTRING(x) STRINGIFY(x)

#ifdef XA_DEBUG
  /** \def LOG
    * Logs a simple text
    *
    * \param text The text to be drawn.
    *
    */
#  define LOG(text) xa_log(concat(__FILE__,TOSTRING(__LINE__)), text);
  /** \def LOGI
    * Log a text containing an integer
    *
    * The text must contain the %d printf escape sequence.
    *
    * \param text The text to be drawn.
    * \param i The integer to be logged
    *
    */
#  define LOGI(text, i) xa_log_int(concat(__FILE__, TOSTRING(__LINE__)),text,i);
  /** \def LOGS
    * Log a text containing a string
    *
    * The text must contain the %s printf escape sequence.
    *
    * \param text The text to be drawn.
    * \param s The string
    *
    */
#  define LOGS(text, s) xa_log_str(concat(__FILE__, TOSTRING(__LINE__)),text,s);
  /** \def LOGC
    * Log a text containing a character
    *
    * The text must contain the %c printf escape sequence.
    *
    * \param text The text to be drawn.
    * \param c The chracter
    *
    */
#  define LOGC(text, c) xa_log_chr(concat(__FILE__, TOSTRING(__LINE__)),text,c);
#else
#  define LOG(text)
#  define LOGI(text, i) 
#  define LOGS(text, s)
#  define LOGC(text, c)
#endif // XA_DEBUG


#ifdef XA_DEBUG
  char* concat(const char*,const char*);
  void free_concat(void);

  void print_header(char*);
  void print_footer(void);

  void xa_log(char*, const char*);
  void xa_log_int(char*, const char*, int);
  void xa_log_str(char*, const char*, const char*);
  void xa_log_chr(char*, const char*, char);
#endif // XA_DEBUG

#endif // _LOGGER_H_
