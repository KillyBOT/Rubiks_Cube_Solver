#ifndef CUBE_H
#define CUBE_H

#include <vector>
#include <string>

#include "cubie.hpp"
#include "move.hpp"

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
    bool isOriented();
    int getCubeSize() {return this->cubeSize;}
    std::vector<cubie_t> getCubies() {return this->cubies;}
    std::vector<Move> getMoves() {return this->moves;}
    int getScore() {return this->heuristicFunc(*this);}
    col_t getCubieFaceCol(byte_t, int, int, int);
    col_t getFaceCol(byte_t, int, int);

    void setHeuristicFunc(int (*)(Cube));
    void rotateCube(byte_t, bool);
    void fixOrientation();
    void randomize(int);
    void doMove(Move, bool = true);
    void doMoves(std::vector<Move>, bool = true);
    void printMoves();
    void reverseFromMoves();

    friend int heuristic_orientation(Cube);
};

#endif