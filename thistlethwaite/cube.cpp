#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <unordered_map>
#include <queue>

#include <ctime>
#include <cstdlib>

#include "cube.hpp"

std::array<face_t,3> get_corner_face(corner_t corner, byte_t ori){

    using namespace cube_defs;

    std::array<face_t,3> faces;
    
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
std::array<face_t,2> get_edge_face(edge_t edge, byte_t ori){

    using namespace cube_defs;

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
    return move ^ 2;
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
    for(corner_t corner = 0; corner < 8; corner++) {
        this->corners[corner] = corner;
        this->cornerOri[corner] = 0;
    }
    for(edge_t edge = 0; edge < 12; edge++) {
        this->edges[edge] = edge;
        this->edgeOri[edge] = 0;
    }

    this->setCompact();
}

Cube::Cube(const Cube &cube){
    this->corners = cube.corners;
    this->edges = cube.edges;
    this->cornerOri = cube.cornerOri;
    this->edgeOri = cube.edgeOri;
    this->compact = cube.compact;
}

Cube::Cube(compact_t compact){
    this->compact = compact;

    for(int ind = 11; ind >= 0; ind--){
        this->edgeOri[ind] = compact & 1;
        compact >>= 1;
        this->edges[ind] = compact & 0b1111;
        compact >>= 4;
    }
    for(int ind = 7; ind >= 0; ind--){
        this->cornerOri[ind] = compact & 0b11;
        compact >>= 2;
        this->corners[ind] = compact & 0b111;
        compact >>= 3;
    }
}

void Cube::setCorner(int ind, corner_t corner, byte_t ori) {
    this->corners[ind] = corner;
    this->cornerOri[ind] = ori;
}
void Cube::setEdge(int ind, edge_t edge, byte_t ori) {
    this->edges[ind] = edge;
    this->edges[ind] = ori;
}
corner_t Cube::getCorner(int ind) {return this->corners[ind];}
edge_t Cube::getEdge(int ind) {return this->edges[ind];}

/*int Cube::getEdgeOrientation(){
    int ans = 0;
    for(int i = 0; i < 11; i++){
        ans += this->edgeOri[i] << i;
    }
    return ans;
}
int Cube::getCornerOrientation(){
    int ans = 0;
    int num = 1;
    for(int i = 0; i < 7; i++){
        ans += this->cornerOri[i] * num;
        num *= 3;
    }

    return ans;
}
int Cube::getEdgePermutation(){
    int ans = 0;
    int num = 1;
    for(int i = 1; i < 12; i++){
        ans += num * this->edges[i];
        num *= i + 1;
    }
    return ans;
}
int Cube::getCornerPermutation(){
    int ans = 0;
    int num = 1;
    for(int i = 1; i < 8; i++){
        ans += num * this->corners[i];
        num *= i + 1;
    }
    return ans;
}*/

/*int Cube::getG2Ind(){
    int ans;
    int num = 1;
    for(int i = 0; i > 7; i++){
        ans += 
    }
}*/

inline int factorial(int n){
    int ans = 1;
    for(int i = 2; i <= n; i++) ans *= i;
    return ans;
}

inline int npk(int n, int k){
    return factorial(n) / factorial(n - k);
}

int Cube::korfGetEdge1Ind(){
    int ans = 0;
    int toAdd;
    for(int i = 0; i < 6; i++){
        toAdd = this->edges[i];
        for(int j = 0; j < i; j++){
            if(this->edges[i] > this->edges[j]) toAdd--;
        }
        ans += toAdd * npk(11-i,5-i);
    }

    for(int ind = 0; ind < 6; ind++){
        ans <<= 1;
        ans |= this->edgeOri[ind];
    }

    return ans;
}

int Cube::korfGetEdge2Ind(){
    int ans = 0;
    int toAdd;
    for(int i = 6; i < 12; i++){
        toAdd = this->edges[i];
        for(int j = 6; j < i; j++){
            if(this->edges[i] > this->edges[j]) toAdd--;
        }
        ans += toAdd * npk(17-i,11-i); 
    }

    for(int ind = 6; ind < 12; ind++){
        ans <<= 1;
        ans |= this->edgeOri[ind];
    }

    return ans;
}

int Cube::korfGetCornerInd(){
    int ans = 0;
    int num = 1;
    int toAdd;
    for(int i = 1; i < 8; i++){
        toAdd = 0;
        num *= i;
        for(int j = 0; j < i; j++){
            if(this->corners[i] < this->corners[j]) toAdd++;
        }
        ans += toAdd * num;
    }
    ans *= 2187; //3^7
    num = 1;
    for(int ind = 0; ind < 7; ind++){
        ans += this->cornerOri[ind] * num;
        num *= 3;
    }

    return ans;
}

void Cube::printCube(bool printSolved){

    using namespace cube_defs;

    std::array<std::array<face_t,3>,8> cornerFaces;
    std::array<std::array<face_t,2>,12> edgeFaces;

    for(int ind = 0; ind < 8; ind++) cornerFaces[ind] = get_corner_face(this->corners[ind], this->cornerOri[ind]);
    for(int ind = 0; ind < 12; ind++) edgeFaces[ind] = get_edge_face(this->edges[ind], this->edgeOri[ind]);

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
    for(byte_t ori : this->cornerOri){
        std::cout << (int)(ori) << ' ';
    }
    std::cout << std::endl;
    for(byte_t ori : this->edgeOri){
        std::cout << (int)(ori) << ' ';
    }
    std::cout << std::endl;
    std::cout << (this->cornersEdgesCorrectOrbit() ? "Correct orbits" : "Incorrect orbits") << std::endl;
}

void Cube::doMove(move_t move, bool updateCompact, bool printMove){

    using namespace cube_defs;

    if(printMove) std::cout << get_str_from_move(move) << std::endl;

    bool twice = move & 1;
    bool ccw = move & 2;
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

    corner_t newCorners[4][3];
    edge_t newEdges[4][3];

    for(int ind = 0; ind < 4; ind++){
        newCorners[ind][0] = cMoves[move][ind][0];
        newEdges[ind][0] = eMoves[move][ind][0];

        newCorners[ind][1] = this->corners[cMoves[move][ind][1]];
        newEdges[ind][1] = this->edges[eMoves[move][ind][1]];

        newCorners[ind][2] = (this->cornerOri[cMoves[move][ind][1]] + cMoves[move][ind][2]) % 3;
        newEdges[ind][2] = (this->edgeOri[eMoves[move][ind][1]] + eMoves[move][ind][2]) & 1;
    }
    for(int ind = 0; ind < 4; ind++){
        this->corners[newCorners[ind][0]] = newCorners[ind][1];
        this->edges[newEdges[ind][0]] = newEdges[ind][1];
        this->cornerOri[newCorners[ind][0]] = newCorners[ind][2];
        this->edgeOri[newEdges[ind][0]] = newEdges[ind][2];
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
    for(byte_t ind = 0; ind < 8; ind++){
        if(this->corners[ind] != ind) return false;
        if(this->cornerOri[ind]) return false;
    }
    for(byte_t ind = 0; ind < 12; ind++){
        if(this->edges[ind] != ind) return false;
        if(this->edgeOri[ind]) return false;
    }
    return true;

    //return (this->compact >> 64) == 0xb0110c8531 && ((unsigned long)this->compact == 0xc008864298e84a96);
}

bool Cube::edgesOriented(){
    for(byte_t ori : this->edgeOri){
        if(ori) return false;
    }
    return true;
}

bool Cube::cornersOriented(){
    for(byte_t ori : this->cornerOri){
        if(ori) return false;
    }

    for(edge_t slice = 8; slice < 12; slice++){ //Also need to make sure that the FL, FR, BL, and BR edges are in the correct locations
        if(this->edges[slice] < 8) return false;
    }

    return true;
}

bool Cube::cornersEdgesCorrectOrbit(){

    /*for(corner_t i = 0; i < 8; i++){
        if((i + this->corners[i]) % 2) return false;
    }
    
    for(edge_t i = 0; i < 4; i++){
        if(this->edges[i] >= 4) return false;
    }

    return true;*/

    using namespace cube_defs;

    if(this->corners[kFUL] != kFUL && this->corners[kFUL] != kBUR) return false;
    if(this->corners[kBUR] != kFUL && this->corners[kBUR] != kBUR) return false;
    if(this->corners[kFUR] != kFUR && this->corners[kFUR] != kBUL) return false;
    if(this->corners[kBUL] != kFUR && this->corners[kBUL] != kBUL) return false;
    if(this->corners[kFDL] != kFDL && this->corners[kFDL] != kBDR) return false;
    if(this->corners[kBDR] != kFDL && this->corners[kBDR] != kBDR) return false;
    
    for(edge_t ind = 0; ind < 4; ind++){
        if(this->edges[ind] >= 4) return false;
    }

    byte_t parity = 0;

    for(corner_t i = 0; i < 8; i++){
        for(corner_t j = i + 1; j < 8; j++) parity ^= this->corners[i] < this->corners[j];
    }

    return parity == 0;
}

void Cube::setCompact(){
    this->compact = 0;

    for(int ind = 0; ind < 8; ind++){
        this->compact <<= 3;
        this->compact |= this->corners[ind] & 0b111;
        this->compact <<= 2;
        this->compact |= this->cornerOri[ind] & 0b11;
    }
    for(int ind = 0; ind < 12; ind++){
        this->compact <<= 4;
        this->compact |= this->edges[ind] & 0b1111;
        this->compact <<= 1;
        this->compact |= this->edgeOri[ind] & 1;
    }
}

compact_t Cube::getCompact(){
    return this->compact;
}