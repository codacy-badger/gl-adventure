# gl-adventure

[![Build Status](https://travis-ci.org/rainbru/gl-adventure.svg?branch=master)](https://travis-ci.org/rainbru/gl-adventure)
[![codecov](https://codecov.io/gh/rainbru/gl-adventure/branch/master/graph/badge.svg)](https://codecov.io/gh/rainbru/gl-adventure)

A ugly C dungeon exploration game :

![Menu screenshot](doc/gla-menu.png)
![Game screenshot](doc/gla-game.png)

# Dependencies

To install needed packages on Debian GNU/Linux :

	sudo apt-get install -y libgl1-mesa-dev freeglut3-dev \
	  libxmu-dev libxi-dev

# Building

This project uses `cmake` :

	mkdir build
	cd build/
	cmake ..
	make
	./gladventure

# Command-line options

	-f, --fullscreen  Set the game in fullscreen mode.  (not working yet)
	-h, --help                  Show available command-line options and exit. 
	-m, --monster [arg]         Test the monster named [arg] in a random map. 

# In-game controls

	Arrows        Move player
	I             Inventory dialog
	C             Character dialog
	Esc           Exit
	Enter         Collect item
	Page up/down  Zoom in/out
	D             Log debug  (only in Debug mode)
	L             Log player (only in Debug mode)
