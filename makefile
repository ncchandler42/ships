CXXFLAGS=-std=c++17 -Wall -O3
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
OBJ=main.o ship.o

ships: $(OBJ)
	g++ -o ships $(OBJ) $(LDFLAGS)

main.o: main.cpp ai1.hpp ai2.hpp game.hpp
	g++ -c main.cpp $(CXXFLAGS)
ship.o: ship.hpp ship.cpp
	g++ -c ship.cpp $(CXXFLAGS)

clean:
	rm -f *.o ships
