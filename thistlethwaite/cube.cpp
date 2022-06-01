#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <unordered_map>
#include <queue>

#include <ctime>
#include <cstdlib>

#include "cube.hpp"

std::array<face_t,3> get_corner_face(corner_t corner){

    using namespace cube_defs;

    std::array<face_t,3> faces;

    byte_t ori = corner & 3;
    corner >>= 2;
    
    int inds[3];

    if(ori == 0){
        inds[kDirL] = kDirL;
        inds[kDirU] = kDirU;
        inds[kDirR] = kDirR;
    } else if (ori == 1) {
        inds[kDirL] = kDirU;
        inds[kDirU] = kDirR;
        inds[kDirR] = kDirL;
    } else { //ori == 2
        inds[kDirL] = kDirR;
        inds[kDirU] = kDirL;
        inds[kDirR] = kDirU;
    }
    
    switch(corner){
        case kFUL:
        faces[inds[kDirL]] = kL;
        faces[inds[kDirU]] = kU;
        faces[inds[kDirR]] = kF;
        break;
        case kBUL:
        faces[inds[kDirL]] = kB;
        faces[inds[kDirU]] = kU;
        faces[inds[kDirR]] = kL;
        break;
        case kBUR:
        faces[inds[kDirL]] = kR;
        faces[inds[kDirU]] = kU;
        faces[inds[kDirR]] = kB;
        break;
        case kFUR:
        faces[inds[kDirL]] = kF;
        faces[inds[kDirU]] = kU;
        faces[inds[kDirR]] = kR;
        break;
        case kFDR:
        faces[inds[kDirL]] = kR;
        faces[inds[kDirU]] = kD;
        faces[inds[kDirR]] = kF;
        break;
        case kFDL:
        faces[inds[kDirL]] = kF;
        faces[inds[kDirU]] = kD;
        faces[inds[kDirR]] = kL;
        break;
        case kBDL:
        faces[inds[kDirL]] = kL;
        faces[inds[kDirU]] = kD;
        faces[inds[kDirR]] = kB;
        break;
        case kBDR:
        faces[inds[kDirL]] = kB;
        faces[inds[kDirU]] = kD;
        faces[inds[kDirR]] = kR;
        break;
    }

    return faces;
}
std::array<face_t,2> get_edge_face(edge_t edge){

    using namespace cube_defs;

    byte_t ori = edge & 1;
    edge >>= 1;

    std::array<face_t,2> faces;

    switch(edge){
        case kFU:
        default:
        faces[0] = kU;
        faces[1] = kF;
        break;
        case kBU:
        faces[0] = kU;
        faces[1] = kB;
        break;
        case kUL:
        faces[0] = kU;
        faces[1] = kL;
        break;
        case kUR:
        faces[0] = kU;
        faces[1] = kR;
        break;
        case kFD:
        faces[0] = kD;
        faces[1] = kF;
        break;
        case kBD:
        faces[0] = kD;
        faces[1] = kB;
        break;
        case kDL:
        faces[0] = kD;
        faces[1] = kL;
        break;
        case kDR:
        faces[0] = kD;
        faces[1] = kR;
        break;
        case kFL:
        faces[0] = kF;
        faces[1] = kL;
        break;
        case kFR:
        faces[0] = kF;
        faces[1] = kR;
        break;
        case kBL:
        faces[0] = kB;
        faces[1] = kL;
        break;
        case kBR:
        faces[0] = kB;
        faces[1] = kR;
        break;
    }

    if(ori){
        face_t temp = faces[0];
        faces[0] = faces[1];
        faces[1] = temp;
    }

    return faces;
}
char get_face_char(face_t face) {

    using namespace cube_defs;

    switch(face){
        case kF:
        default:
        return 'F';
        case kB:
        return 'B';
        case kU:
        return 'U';
        case kD:
        return 'D';
        case kL:
        return 'L';
        case kR:
        return 'R';
    }
}

move_t get_move_opposite(move_t move){
    return move ^ 3;
}

std::vector<move_t> get_move_opposite(std::vector<move_t> moves){
    std::vector<move_t> invMoves;

    while(moves.size()){
        invMoves.push_back(get_move_opposite(moves.back()));
        moves.pop_back();
    }

    return invMoves;
}

move_t get_move_from_str(std::string str){
    move_t move;
    switch(str[0]){
        case 'L':
        default:
        move = 0;
        break;
        case 'R':
        move = 1;
        break;
        case 'U':
        move = 2;
        break;
        case 'D':
        move = 3;
        break;
        case 'F':
        move = 4;
        break;
        case 'B':
        move = 5;
        break;
    }

    move <<= 2;
    if(str.size() == 2){
        if(str[1] == '\'') move |= 0b10;
        else move |= 0b01;
    } else if(str.size() == 3) move |= 0b11;
    return move;
}

std::string get_str_from_move(move_t move){
    std::string str = "";
    bool ccw = (move & 0b10) >> 1;
    bool twice = move & 1;
    move >>= 2;

    str += get_face_char(move);
    if(twice) str += '2';
    if(ccw) str += '\'';

    return str;
}

std::vector<move_t> get_moves_from_str(std::string str){

    std::vector<move_t> moves;

    size_t start = 0;
    size_t len;

    do{
        len = 0;
        while(start + len < str.size() && str[start + len] != ' ') len++;
        moves.push_back(get_move_from_str(str.substr(start,len)));
        start += len + 1;
    } while (start < str.size());

    return moves;
}

std::string get_str_from_moves(std::vector<move_t> moves){
    if(moves.empty()) return "";

    std::string str = "";
    for(move_t move : moves) str += (get_str_from_move(move) + ' ');
    return str.substr(0,str.size()-1);
}

Cube::Cube(){
    for(corner_t corner = 0; corner < 8; corner++) this->corners[corner] = corner << 2;
    for(edge_t edge = 0; edge < 12; edge++) this->edges[edge] = edge << 1;

    this->setCompact();
}

Cube::Cube(const Cube &cube){
    this->corners = cube.corners;
    this->edges = cube.edges;
    this->compact = cube.compact;
}

Cube::Cube(compact_t compact){
    this->compact = compact;

    for(int ind = 11; ind >= 0; ind--){
        this->edges[ind] = compact & 0b11111;
        compact >>= 5;
    }
    for(int ind = 7; ind >= 0; ind--){
        this->corners[ind] = compact & 0b11111;
        compact >>= 5;
    }
}

void Cube::setCorner(int ind, corner_t corner) {this->corners[ind] = corner;}
void Cube::setEdge(int ind, edge_t edge) {this->edges[ind] = edge;}
corner_t Cube::getCorner(int ind) {return this->corners[ind];}
edge_t Cube::getEdge(int ind) {return this->edges[ind];}

void Cube::printCube(bool printSolved){

    using namespace cube_defs;

    std::array<std::array<face_t,3>,8> cornerFaces;
    std::array<std::array<face_t,2>,12> edgeFaces;

    for(int ind = 0; ind < 8; ind++) cornerFaces[ind] = get_corner_face(this->corners[ind]);
    for(int ind = 0; ind < 12; ind++) edgeFaces[ind] = get_edge_face(this->edges[ind]);

    std::cout << "   " << get_face_char(cornerFaces[kBUL][kDirU]) << get_face_char(edgeFaces[kBU][0]) << get_face_char(cornerFaces[kBUR][kDirU]) << std::endl;
    std::cout << "   " << get_face_char(edgeFaces[kUL][0]) << get_face_char(kU) << get_face_char(edgeFaces[kUR][0]) << std::endl;
    std::cout << "   " << get_face_char(cornerFaces[kFUL][kDirU]) << get_face_char(edgeFaces[kFU][0]) << get_face_char(cornerFaces[kFUR][kDirU]) << std::endl;

    std::cout << get_face_char(cornerFaces[kBUL][kDirR]) << get_face_char(edgeFaces[kUL][1]) << get_face_char(cornerFaces[kFUL][kDirL]);
    std::cout << get_face_char(cornerFaces[kFUL][kDirR]) << get_face_char(edgeFaces[kFU][1]) << get_face_char(cornerFaces[kFUR][kDirL]);
    std::cout << get_face_char(cornerFaces[kFUR][kDirR]) << get_face_char(edgeFaces[kUR][1]) << get_face_char(cornerFaces[kBUR][kDirL]);
    std::cout << get_face_char(cornerFaces[kBUR][kDirR]) << get_face_char(edgeFaces[kBU][1]) << get_face_char(cornerFaces[kBUL][kDirL]) << std::endl;

    std::cout << get_face_char(edgeFaces[kBL][1]) << get_face_char(kL) << get_face_char(edgeFaces[kFL][1]);
    std::cout << get_face_char(edgeFaces[kFL][0]) << get_face_char(kF) << get_face_char(edgeFaces[kFR][0]);
    std::cout << get_face_char(edgeFaces[kFR][1]) << get_face_char(kR) << get_face_char(edgeFaces[kBR][1]);
    std::cout << get_face_char(edgeFaces[kBR][0]) << get_face_char(kB) << get_face_char(edgeFaces[kBL][0]) << std::endl;

    std::cout << get_face_char(cornerFaces[kBDL][kDirL]) << get_face_char(edgeFaces[kDL][1]) << get_face_char(cornerFaces[kFDL][kDirR]);
    std::cout << get_face_char(cornerFaces[kFDL][kDirL]) << get_face_char(edgeFaces[kFD][1]) << get_face_char(cornerFaces[kFDR][kDirR]);
    std::cout << get_face_char(cornerFaces[kFDR][kDirL]) << get_face_char(edgeFaces[kDR][1]) << get_face_char(cornerFaces[kBDR][kDirR]);
    std::cout << get_face_char(cornerFaces[kBDR][kDirL]) << get_face_char(edgeFaces[kBD][1]) << get_face_char(cornerFaces[kBDL][kDirR]) << std::endl;

    std::cout << "   " << get_face_char(cornerFaces[kFDL][kDirU]) << get_face_char(edgeFaces[kFD][0]) << get_face_char(cornerFaces[kFDR][kDirU]) << std::endl;
    std::cout << "   " << get_face_char(edgeFaces[kDL][0]) << get_face_char(kD) << get_face_char(edgeFaces[kDR][0]) << std::endl;
    std::cout << "   " << get_face_char(cornerFaces[kBDL][kDirU]) << get_face_char(edgeFaces[kBD][0]) << get_face_char(cornerFaces[kBDR][kDirU]) << std::endl;

    if(printSolved) std::cout << (this->isComplete() ? "Solved" : "Unsolved") << std::endl;
}

void Cube::printOrientations(){
    for(corner_t corner : this->corners){
        std::cout << (int)(corner & 3) << ' ';
    }
    std::cout << std::endl;
    for(edge_t edge : this->edges){
        std::cout << (int)(edge & 1) << ' ';
    }
    std::cout << std::endl;
    std::cout << (this->cornersEdgesCorrectOrbit() ? "Correct orbits" : "Incorrect orbits") << std::endl;
}

void Cube::doMove(move_t move, bool updateCompact, bool printMove){

    using namespace cube_defs;

    if(printMove) std::cout << get_str_from_move(move) << std::endl;

    bool twice = move & 1;
    bool ccw = (move & 2) >> 1;
    move >>= 2;

    std::array<std::array<std::array<edge_t,3>,4>,6> cMoves = cornerMoves;
    std::array<std::array<std::array<edge_t,3>,4>,6> eMoves = edgeMoves;

    if(twice){
        cMoves = twiceCornerMoves;
        eMoves = twiceEdgeMoves;
    }
    else if(ccw){
        cMoves = primeCornerMoves;
        eMoves = primeEdgeMoves;
    }

    corner_t newCorners[4][2];
    edge_t newEdges[4][2];

    corner_t cornerOri;
    corner_t edgeOri;

    corner_t corner;
    edge_t edge;

    for(int ind = 0; ind < 4; ind++){
        newCorners[ind][0] = cMoves[move][ind][0];
        newEdges[ind][0] = eMoves[move][ind][0];

        corner = this->corners[cMoves[move][ind][1]];
        edge = this->edges[eMoves[move][ind][1]];

        cornerOri = ((corner & 3) + cMoves[move][ind][2]) % 3;
        edgeOri = ((edge & 1) + eMoves[move][ind][2]) % 2;

        newCorners[ind][1] = (corner & 0b11111100) | cornerOri;
        newEdges[ind][1] = (edge & 0b11111110) | edgeOri;
    }
    for(int ind = 0; ind < 4; ind++){
        this->corners[newCorners[ind][0]] = newCorners[ind][1];
        this->edges[newEdges[ind][0]] = newEdges[ind][1];
    }

    if(updateCompact) this->setCompact();
}

void Cube::doMoves(std::vector<move_t> moves, bool updateCompact, bool printMoves){
    for(move_t move : moves) this->doMove(move, false);
    if(updateCompact) this->setCompact();
}

void Cube::randomize(int times, bool printMoves){
    std::srand(std::time(nullptr));
    move_t move, prevFace;

    prevFace = 255;

    for(int n = 0; n < times; n++){
        do{
            move = rand() % 6;
            move <<= 2;
            move |= rand() % 3;
        }
        while(move >> 2 == prevFace);

        prevFace = move >> 2;
        this->doMove(move);
        if(printMoves) std::cout << get_str_from_move(move) << ' ';
    }
    if(printMoves) std::cout << std::endl;
}

bool Cube::isComplete(){
    for(corner_t corner = 0; corner < 8; corner++){
        if(this->corners[corner] != (corner << 2)) return false;
    }
    for(edge_t edge = 0; edge < 12; edge++){
        if(this->edges[edge] != (edge << 1)) return false;
    }
    return true;

    //return (this->compact >> 64) == 0xb0110c8531 && ((unsigned long)this->compact == 0xc008864298e84a96);
}

bool Cube::edgesOriented(){
    for(edge_t edge : this->edges){
        if(edge & 1) return false;
    }
    return true;
}

bool Cube::cornersOriented(){
    for(corner_t corner : this->corners){
        if(corner & 3) return false;
    }

    for(edge_t slice = 8; slice < 12; slice++){ //Also need to make sure that the FL, FR, BL, and BR edges are in the correct locations
        if((this->edges[slice] >> 1) < 8) return false;
    }

    return true;
}

bool Cube::cornersEdgesCorrectOrbit(){
    /*for(corner_t ind = 0; ind < 8; ind++){
        if(((this->corners[ind] >> 2) + ind) % 2) return false;
    }*/

    using namespace cube_defs;

    if(((this->corners[kFUL] >> 2) != kFUL) && ((this->corners[kFUL] >> 2) != kBUR)) return false;
    if(((this->corners[kBUR] >> 2) != kFUL) && ((this->corners[kBUR] >> 2) != kBUR)) return false;
    if(((this->corners[kFUR] >> 2) != kFUR) && ((this->corners[kFUR] >> 2) != kBUL)) return false;
    if(((this->corners[kBUL] >> 2) != kFUR) && ((this->corners[kBUL] >> 2) != kBUL)) return false;
    if(((this->corners[kFDL] >> 2) != kFDL) && ((this->corners[kFDL] >> 2) != kBDR)) return false;
    if(((this->corners[kBDR] >> 2) != kFDL) && ((this->corners[kBDR] >> 2) != kBDR)) return false;
    
    for(edge_t ind = 0; ind < 4; ind++){
        if((this->edges[ind] >> 1) >= 4) return false;
    }

    byte_t parity = 0;

    for(corner_t i = 0; i < 8; i++){
        for(corner_t j = i + 1; j < 8; j++) parity ^= this->corners[i] < this->corners[j];
    }

    return parity == 0;
}

void Cube::setCompact(){
    this->compact = 0;

    for(corner_t corner : this->corners){
        this->compact <<= 5;
        this->compact |= corner & 0b11111;
    }
    for(edge_t edge : this->edges){
        this->compact <<= 5;
        this->compact |= edge & 0b11111;
    }
}

compact_t Cube::getCompact(){
    return this->compact;
}