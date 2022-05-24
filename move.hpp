#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <vector>

#include "cubie.hpp"

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

std::vector<Move> getMovesFromStr(std::string);

const std::string basicMoveStr = "F F2 F\' B B2 B\' U U2 U\' D D2 D\' L L2 L\' R R2 R\'";

#endif