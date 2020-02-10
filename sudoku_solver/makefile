CXX = gcc

CXXFLAGS = -Wall -std=c99
LDFLAGS = -lSDL2 -lSDL2_ttf -lm
OBJECTS = main.o utils.o solver.o 

all:sudoku_solver

sudoku_solver:$(OBJECTS)
	$(CXX) -g $(OBJECTS) -o sudoku_solver $(LDFLAGS)

utils.o:utils.c utils.h
	$(CXX) $(CXXFLAGS) -c utils.c -o utils.o

main.o:main.c
	$(CXX) $(CXXFLAGS) -c main.c -o main.o

solver.o:solver.c solver.h
	$(CXX) $(CXXFLAGS) -c solver.c -o solver.o

clean:
	rm *.o







