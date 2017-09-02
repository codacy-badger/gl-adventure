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

/** \file time_utils.c
  * Implements function to deal with system time
  *
  */

#include "time_utils.h"

#include "gsgame.h"
#include "player.h"

#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h> 

float lastUpdate           = 0;
float fpsUpdateInterval    = 1.0f;
unsigned int  numFrames    = 0;
long fps                   = 0;
float lastInGameTimeUpdate = 0;
float lastFoodDownTime     = 0;
float lastWaterDownTime    = 0;

/** Init the game time
  *
  * This must be called only one tile when the game is started.
  *
  */
void init_game_time(){
  ticksPerSecond = CLOCKS_PER_SEC;

  // First set it to 0 to get the time since computer start up
  timeAtGameStart = 0;
  timeAtGameStart = get_game_time();
}

/** Get the processed time since the game start
  *
  * \return The elapsed time in seconds
  *
  */
float get_game_time(){
  clock_t process_time;
  
  float time; // Time in seconds

  process_time = clock();


  time = (float)(long)process_time/(float)(long)ticksPerSecond;
  return time;
}

/** Update the fps value
  *
  * This must be called when a frame is drawn.
  *
  */
void update_fps(){
  numFrames++;

  float currentUpdate = get_game_time();
  if( currentUpdate - lastUpdate > fpsUpdateInterval ){
    float f_fps = numFrames / (currentUpdate - lastUpdate);
    lastUpdate = currentUpdate;
    numFrames = 0;

    fps = (int)floorf(f_fps);

    printf("===> fps = %d\n", fps);
  }

  if (currentUpdate - lastInGameTimeUpdate > 1.0f){
    player_increase_time(player);
    lastInGameTimeUpdate = currentUpdate;
  }

  if (currentUpdate - lastFoodDownTime > player->foodDownTime){
    player_decrease_food(player, 1);
    lastFoodDownTime = currentUpdate;

    
  }

  if (currentUpdate - lastWaterDownTime > player->waterDownTime){
    player_decrease_water(player, 1);
    lastWaterDownTime = currentUpdate;
  }
}
