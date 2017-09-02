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

/** \file time_utils.h
  * Declares function to deal with system time
  *
  * The following page http://www.programmersheaven.com/2/time-fps was
  * really usefull.
  *
  * Modifications :
  * - 24 nov 2008 : Stating implementation
  *
  */


#ifndef TIME_UTILS_H
#  define TIME_UTILS_H

// Global variables for time
float timeAtGameStart; //!< Time when the game start
long  ticksPerSecond;  //!< The system ticks per second

// Global variables for measuring fps
extern float lastUpdate;           //!< The time when the last update occur
extern float fpsUpdateInterval;    //!< Updates every (?? seconds)
extern unsigned int numFrames;     //!< Frames drawn since last FPS update
extern long fps;                   //!< The frame per second number
extern float lastInGameTimeUpdate; //!< Ellapsed time since last in-game update
extern float lastFoodDownTime;     //!< Ellapsed time since last food down
extern float lastWaterDownTime;    //!< Ellapsed time since last water down

void init_game_time();
float get_game_time();

void update_fps();

#endif // !TIME_UTILS_H
