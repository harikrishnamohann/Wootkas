CC = gcc
CFLAGS = -g
OBJ_DIR = ./obj

all: debug
	
run: debug
	./target/debug

debug: utils strings
	@ $(CC) -lm $(CFLAGS) $(OBJ_DIR)/strings.o src/main.c $(OBJ_DIR)/utils.o -o target/debug

list:
	@ $(CC) -c ./lib/list.c -o $(OBJ_DIR)/list.o $(CFLAGS)

arena:
	@ $(CC) -c ./lib/arena_allocator/arena.c -o $(OBJ_DIR)/arena.o $(CFLAGS)

utils:
	@ $(CC) -c ./lib/utils.c -o $(OBJ_DIR)/utils.o $(CFLAGS)

strings:
	@ $(CC) -c ./lib/strings.c -o $(OBJ_DIR)/strings.o $(CFLAGS)

clean:
	@ rm $(OBJ_DIR)/* target/debug

