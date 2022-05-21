CC = g++
CXX = g++

INCLUDES  = 

CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g

LDLIBS = 

main: main.o cube.o solve_stupid.o

main.o: main.cpp cube.hpp

cube.o: cube.cpp cube.hpp

solve_stupid.o: solve_stupid.cpp cube.hpp

.PHONY: clean
clean:
	rm -f *.o a.out main

.PHONY: all
all: clean main