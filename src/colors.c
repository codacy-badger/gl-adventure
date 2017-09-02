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
 
/** \file colors.c
  *
  * Implementation of colors-related functions
  *
  */

#include "colors.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h> 

#include "logger.h"

/** Initialize colors from the given X11 display
  *
  * \param dis     The X11 display used to get colors
  * \param w       The Xlib window
  *
  * \return The color structure newly initialized
  *
  */
colors_t* init_colors( Display* dis, Window w){
  LOG("Starting initialization of colors");

  colors_t* vColors = (colors_t*)malloc(sizeof(colors_t));
  assert(vColors && "colors pointer not initialized");
  // Exact colors
  XColor eblack, ewhite, ered, eblue, elgrey, egrey, egreen;
  Status rc;

  // Allocating a color map
  LOG("  Create colormap");
  Visual* dv = DefaultVisual(dis, DefaultScreen(dis));
  DEBUG_VISUAL(dv);

  colormap = XCreateColormap(dis, w, dv, AllocNone);

  LOG("  Creating black color");
  rc= XAllocNamedColor(dis,colormap,"black",&vColors->black,&eblack);
  assert(rc && "Failed to load 'black' color.");  

  LOG("  Creating white color");
  rc= XAllocNamedColor(dis,colormap,"white",&vColors->white,&ewhite);
  perror("Creating white color");
  assert(rc && "Failed to load 'white' color.");  

  LOG("  Creating red color");
  rc= XAllocNamedColor(dis,colormap,"red",&vColors->red,&ered);
  assert(rc && "Failed to load 'red' color.");  

  LOG("  Creating blue color");
  rc= XAllocNamedColor(dis,colormap,"blue",&vColors->blue,&eblue);
  assert(rc && "Failed to load 'blue' color."); 
 
  LOG("  Creating green color");
  rc= XAllocNamedColor(dis,colormap,"green",&vColors->green,&egreen);
  assert(rc && "Failed to load 'green' color.");  

  allocate_RGB(&vColors->light_grey, dis, 120, 120, 120);
  allocate_RGB(&vColors->grey, dis, 70, 70, 70);
  allocate_RGB(&vColors->brown, dis, 139, 87, 46);

  LOG("Colors initialized");

  return vColors;
}

/** Free the memory aloocated for colors
  *
  * \todo Fix it
  *
  * \param cols The colors tp delete
  * \param dis  The XLib display
  *
  */
void free_colors(colors_t* cols, Display* dis){
  LOG("Freeing colors");

  int nb_colors = sizeof(colors_t)/sizeof(XColor);
  int i=0;

  // Create pixel list
  unsigned long* pixel_list;
  pixel_list = (unsigned long*)malloc(nb_colors * sizeof(unsigned long));

  // Initializing pixel list
  pixel_list[0] = cols->black.pixel;               i++;
  pixel_list[1] = cols->white.pixel;               i++;
  pixel_list[2] = cols->red.pixel;                 i++;
  pixel_list[3] = cols->blue.pixel;                i++;
  pixel_list[4] = cols->green.pixel;               i++;
  pixel_list[5] = cols->grey.pixel;                i++;
  pixel_list[6] = cols->light_grey.pixel;          i++;
  pixel_list[7] = cols->brown.pixel;               i++;
  
  assert( i == nb_colors && "Not freeing all colors!");

  // The following cause a SEGFAULT
  XFreeColors(dis, colormap, pixel_list, nb_colors, 0);
  free(pixel_list);
  free(cols);

}

/** Allocate a RGB color
  *
  * \param xc          The XColor object where the new will be stored
  * \param dis         The X Display object
  * \param red, green, blue The color component (0-255)
  *
  *
  *
  */
int allocate_RGB(XColor* xc, Display* dis, ushort red, ushort green, ushort blue){

  xc->red = TO_X(red);
  xc->green = TO_X(green);
  xc->blue = TO_X(blue);
  xc->flags = DoRed|DoGreen|DoBlue;
  
  printf("Trying to allocate color %d, %d, %d\n", xc->red,xc->green, xc->blue);

  Status rc= XAllocColor(dis,colormap, xc);
  assert(rc && "Failed to load RGB color."); 
  
}

//////////////////////////////////////////////////////
// Only if debug
#ifdef XA_DEBUG
  /** Debug the given XLib visual
    *
    * \param v The visual to debug
    *
    */
  void debug_visual(Visual* v){

    char* class_name;

    switch(v->class){
    case StaticGray:
      class_name = "StaticGray";
      break;
      
    case GrayScale:
      class_name = "GrayScale";
      break;

    case StaticColor:
      class_name = "StaticColor";
      break;

    case PseudoColor:
      class_name = "PseudoColor";
      break;

    case TrueColor:
      class_name = "TrueColor";
      break;

    case DirectColor:
      class_name = "DirectColor";
      break;

    default:
      class_name = "Unset";
    }

    LOGS("Visual type is %s.",class_name); 
  }
#endif // XA_DEBUG
