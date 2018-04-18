cmake_minimum_required (VERSION 2.6)

project (gladventure)

# XA_DEBUG adds a lot of console output
add_definitions(-DXA_BOOL) # -DXA_DEBUG)
set( CMAKE_VERBOSE_MAKEFILE on )

# Generate a config file
set(VERSION 0.3.90)
set(REVISION 0)
configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

include_directories(./src)
# To be able to include config.h
include_directories(${PROJECT_BINARY_DIR})

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
find_package (Threads)

find_package(PkgConfig)
pkg_search_module(CHECK check REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} 
		     ${X11_INCLUDE_DIR} ${X11_xf86vmode_INCLUDE_PATH})
target_link_libraries(gladventure m ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
				  ${X11_LIBRARIES} ${X11_Xxf86vm_LIB})
				
				
# Build	unit tests
IF( CHECK_FOUND )
  enable_testing()
  set(test-src
    src/colors_t.c
    src/tests.c
    
    src/colors.c
    )
  add_executable(gladventure-tests ${test-src})
  target_link_libraries(gladventure-tests m ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} 
    ${X11_LIBRARIES} ${X11_Xxf86vm_LIB} ${CHECK_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT})
  add_test(check gladventure-tests)
  add_custom_target(check COMMAND gladventure-tests)
ENDIF( CHECK_FOUND )
