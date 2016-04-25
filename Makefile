all: game

game: game.o main.o
	g++ main.o game.o -lSDL -lSDL_image -lSDL_mixer -o game

main.o:
	g++ -lSDL -lSDL_image -lSDL_mixer -c main.cpp

game.o:
	g++ -lSDL -lSDL_image -lSDL_mixer -c game.cpp

clean:
	rm game *.o 






#OBJS specifies which files to compile as part of the project
#OBJS = test.cpp

#CC specifies which compiler we're using
#CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
#COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
#LINKER_FLAGS = -lSDL -lSDL_image -lSDL_mixer

#OBJ_NAME specifies the name of our exectuable
#OBJ_NAME = test

#This is the target that compiles our executable
#all : $(OBJS)
#	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

#clean : 
#	rm $(OBJ_NAME) 



