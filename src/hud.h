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

/** \file hud.h
  *
  * Declares some functions for head up display
  *
  * Modifications :
  * - 24 nov 2008 : Now draws player life and mana
  *
  */

#ifndef _HUD_H_
#define _HUD_H_

#include "graphics.h"
#include "player.h"
#include "map.h"

/** The border of all HUD */
#define HUD_BORDER 5
/** The border of the mini HUD */
#define HUD_WIDTH  100
/** The width of a mini HUD bar */
#define HUD_BAR_WIDTH  (HUD_WIDTH - (HUD_BORDER*2))
/** Indent value for right aligned values 
  *
  * Must be added to the x position of a text to align it at the left
  *
  */
#define HUD_INDENT 150

/** The item index selected in the inventory */
extern unsigned int current_selected_item;

void draw_player_hud( player_t*);
void draw_character_dialog( player_t*);
void draw_inventory_dialog(player_t*);

void draw_life_bar( player_t* );
void draw_mana_bar( player_t* );
void draw_character_time( player_t*);

void draw_background_bar(graphics_t*, int, int, int, int, int);

void previous_selected_item(player_t*);
void next_selected_item(player_t*);

#endif// _HUD_H_
