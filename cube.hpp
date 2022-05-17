#ifndef CUBE_H
#define CUBE_H

#include <vector>

#define FACE_NUM 6
#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_TOP 2
#define FACE_RIGHT 3
#define FACE_LEFT 4
#define FACE_BOTTOM 5

#define COL_RED 0
#define COL_ORANGE 1
#define COL_YELLOW 2
#define COL_GREEN 3
#define COL_BLUE 4
#define COL_WHITE 5

#define CLOCKWISE_V 0
#define C_CLOCKWISE_V 1
#define CLOCKWISE_H 2
#define C_CLOCKWISE_H 3

enum eMove {MOVE_F, MOVE_R, MOVE_U, MOVE_L, MOVE_B, MOVE_D};

using byte_t = unsigned char;
using col_t = byte_t;
using move_t = byte_t;
using cubie_t = uint32_t;

const std::string faceStrings[] = {"Front", "Back", "Top", "Right", "Left", "Bottom"};
const std::string colStrings[] = {"R","O","Y","G","B","W"};

class Cube {
    cubie_t *cubeData;
    int cubeSize;
    int faceSize;
    std::vector<move_t> moves;

    public:
    Cube(int);
    ~Cube();
    void printCube();
};

cubie_t newCubie();
void printCubie(cubie_t&);
col_t getCubieFace(cubie_t, byte_t);
void setCubieFace(cubie_t&, byte_t, col_t);
void rotateCubie(cubie_t&, byte_t);

#endif