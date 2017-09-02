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

/** \file keybinding.h
  * Defines XLib keysim/function pointer binding
  *
  *
  */

#ifndef _KEY_BINDING_H_
#define _KEY_BINDING_H_

#include <X11/X.h> // For KeysYm typedef

#include "def.h"

// to be used with fncptr_t returned value
/** Should we quit application
  *
  * This macro intend to be used with the return value of
  * \ref fncptr_t.
  *
  * \note Negative values always return \c false. It is mandatory
  *       as a not found binding returns -1 (see \ref handle_kb_event()).
  *
  */
#define MUST_QUIT(x) ((x < 0)? false : x & 0x0001)

/** Should we redraw application
  *
  * This macro intend to be used with the return value of
  * \ref fncptr_t.
  *
  * \note Negative values always return \c false. It is mandatory
  *       as a not found binding returns -1 (see \ref handle_kb_event()).
  *
  */
#define MUST_DRAW(x) ((x < 0)? false : x & 0x0002)

/** Should we change current game state
  *
  * From the bit 5, we have the new index.
  *
  */
#define MUST_CHST(x) ((x < 0)? false : x & 0x0004)

/** Get the new state index
  *
  * When changing current game state(3rd bit of the returned value)
  * you must retrieve the bit shifted index of the new state. Use this
  * function to retrieve the new game state index.
  *
  * \param x The value returned by the key binding function pointer.
  *
  * \sa \ref fncptr_t, \ref GS_CHST_BIT_SHIFT
  *
  */
#define GET_STATE(x) (x>>GS_CHST_BIT_SHIFT)

/** A function pointer with int return value and no parameters 
  *
  * The returned value is a bit-mask corresponding to :
  * - bit 1 : quit game
  * - bit 2 : redraw graphics
  * - bit 3 : change state (bit 5-end state index)
  *
  * This bitmask is also used in the handle_xevent() function.
  *
  */
typedef int (*fncptr_t)(void);

/** Defines the binding of a key with a function pointer
  *
  */
typedef struct{
  int      x_keysym; //!< The Xlib keysym we associated
  fncptr_t fnc_ptr;  //!< The function pointer associated with the keysym
}key_binding_t;

/** Defines a list of key binding
  *
  */
typedef struct{
  unsigned int nb_binding; //!< number of binding
  key_binding_t** bindings; //!< The array of bindings pointer
}key_binding_list_t;


// Functions
key_binding_list_t* init_key_binding_list(void);
void                destroy_key_binding_list(key_binding_list_t*);

key_binding_list_t* add_binding(key_binding_list_t*, int, fncptr_t);

key_binding_t* get_addr_by_index(key_binding_list_t*, unsigned int);
int handle_kb_event(key_binding_list_t*, KeySym);

#ifdef XA_DEBUG
void test_fncptr_returned_value(void);
void test_one_fncptr_returned_value(int);
void debug_key_binding_list(key_binding_list_t*);
#endif // XA_DEBUG

#endif // _KEY_BINDING_H_
