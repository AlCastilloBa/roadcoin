#OBJS specifies which files to compile as part of the project 
OBJS = src/game.c src/camera.c src/physics.c src/maps.c src/geometry.c src/options.c src/menu.c src/graphics.c src/sound.c src/enemies.c

#CC specifies which compiler we're using 
# CC = g++ 
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using 
# -w suppresses all warnings 
# COMPILER_FLAGS = -w 
COMPILER_FLAGS = `sdl2-config --cflags`

#LINKER_FLAGS specifies the libraries we're linking against 
#LINKER_FLAGS = -lSDL2 
LINKER_FLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable 
OBJ_NAME = roadcoin 

#This is the target that compiles our executable 
all : $(OBJS) 
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

