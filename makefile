CC = gcc
CFLAGS = -g
OBJ_DIR = ./obj

all: debug
	
run: debug
	./target/debug

debug: check utils strings
	@ $(CC) -lm $(CFLAGS) $(OBJ_DIR)/strings.o src/main.c $(OBJ_DIR)/utils.o src/sample.c src/triogons.c -o target/debug

utils:
	@ $(CC) -c ./lib/utils.c -o $(OBJ_DIR)/utils.o $(CFLAGS)

strings:
	@ $(CC) -c ./lib/strings.c -o $(OBJ_DIR)/strings.o $(CFLAGS)

check: ./obj ./target
	
./obj:
	@echo "Creating directory ./obj"
	mkdir -p ./obj

./target:
	@echo "Creating directroy ./target"
	mkdir -p ./target

clean:
	@ rm $(OBJ_DIR)/* target/debug

