cmake_minimum_required (VERSION 2.6)

project (gladventure)

# XA_DEBUG adds a lot of console output
add_definitions(-DXA_BOOL) # -DXA_DEBUG)

include_directories(./src)


set(SRC		  src/colors.c  		
		  src/fonts.c		
		  src/gamestate.c		
		  src/gl_font.c		
		  src/gl_window.c		
		  src/gslist.c		
		  src/hud.c			
		  src/inventory.c		
		  src/keybinding.c		
		  src/load_bmp.c		
		  src/loadmap.c		
		  src/load_objects.c	
		  src/logger.c 		
		  src/main.c  		
		  src/map.c			
		  src/monster.c		
		  src/object_list.c		
		  src/opengl.c		
		  src/options.c		
		  src/player.c		
		  src/stringlist.c		
		  src/time_utils.c		
		  
		  src/gsgame.c		
		  src/gshelp.c		
		  src/gsintro.c		
		  src/gs_monster.c		
		  
		  src/th_basic.c)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -ggdb3 -O0 -DXA_DEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(gladventure ${SRC})

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} modules)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(X11 REQUIRED)
#find_package(X11XVM REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} 
		     ${X11_INCLUDE_DIR} ${X11_xf86vmode_INCLUDE_PATH})
target_link_libraries(gladventure m ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
				  ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
