BIN =   test_stringlist		\
	test_keybindings	\
	test_map		\
	test_objectlist		\
	random_object		\
	test_opengl

OPT = -DXA_DEBUG -DXA_BOOL -I.. -g -O0
GCC = gcc
LIB = -L/usr/X11R6/lib -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

test_stringlist.o =  	test_stringlist.o	\
			../stringlist.o		\
			../logger.o

test_keybindings.o =  	test_keybinding.o	\
			../keybinding.o		\
			../logger.o

test_map.o 	=  	test_map.o		\
			../map.o		\
			../logger.o		\
			../stringlist.o	

test_objectlist.obj=  	test_objectlist.o	\
			../object_list.o	\
			../logger.o

random_object.obj = 	random_object.o		\
			../load_objects.o	\
			../object_list.o	\
			../logger.o

test_opengl.obj	  = 	test_opengl.o		\
			../opengl.o

VAL 	= valgrind --leak-check=full --show-reachable=yes
all: $(BIN)

test_stringlist: $(test_stringlist.o)
	$(GCC) -o $@ $^ $(LIB)

test_keybindings: $(test_keybindings.o)
	$(GCC) -o $@ $^ $(LIB)

test_map: $(test_map.o)
	$(GCC) -o $@ $^ $(LIB)

test_objectlist: $(test_objectlist.obj)
	$(GCC) -o $@ $^ $(LIB)

random_object: $(random_object.obj)
	$(GCC) -o $@ $^ $(LIB)  -lm

test_opengl: $(test_opengl.obj)
	$(GCC) -o $@ $^ $(LIB)  -lm

%.o: %.c
	$(GCC) -o $@ -c $< $(OPT) 

clean:
	rm -f *~
	rm -f *.o

valgrind-sl:
	$(VAL) ./test_stringlist

valgrind-kb:
	$(VAL) ./test_keybindings

valgrind-map:
	$(VAL) ./test_map

valgrind-ol:
	$(VAL) ./test_objectlist
