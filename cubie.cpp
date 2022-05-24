#include <iostream>
#include <vector>
#include <string>

#include "cubie.hpp"

static byte_t xFaces[] = {FACE_UP, FACE_LEFT, FACE_DOWN, FACE_RIGHT};
static byte_t yFaces[] = {FACE_BACK, FACE_LEFT, FACE_FRONT, FACE_RIGHT};
static byte_t zFaces[] = {FACE_UP, FACE_BACK, FACE_DOWN, FACE_FRONT};

void printCubie(cubie_t &cubie){
    std::cout << " " << colStrings[getCubieFace(cubie,FACE_UP)] << std::endl;
    std::cout << colStrings[getCubieFace(cubie,FACE_LEFT)] << colStrings[getCubieFace(cubie,FACE_FRONT)] << colStrings[getCubieFace(cubie,FACE_RIGHT)] << colStrings[getCubieFace(cubie,FACE_BACK)] << std::endl;
    std::cout << " " << colStrings[getCubieFace(cubie,FACE_DOWN)] << std::endl;
}
cubie_t newCubie(){
    cubie_t cubie = 0;
    for(int x = FACE_NUM - 1; x >= 0; x--){
        cubie <<= 5;
        cubie |= x;
    }

    return cubie;
}
col_t getCubieFace(cubie_t cubie, byte_t face){
    cubie >>= face * 5;
    cubie &= 0b11111;
    return (col_t)cubie;
}
void setCubieFace(cubie_t &cubie, byte_t face, col_t col){
    cubie &= ~(0b11111 << (face * 5));
    cubie |= ((cubie_t)col) << (face * 5);
}
void rotateCubie(cubie_t &cubie, byte_t dir, bool counterClockwise){
    col_t temp;
    byte_t *rotType;

    switch(dir){
        case DIR_X:
        default:
        rotType = xFaces;
        break;
        case DIR_Y:
        rotType = yFaces;
        break;
        case DIR_Z:
        rotType = zFaces;
        break; 
    }

    if(counterClockwise){
        temp = getCubieFace(cubie,rotType[3]);
        for(int x = 3; x > 0; x--) setCubieFace(cubie,rotType[x],getCubieFace(cubie,rotType[x-1]));
        setCubieFace(cubie,rotType[0],temp);
    } else {
        temp = getCubieFace(cubie,rotType[0]);
        for(int x = 0; x < 3; x++) setCubieFace(cubie,rotType[x],getCubieFace(cubie,rotType[x+1]));
        setCubieFace(cubie,rotType[3],temp);
    }
    
}