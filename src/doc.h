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

/** \file doc.h
  * Contains the main page of the documentation
  *
  *
  */


// Documentation main page

/** \mainpage MainPage
  *
  * \section introduction
  *
  * \c glAdventure is a project dedicated to the building of a very little
  * adventure game using OpenGl and GLX.
  *
  * \section Translation
  *
  * The translations are assured by \c gettext. To update them, please
  * try :
  * <pre>
  * xgettext -d glAdventure -s -o xAdventure-new.pot *.c def.h
  * msgmerge -s -U french.po glAdventure-new.pot
  * </pre>
  *
  * Then you can edit the \c french.po file, for example with \c qtranslator.
  * And you need to install the new translation file after having compiled it :
  * <pre>
  * msgfmt -c -v -o glAdventure.mo french.po
  * cp xAdventure.mo /usr/share/locale/fr_FR/LC_MESSAGES
  * </pre>
  * 
  * \section xlib_keysym Xlib keysym
  *
  * XLib uses keysym for keyboard events. You can find them in the 
  * \c /usr/include/X11/keysymdef.h header.
  *
  * \section time_function_sec Timing functions
  *
  * For computation of elapsed time and other time-related function, please
  * see \ref time_utils.h.
  *
  */

/** \page Concept
  *
  * \section Map drawing
  *
  * The following images shows how the map should be drawn. The player is
  * always in the center of the map.
  *
  * \image html example.png "There are two separate grids"
  *
  * The fact is that we draw walkside and not walls. Walls are white and we 
  * draw the hole to the further grid where the player can walk.
  *
  * \image html example2.png "Two walls was added"
  *
  * Once we apply colors, it can be this.
  *
  * \image html example3.png "The same map colored"
  *
  */

/** \page troubleshooting Troubleshooting
  *
  * During development, I have seen these errors. If one occurs, please test 
  * these solution.
  *
  * \section glibc_detected glibc detected corrupted double-linked list
  *
  * This is a very annoying error, due to internal glibc sanity check. To
  * temporally be able to run xAdventure, please test this :
  * <pre>export MALLOC_CHECK_=0&&./xAdventure</pre>
  *
  * The <code>export MALLOC_CHECK_=0</code> tells glibc to ignore sanity
  * checks that cause the bug.
  *
  */
