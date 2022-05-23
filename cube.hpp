#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <string>

#define FACE_NUM 6
#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_UP 2
#define FACE_DOWN 3
#define FACE_LEFT 4
#define FACE_RIGHT 5

#define COL_RED 0
#define COL_ORANGE 1
#define COL_YELLOW 2
#define COL_WHITE 3
#define COL_BLUE 4
#define COL_GREEN 5

#define DIR_X 0
#define DIR_Y 1
#define DIR_Z 2

using byte_t = unsigned char;
using col_t = byte_t;
using cubie_t = uint32_t;

struct Move {
    byte_t dir;
    int depth;
    int num;
    bool ccw;
    std::string moveStr;

    Move();
    Move(byte_t, int, bool);
    Move(byte_t, int, int, bool);
    Move(std::string);
    void printMove();
};

class Cube {

    std::vector<cubie_t> cubies;
    int cubeSize;
    int faceSize;
    std::vector<std::vector<int>> *rings;
    std::vector<Move> moves;

    int (*heuristicFunc)(Cube);
    //int score;

    std::vector<int> getFaceInds(byte_t, int);
    std::vector<std::vector<int>> makeRing(byte_t, int);
    std::vector<std::vector<int>> getRing(byte_t, int);

    void printFace(byte_t, int, byte_t);
    void rotateFace(byte_t, int, bool);
    //bool isValid();

    public:

    Cube(int);
    Cube(int, int(*)(Cube));
    Cube(const Cube&);
    ~Cube();

    bool operator==(const Cube&);

    void printCube();
    bool isComplete();
    int getCubeSize() {return this->cubeSize;}
    std::vector<cubie_t> getCubies() {return this->cubies;}
    std::vector<Move> getMoves() {return this->moves;}
    int getScore() {return this->heuristicFunc(*this);}
    col_t getCubieFaceCol(byte_t, int, int, int);
    col_t getFaceCol(byte_t, int, int);

    void setHeuristicFunc(int (*)(Cube));
    void rotateCube(byte_t, bool);
    void randomize(int);
    void doMove(Move, bool = true);
    void doMoves(std::vector<Move>, bool = true);
    void printMoves();
    void reverseFromMoves();

};

std::vector<Move> getMovesFromStr(std::string);

cubie_t newCubie();
void printCubie(cubie_t&);
col_t getCubieFace(cubie_t, byte_t);
void setCubieFace(cubie_t&, byte_t, col_t);
void rotateCubie(cubie_t&, byte_t, bool);

#endif