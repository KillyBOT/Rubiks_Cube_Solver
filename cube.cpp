#include <iostream>
#include <vector>

#include "cube.hpp"

Cube::Cube(int size) : cubeSize(size){
    cubeData = new int*[6];
    for(int face = 0; face < 6; face++){
        cubeData[face] = new int[size * size];
    }
}

Cube::~Cube(){
    for(int face = 0; face < 6; face++){
        delete[] cubeData[face];
    }
    delete[] cubeData;
}