CC = g++
CXX = g++

CXXFLAGS = -g -Wall

LDFLAGS = -g

LDLIBS = 

main: main.o thistlethwaite.o korf.o cube.o

main.o: main.cpp cube.hpp solve.hpp

thistlethwaite.o: thistlethwaite.cpp cube.hpp solve.hpp

korf.o: korf.cpp cube.hpp solve.hpp

cube.o: cube.cpp cube.hpp

.PHONY: clean
clean:
	rm -f *.o a.out main

.PHONY: all
all: clean main