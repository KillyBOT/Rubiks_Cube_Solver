CC = g++
CXX = g++

INCLUDES  = 

CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g

LDLIBS = 

main: main.o cube.o solve.o

main.o: main.cpp cube.hpp solve.hpp

cube.o: cube.cpp cube.hpp

solve.o: solve.cpp cube.hpp solve.hpp

.PHONY: clean
clean:
	rm -f *.o a.out main

.PHONY: all
all: clean main