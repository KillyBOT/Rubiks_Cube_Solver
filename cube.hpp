#ifndef CUBE_H
#define CUBE_H

#include <vector>

#define FACE_NUM 6
#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_UP 2
#define FACE_RIGHT 3
#define FACE_LEFT 4 
#define FACE_DOWN 5

#define COL_RED 0
#define COL_ORANGE 1
#define COL_YELLOW 2
#define COL_GREEN 3
#define COL_BLUE 4
#define COL_WHITE 5

#define ROT_CLOCK 0
#define ROT_COUNTCLOCK 4

#define DIR_X 0
#define DIR_Y 1
#define DIR_Z 2

enum eMove {MOVE_F, MOVE_R, MOVE_U, MOVE_L, MOVE_B, MOVE_D};

using byte_t = unsigned char;
using col_t = byte_t;
using move_t = byte_t;
using cubie_t = uint32_t;

class Cube {
    cubie_t *cubies;
    int cubeSize;
    int faceSize;
    std::vector<std::vector<int>> *rings;
    std::vector<move_t> moves;

    std::vector<int> getFaceInds(byte_t, int);
    std::vector<std::vector<int>> makeRing(byte_t, int);
    std::vector<std::vector<int>> getRing(byte_t, int);

    void printFace(byte_t, int, byte_t);
    void rotateFace(byte_t, int, bool);

    public:

    Cube(int);
    ~Cube();

    void printCube();
    bool isComplete();

    void randomize();
    void doMove(move_t);
    void printMoves();
};



cubie_t newCubie();
void printCubie(cubie_t&);
col_t getCubieFace(cubie_t, byte_t);
void setCubieFace(cubie_t&, byte_t, col_t);
void rotateCubie(cubie_t&, byte_t, bool);

#endif