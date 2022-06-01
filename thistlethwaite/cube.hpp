#ifndef CUBE_H
#define CUBE_H

#include <string>
#include <array>
#include <vector>
#include <unordered_map>

//This time, I'll put FB on the z axis, and BG on the X axis

using byte_t = unsigned char;
using face_t = byte_t;
using col_t = byte_t; //5th bit is RO, 6th bit is WY, 7th bit is BG
using corner_t = byte_t; //First 6 bits are location, last 2 are orientation
using edge_t = byte_t; //First 7 bits are location, last is orientation
using move_t = byte_t; //First 6 bits are face, then 1 for if move is counterclockwise, finally 1 if move should be done twice

//This only compiles with GCC, I'll need to fix this later
using compact_t = __uint128_t;

namespace cube_defs{

    /*const int kDirX = 0;
    const int kDirY = 1;
    const int kDirZ = 2;*/

    const int kDirL = 0;
    const int kDirU = 1;
    const int kDirR = 2;

    const face_t kL = 0;
    const face_t kR = 1;
    const face_t kU = 2;
    const face_t kD = 3;
    const face_t kF = 4;
    const face_t kB = 5;

    const corner_t kFUL = 0;
    const corner_t kBUL = 1;
    const corner_t kBUR = 2;
    const corner_t kFUR = 3;
    const corner_t kFDR = 4;
    const corner_t kFDL = 5;
    const corner_t kBDL = 6;
    const corner_t kBDR = 7;

    /*const corner_t kFUL = 0;
    const corner_t kBUR = 1;
    const corner_t kFUR = 2;
    const corner_t kBUL = 3;
    const corner_t kFDL = 4;
    const corner_t kBDR = 5;
    const corner_t kFDR = 6;
    const corner_t kBDL = 7;*/

    /*const corner_t kBUL = 0;
    const corner_t kBUR = 1;
    const corner_t kFUR = 2;
    const corner_t kFUL = 3;
    const corner_t kFDL = 4;
    const corner_t kBDL = 5;
    const corner_t kBDR = 6;
    const corner_t kFDR = 7;*/

    const edge_t kFU = 0;
    const edge_t kBU = 1;
    const edge_t kBD = 2;
    const edge_t kFD = 3;
    const edge_t kUL = 4;
    const edge_t kUR = 5;
    const edge_t kDR = 6;
    const edge_t kDL = 7;
    const edge_t kFL = 8;
    const edge_t kFR = 9;
    const edge_t kBR = 10;
    const edge_t kBL = 11;

    const std::array<std::array<std::array<corner_t,3>,4>,6> cornerMoves //cornerMoves[move][corner] = {ind,replaceInd,oriAdd}
    =
    {
        kFUL,kBUL,1,kBUL,kBDL,2,kBDL,kFDL,1,kFDL,kFUL,2,//L
        kFUR,kFDR,2,kBUR,kFUR,1,kBDR,kBUR,2,kFDR,kBDR,1,//R
        kFUL,kFUR,0,kBUL,kFUL,0,kBUR,kBUL,0,kFUR,kBUR,0,//U
        kFDL,kBDL,0,kBDL,kBDR,0,kBDR,kFDR,0,kFDR,kFDL,0,//D
        kFUL,kFDL,2,kFUR,kFUL,1,kFDR,kFUR,2,kFDL,kFDR,1,//F
        kBUL,kBUR,1,kBUR,kBDR,2,kBDL,kBUL,2,kBDR,kBDL,1//B
    };

    const std::array<std::array<std::array<edge_t,3>,4>,6> edgeMoves
    =
    {
        kFL,kUL,0,kUL,kBL,0,kBL,kDL,0,kDL,kFL,0,//L
        kFR,kDR,0,kUR,kFR,0,kBR,kUR,0,kDR,kBR,0,//R
        kFU,kUR,0,kUR,kBU,0,kBU,kUL,0,kUL,kFU,0,//U
        kFD,kDL,0,kDL,kBD,0,kBD,kDR,0,kDR,kFD,0,//D
        kFU,kFL,1,kFL,kFD,1,kFD,kFR,1,kFR,kFU,1,//F
        kBU,kBR,1,kBR,kBD,1,kBD,kBL,1,kBL,kBU,1//B
    };

    const std::array<std::array<std::array<corner_t,3>,4>,6> primeCornerMoves  //cornerMoves[move][corner] = {ind,replaceInd,oriAdd}
    =
    {
        kFUL,kFDL,1,kFDL,kBDL,2,kBDL,kBUL,1,kBUL,kFUL,2,//L
        kFUR,kBUR,2,kBUR,kBDR,1,kBDR,kFDR,2,kFDR,kFUR,1,//R
        kFUL,kBUL,0,kBUL,kBUR,0,kBUR,kFUR,0,kFUR,kFUL,0,//U
        kFDL,kFDR,0,kFDR,kBDR,0,kBDR,kBDL,0,kBDL,kFDL,0,//D
        kFUL,kFUR,2,kFUR,kFDR,1,kFDR,kFDL,2,kFDL,kFUL,1,//F
        kBUL,kBDL,1,kBDL,kBDR,2,kBDR,kBUR,1,kBUR,kBUL,2//B
    };

    const std::array<std::array<std::array<edge_t,3>,4>,6> primeEdgeMoves 
    =
    {
        kFL,kDL,0,kDL,kBL,0,kBL,kUL,0,kUL,kFL,0,//L
        kFR,kUR,0,kUR,kBR,0,kBR,kDR,0,kDR,kFR,0,//R
        kFU,kUL,0,kUL,kBU,0,kBU,kUR,0,kUR,kFU,0,//U
        kFD,kDR,0,kDR,kBD,0,kBD,kDL,0,kDL,kFD,0,//D
        kFU,kFR,1,kFR,kFD,1,kFD,kFL,1,kFL,kFU,1,//F
        kBU,kBL,1,kBL,kBD,1,kBD,kBR,1,kBR,kBU,1//B
    };

    const std::array<std::array<std::array<corner_t,3>,4>,6> twiceCornerMoves  //cornerMoves[move][corner] = {ind,replaceInd,oriAdd}
    =
    {
        kFUL,kBDL,0,kBUL,kFDL,0,kBDL,kFUL,0,kFDL,kBUL,0,//L
        kFUR,kBDR,0,kBUR,kFDR,0,kBDR,kFUR,0,kFDR,kBUR,0,//R
        kFUL,kBUR,0,kBUL,kFUR,0,kBUR,kFUL,0,kFUR,kBUL,0,//U
        kFDL,kBDR,0,kBDL,kFDR,0,kBDR,kFDL,0,kFDR,kBDL,0,//D
        kFUL,kFDR,0,kFDL,kFUR,0,kFDR,kFUL,0,kFUR,kFDL,0,//F
        kBUL,kBDR,0,kBDL,kBUR,0,kBDR,kBUL,0,kBUR,kBDL,0//B
    };

    const std::array<std::array<std::array<edge_t,3>,4>,6> twiceEdgeMoves 
    =
    {
        kFL,kBL,0,kDL,kUL,0,kBL,kFL,0,kUL,kDL,0,//L
        kFR,kBR,0,kUR,kDR,0,kBR,kFR,0,kDR,kUR,0,//R
        kFU,kBU,0,kUL,kUR,0,kBU,kFU,0,kUR,kUL,0,//U
        kFD,kBD,0,kDR,kDL,0,kBD,kFD,0,kDL,kDR,0,//D
        kFU,kFD,0,kFR,kFL,0,kFD,kFU,0,kFL,kFR,0,//F
        kBU,kBD,0,kBL,kBR,0,kBD,kBU,0,kBR,kBL,0//B
    };



}

class Cube {
    std::array<corner_t, 8> corners;
    std::array<edge_t, 12> edges;
    compact_t compact;

    public:

    Cube();
    Cube(const Cube&);
    Cube(compact_t);

    void printCube(bool = false);
    void printOrientations();

    void setCompact();
    void doMove(move_t, bool = true, bool = false);
    void doMoves(std::vector<move_t>, bool = true, bool = false);
    void randomize(int = 50, bool = false);

    bool edgesOriented(); //In G1?
    bool cornersOriented(); //In G2?
    bool cornersEdgesCorrectOrbit(); //In G3?
    bool isComplete(); //In G4?

    compact_t getCompact();
};

std::array<face_t,3> get_corner_face(corner_t);
std::array<face_t,2> get_edge_face(edge_t);
char get_face_char(face_t);

move_t get_move_opposite(move_t);
move_t get_move_from_str(std::string);
std::string get_str_from_move(move_t);
std::vector<move_t> get_moves_from_str(std::string);
std::string get_str_from_moves(std::vector<move_t>);

std::unordered_map<compact_t,compact_t> generate_move_map();
void write_move_map(std::string, std::unordered_map<compact_t,compact_t>);
std::unordered_map<compact_t,compact_t> read_move_map(std::string);

#endif