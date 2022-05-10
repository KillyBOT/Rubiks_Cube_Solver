#ifndef CUBE_H
#define CUBE_H

#include <vector>

#define FACE_TOP 0
#define FACE_LEFT 1
#define FACE_FRONT 2
#define FACE_RIGHT 3
#define FACE_BACK 4
#define FACE_BOTTOM 5

using byte_t = unsigned char;
using col_t = int;
using move_t = byte_t;

class Cube {
    col_t **cubeData;
    int cubeSize;
    std::vector<move_t> moveList;

    public:
    Cube(int);
    ~Cube();
    void printFace(int);
    void printCube();
    bool checkIfCompleted();

    static void printMove(int);
    void printMoves();
    void doMove(int);
};

#endif