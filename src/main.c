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

/** \file main.c
  * The main entry file 
  *
  * Modifications :
  * - Switchiçng to OpenGL
  * - 28 jul 2008 : Using POSIX timer
  * - 19 jul 2008 : First version
  *
  */


#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>

#include "gl_window.h"
#include "opengl.h"

#include "colors.h"
#include "map.h"
#include "logger.h"
#include "fonts.h"

#include "gamestate.h"
#include "gslist.h"
#include "player.h"
#include "time_utils.h"
#include "options.h"

#include "config.h"

/** Defines a simpler way to use gettext
  *
  * \param String The text to be translated
  *
  */
#define _(String) gettext(String)


#include <X11/keysym.h>



///////////////////////////////////////////////////////////
// Global variables

/** The number of our GLUT window */
int window; 


/** The current map */
map_t* map = NULL;

/** The game state list */
game_state_t** gslist;

//bool quit = false; 

/** Set to \c true to redraw the game */
bool must_redraw = false;

/** The current gamestate */
int current_gs = 0;

/** Set to \c true to quit the game */
Bool done;

/** The command-line options list */
option_list_t* opts;

// Forward declarations
void keyPressed(KeySym key);
void drawGLScene(void);
bool show_help(char*);
bool test_monster(char*);
// End of forward declarations

///////////////////////////////////////////////////////////
// Functions

/** The main entry funtion
  *
  * \param argc The number of command line arguments 
  * \param argv The command line arguments array
  *
  * \return 0 on success, 1 if execution failled
  *
  */
int main(int argc, char** argv){
  XEvent event;
  KeySym key;
 
  srand ( time(NULL) );

  // Testing options
  option_list_set_proginfos("glAdventure", "An adventure game written in C "
			    "language and using OpenGL for graphics "
			    "rendering.");
  opts = option_list_create();
  option_list_add(opts, "-f", "--fullscreen", false, NULL, 
		  "Set the game in fullscreen mode. By default, the game "
		  "is always launched in window mode.");
  option_list_add(opts, "-h", "--help", false, show_help, 
		  "Show available command-line options and exit.");
  option_list_add(opts, "-m", "--monster", true, test_monster, 
		  "Test the monster named [arg] in a random map.");

  option_list_handle_args(opts, argc, argv);  

  init_game_time();

  must_redraw = false;

  setlocale (LC_ALL, "fr_FR");

  bindtextdomain( "xAdventure", "/usr/share/locale" );
  textdomain( "xAdventure" );

  printf(_("Starting %s v%s\n"), PACKAGE, VERSTRING);

  // Create game state list
  gslist = init_gs_list();

  done = False;
  /* default to windowed */
  GLWin.fs = False;
  char title[40];
  sprintf(title, "%s v%s", PACKAGE, VERSTRING);
  createGLWindow(title, 1024, 768, 24, GLWin.fs);

    /* wait for events*/ 
    while (!done)
    {
        /* handle the events in the queue */
        while (XPending(GLWin.dpy) > 0)
        {
            XNextEvent(GLWin.dpy, &event);
            switch (event.type)
            {
	    case Expose:
	      if (event.xexpose.count != 0)
		break;
	      drawGLScene();
	      break;
	    case ConfigureNotify:
                /* call resizeGLScene only if our window-size changed */
                    if ((event.xconfigure.width != GLWin.width) || 
                        (event.xconfigure.height != GLWin.height))
                    {
                        GLWin.width = event.xconfigure.width;
                        GLWin.height = event.xconfigure.height;
                        printf("Resize event\n");
                        resizeGLScene(event.xconfigure.width,
                            event.xconfigure.height);
                    }
                    break;
                /* exit in case of a mouse button press */
                case ButtonPress:     
                    done = True;
                    break;
                case KeyPress:
                    key = XLookupKeysym(&event.xkey, 0);
                    keyPressed(key);
                    break;
                case ClientMessage:    
                    if (*XGetAtomName(GLWin.dpy, event.xclient.message_type) == 
                        *"WM_PROTOCOLS")
                    {
                        printf("Exiting sanely...\n");
                        done = True;
                    }
                    break;
                default:
                    break;
            }
        }
        drawGLScene();
    }
    killGLWindow();

  // Freeing game state
  free_gs_list(gslist);

  return EXIT_SUCCESS;
}

/** Handle a keyboard event
  *
  * \param key The XLib key that has been pressed
  *
  */
void keyPressed(KeySym key){
  if (key== XK_F12){
    killGLWindow();
    GLWin.fs = !GLWin.fs;
    createGLWindow("glAdventure", 640, 480, 24, GLWin.fs);
  }
  else{
    int ret = handle_gs_event(gslist[current_gs], key);
    // Should we quit ??
    if (MUST_QUIT(ret)) done = True;
	
    // Change state if needed
    if (MUST_CHST(ret)){
      current_gs = GET_STATE(ret);
      LOGI("Changing to game state %d", current_gs);
      assert( current_gs <= GS_LIST_SIZE&& "Game state index out of bound");
    }
  }
}



/** The main drawing function. 
  *
  * It simply clear the screen, reset the view and call the current game state
  * drawing function.
  *
  */
void drawGLScene(){
  update_fps(); // Compute FPS
  advance_cycling_color();

  // Clear The Screen And The Depth Buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
  // Reset The View
  glLoadIdentity();

  assert (gslist[current_gs]->draw_fnc_ptr && "Invalid draw fun ptr");
  gslist[current_gs]->draw_fnc_ptr();
  
  glXSwapBuffers(GLWin.dpy, GLWin.win);
}

/** Show help message and quit game
  *
  * \param vOption The command-line option we found
  *
  * \return Always \c true to quit the game.
  *
  */
bool show_help(char* vOption){
  option_list_show_help(opts);
  return true; 
}

/** Change to the monster test gamestate
  *
  * \param monster_name The name of the monster to be tested
  *
  */
bool test_monster(char* monster_name){
  current_gs=GS_MONST;
  return false;
}
