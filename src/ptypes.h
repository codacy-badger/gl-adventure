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

/** \file ptypes.h
  * Header file for portable common types.
  *
  */

#ifndef _PTYPES_H_
#define _PTYPES_H_

#ifndef WIN32
#  include <features.h>
#endif

/** Size should be 20bits on 32bits system
  * and 32 bits on 64 bits systems
  *
  * http://www.ibm.com/developerworks/library/l-port64.html
  *
  */
typedef struct {
	int i1;   //!< Only for tests
	double d; //!< Only for tests
	int i2;   //!< Only for tests
	long l;   //!< Only for tests
}test;

#if (__WORDSIZE == 64)                         // for 64 bits
  typedef char  int8; 
  typedef short int16; 
  typedef int   int32; 
  typedef long  int64; 

  typedef unsigned char  uint8; 
  typedef unsigned short uint16; 
  typedef unsigned int   uint32; 
  typedef unsigned long  uint64; 

#  elif defined(__WIN32) || (__WORDSIZE == 32) // for 32 bits
  typedef unsigned char  int8; 
  typedef unsigned short int16; 
  typedef unsigned int   int32;
  typedef unsigned long long  int64; 

  typedef unsigned char  uint8; 
  typedef unsigned short uint16; 
  typedef unsigned int   uint32;
  typedef unsigned long long  uint64; 
#endif // !(__WORDSIZE == 64) 

#endif // !_PTYPES_H_
