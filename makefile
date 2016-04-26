all: game

game: game.o main.o
	g++ main.o game.o -lSDL -lSDL_image -lSDL_mixer -o game

main.o:
	g++ -lSDL -lSDL_image -lSDL_mixer -c main.cpp

game.o:
	g++ -lSDL -lSDL_image -lSDL_mixer -c game.cpp

clean:
	rm game *.o *~





 



