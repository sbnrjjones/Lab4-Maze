CFLAGS=-std=c++11 -g

all: main.cpp Pathfinder.cpp Pathfinder.h PathfinderInterface.h
	g++ $(CFLAGS) -o main main.cpp Pathfinder.cpp
	./main