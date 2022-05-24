CC = g++
CXX = g++

INCLUDES = 
DEPS = cubie.hpp move.hpp cube.hpp solve.hpp
OBJ = main.o cubie.o move.o cube.o solve.o

CXXFLAGS = -g -Wall $(INCLUDES)

LDFLAGS = -g

LDLIBS = 

main: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: clean
clean:
	rm -f *.o a.out main

.PHONY: all
all: clean main