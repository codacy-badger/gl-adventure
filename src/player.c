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

/** \file player.c
  * Implementation of the player
  *
  */

#include "player.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "logger.h"
#include "object_list.h"
#include "gsgame.h"

/** Create, allocate and return a player
  *
  * \return A new player
  *
  */
player_t* init_player(void){
  size_t size = sizeof(player_t);
  LOGI("Allocating %d bytes for the player.", size);
  
  player_t* tmp = (player_t*)malloc(size);

  tmp->name=getenv("USERNAME");
  tmp->gold = rand()%15;
  tmp->level = 1;
  tmp->moves = 0;
  tmp->points = 0;
  tmp->time = 0;
  tmp->why_death = NULL;

  init_attribute(&tmp->life);
  init_attribute(&tmp->mana);
  
  init_food(&tmp->food, 300);
  init_food(&tmp->water, 200);

  init_level(&tmp->fighter);
  init_level(&tmp->wizard);

  // individual
  tmp->strength = ((rand()%15)+35) - ((tmp->fighter.next_level_xp-100)*2);
  tmp->intelligence = ((rand()%15)+35) - ((tmp->wizard.next_level_xp-100)*2);
  tmp->max_charge = (double)tmp->strength / 2;

  // Inventory
  tmp->inventory=create_inventory_list();

  // Food
  int food_lose_init = ((rand()%100)+100); // between 1s and 2s
  // If next_level_up is less than 100 the player looses its
  // food faster
  food_lose_init -= ((100 - tmp->fighter.next_level_xp)*FOOD_LEVEL_FACTOR);
  tmp->foodDownTime = (float)food_lose_init / 100;

  // Water
  int water_lose_init = ((rand()%100)+100); // between 1s and 2s
  water_lose_init -= ((100 - tmp->wizard.next_level_xp)*FOOD_LEVEL_FACTOR);
  tmp->waterDownTime = (float)water_lose_init / 100;

  return tmp;
}

/** Free a allocated player
  *
  * \param pl The player to free
  *
  */
void free_player(player_t* pl){
  free_inventory_list(pl->inventory);
  pl->inventory = NULL;

  if (pl->why_death) free(pl->why_death);
  free(pl);
}

/** Initialize an attribute to a random value
  *
  * \param a The atrribute to initialize
  *
  */
void init_attribute(player_attribute_t* a){
  unsigned int i = 15 + (rand()%15);
  a->value = i;
  a->max = i;
}

/** Initialize food or water value
  *
  * \param a    The attribute to initialize
  * \param base The base attribute, used to get a random value.
  *
  */
void init_food(player_attribute_t* a, unsigned int base){
  unsigned int i = base;
  int j = (rand()%40) - 20;
  i += j;
  a->value = i;
  a->max = i;
}


/** Initialize a level to a random value
  *
  * \param a The level to initialize
  *
  */
void init_level(player_level_t* a){
  // I apply a positive or negative random value to
  // add some chance.
  //
  // When switching level up, new next_level_xp will be
  // current : next_level_xp * 2. It can make sense in some levels.
  //
  int def = (rand()%30)-15;

  a->level=1;
  a->next_level_xp = 100 - def;
  a->current_level_xp = 0;
}

/** Player lost life points
  *
  * \param pl     The player
  * \param val    The number of points he lost
  * \param reason The reason the player lost life. It can be use as death 
  *               reason if player die.
  *
  */
void player_decrease_life(player_t* pl, unsigned int val, const char* reason){
  pl->life.value -= val;

  if (pl->why_death){
    free(pl->why_death);
  }

  pl->why_death = (char*)malloc(sizeof(char) * (strlen(reason)+1));
  strcpy(pl->why_death, reason);

  printf("Death reason is '%s'.\n", reason);
  assert(pl->life.value>=0 && "You are dead");
}

/** Players moves
  *
  * \param pl  The player
  *
  */
void player_move(player_t* pl){
  pl->moves++;
}

/** Decrease the player's amount of food
  *
  * \param pl  The player
  * \param val The value we remove
  *
  */
bool player_decrease_food(player_t* pl, int val){
  pl->food.value -= val;

  if (pl->food.value < 0){
    pl->food.value = 0;
    player_decrease_life(pl, 2, "Need some food");
    return true;
  }
  return false;
}

/** Decrease the player's amount of water
  *
  * \param pl  The player
  * \param val The value we remove
  *
  */
bool player_decrease_water(player_t* pl, int val){
  pl->water.value -= val;

  if (pl->water.value < 0){
    pl->water.value = 0;
    player_decrease_life(pl, 3, "Need some water");
    return true;
  }
  return false;
}

/** Increase the player's in-game time by one second
  *
  * \param pl The player
  *
  */
void player_increase_time(player_t* pl){
  pl->time++;
}

/** Remove the reason the player lost life
  *
  * Please always call this function if player has no reason to death. It
  * will remove the entry only if it is the same reason the the given one in
  * parameter.
  *
  * \sa \ref player_decrease_life().
  *
  *
  * \param pl     The player
  * \param reason The reason to remove
  *
  */
void player_remove_why_death(player_t* pl, const char* reason){
  if (pl->why_death == NULL){
    return;
  }
  else if (strcmp(pl->why_death, reason)==0){
    free(pl->why_death);
    pl->why_death=NULL;
  }
  
}

/** The player uses the current selected item
  *
  * The given player is about to use the item with index
  * \ref current_selected_item in its inventory. After some basic controls,
  * the object is removed from the player's inventory using the 
  * \ref inventory_list_remove() function and its effect is applied
  * to the given character.
  *
  * \param pl The player
  *
  */
void player_use_item(player_t* pl){
  char* obj_id = pl->inventory->items[current_selected_item]->name;
  object_t* obj = get_object_by_id(object_list, obj_id);
  inventory_list_remove(pl->inventory, obj_id);

  assert(obj && "Used object is NULL");

  switch(obj->type){
  case OT_FOOD:
    player_increase_food(pl, obj->val_int);
    break;
  
  case OT_WATER:
    player_increase_water(pl, obj->val_int);
    break;

  default:
    assert( 0 && "I can't handle this type of object");
  }

  player_compute_charge(pl);
}

/** Increases the amount of food of the given player
  *
  * \param pl  The player
  * \param val The amount of food added
  *
  */
void player_increase_food(player_t* pl, int val){
  pl->food.value += val;

  if (pl->food.value > pl->food.max){
    pl->food.value = pl->food.max;
  }
}

/** Increases the amount of water of the given player
  *
  * \param pl  The player
  * \param val The amount of water added
  *
  */
void player_increase_water(player_t* pl, int val){
  pl->water.value += val;

  if (pl->water.value > pl->water.max){
    pl->water.value = pl->water.max;
  }
}

/** Compute the current charge of a player
  *
  * The computed result is not returned. It is stored in the
  * player's structure. Please see the player's current_charge
  * field (\ref player_t).
  *
  * \param pl The player to compute the charge to
  *
  */
void player_compute_charge(player_t* pl){
  pl->current_charge=0;

  inventory_t* iv= pl->inventory;
  unsigned int i;
  for (i=0; i<iv->size; i++){
    char* obj_id = iv->items[i]->name;
    object_t* obj = get_object_by_id(object_list, obj_id);

    double tmp_charge = iv->items[i]->number * obj->weight;
    pl->current_charge += tmp_charge / 1000;
  }
}


#ifdef XA_DEBUG
/** Debug a pleyer in the console
  *
  * \param pl  The player
  *
  */
void debug_player(player_t* pl){
  printf("Debugging player:\n");
  printf("  name : %s\n", pl->name);
  printf("  lvl  : %d\n", pl->level);

  printf("  gold   : %d\n", pl->gold);
  printf("  points : %d\n", pl->points);
  printf("  moves  : %d\n", pl->moves);

  printf("  food : %d/%d\n", pl->food.value, pl->food.max);
  printf("  water : %d/%d\n", pl->water.value, pl->water.max);

  printf("  life : %d/%d\n", pl->life.value, pl->life.max);
  printf("  mana : %d/%d\n", pl->mana.value, pl->mana.max);
  printf("  fighter lvl : %d (%d/%d)\n", pl->fighter.level, 
	 pl->fighter.current_level_xp, pl->fighter.next_level_xp);
  printf("  wizard lvl : %d (%d/%d)\n", pl->wizard.level, 
	 pl->wizard.current_level_xp, pl->wizard.next_level_xp);

  printf("  strength     : %d\n", pl->strength);
  printf("  intelligence : %d\n", pl->intelligence);

  printf("  charge       : %4.2f kg\n", pl->max_charge);

  printf("  why_death  : %s\n\n", pl->why_death);

}
#endif // XA_DEBUG
