#include <iostream>
#include <vector>
#include <string>

#include "cube.hpp"

byte_t clockwiseVFaces[] = {FACE_TOP, FACE_LEFT, FACE_BOTTOM, FACE_RIGHT};
byte_t cClockwiseVFaces[] = {FACE_TOP, FACE_RIGHT, FACE_BOTTOM, FACE_LEFT};
byte_t clockwiseHFaces[] = {FACE_BACK, FACE_LEFT, FACE_FRONT, FACE_RIGHT};
byte_t cClockwiseHFaces[] = {FACE_BACK, FACE_RIGHT, FACE_FRONT, FACE_LEFT};

void printCubie(cubie_t &cubie){
    std::cout << " " << colStrings[getCubieFace(cubie,FACE_TOP)] << std::endl;
    std::cout << colStrings[getCubieFace(cubie,FACE_LEFT)] << colStrings[getCubieFace(cubie,FACE_FRONT)] << colStrings[getCubieFace(cubie,FACE_RIGHT)] << colStrings[getCubieFace(cubie,FACE_BACK)] << std::endl;
    std::cout << " " << colStrings[getCubieFace(cubie,FACE_BOTTOM)] << std::endl;
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

void rotateCubie(cubie_t &cubie, byte_t rotationType){
    col_t temp;
    byte_t *rotType;

    switch(rotationType){
        case CLOCKWISE_V:
            rotType = clockwiseVFaces;
            break;
        case C_CLOCKWISE_V:
            rotType = cClockwiseVFaces;
            break;
        case CLOCKWISE_H:
            rotType = clockwiseHFaces;
            break;
        case C_CLOCKWISE_H:
            rotType = cClockwiseHFaces;
            break;
        default:
            rotType = clockwiseVFaces;
            break;
    }

    temp = getCubieFace(cubie, rotType[0]);

    for(int x = 0; x < 4; x++)setCubieFace(cubie, rotType[x], getCubieFace(cubie,rotType[x+1]));
    setCubieFace(cubie,rotType[3],temp);
    
}