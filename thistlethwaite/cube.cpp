#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <unordered_set>
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

    /*bool swapInds = ((int)corner + ind) % 2;
    if(swapInds) std::cout << "swap" << std::endl;

    //Just took this from the medium article, would've been annoying to code otherwise
    //Also, thanks for saying that the cubie naming was "arbitrary"

    if(ori == 0){
        inds[kDirY] = kDirY;

        if(swapInds){
            inds[kDirX] = kDirZ;
            inds[kDirZ] = kDirX;
        } else {
            inds[kDirX] = kDirX;
            inds[kDirZ] = kDirZ;
        }
    }
    else if(ori == 1){
        inds[kDirZ] = kDirY;

        if(swapInds){
            inds[kDirX] = kDirX;
            inds[kDirY] = kDirZ;
        } else {
            inds[kDirX] = kDirZ;
            inds[kDirY] = kDirX;
        }
    }
    else{ //ori == 2
        std::cout << "test" << std::endl;
        inds[kDirX] = kDirY;

        if(swapInds){
            inds[kDirY] = kDirX;
            inds[kDirZ] = kDirZ;
        } else {
            inds[kDirY] = kDirZ;
            inds[kDirZ] = kDirX;
        }
    }

    std::cout << inds[kDirX] << '\t' << inds[kDirY] << '\t' << inds[kDirZ] << std::endl;

    switch(corner){
        case kFUL:
        faces[inds[kDirX]] = kL;
        faces[inds[kDirY]] = kU;
        faces[inds[kDirZ]] = kF;
        break;
        case kBUL:
        faces[inds[kDirX]] = kL;
        faces[inds[kDirY]] = kU;
        faces[inds[kDirZ]] = kB;
        break;
        case kBUR:
        faces[inds[kDirX]] = kR;
        faces[inds[kDirY]] = kU;
        faces[inds[kDirZ]] = kB;
        break;
        case kFUR:
        faces[inds[kDirX]] = kR;
        faces[inds[kDirY]] = kU;
        faces[inds[kDirZ]] = kF;
        break;
        case kFDR:
        faces[inds[kDirX]] = kR;
        faces[inds[kDirY]] = kD;
        faces[inds[kDirZ]] = kF;
        break;
        case kFDL:
        faces[inds[kDirX]] = kL;
        faces[inds[kDirY]] = kD;
        faces[inds[kDirZ]] = kF;
        break;
        case kBDL:
        faces[inds[kDirX]] = kL;
        faces[inds[kDirY]] = kD;
        faces[inds[kDirZ]] = kB;
        break;
        case kBDR:
        faces[inds[kDirX]] = kR;
        faces[inds[kDirY]] = kD;
        faces[inds[kDirZ]] = kB;
        break;
    }

    std::cout << get_face_char(faces[inds[kDirX]]) << '\t' << get_face_char(faces[inds[kDirY]]) << '\t' << get_face_char(faces[inds[kDirZ]]) << std::endl;*/

    //for(int dir = 0; dir < 3; dir++) faces[dir] = inds[dir] * 2 + ((corner >> inds[dir]) & 1);

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

std::unordered_map<compact_t,compact_t> generate_move_map(){
    std::unordered_map<compact_t,compact_t> moveMap;

    std::queue<compact_t> cubeQueue;

    compact_t cubeMoveToAdd;
    Cube currentCube;

    std::vector<move_t> moves = get_moves_from_str("F F\' F2 B B\' B2 U U\' U2 D D\' D2 L L\' L2 R R\' R2");

    cubeQueue.push(currentCube.getCompact());

    while(cubeQueue.size()){

        std::cout << cubeQueue.size() << std::endl;
        
        for(move_t move : moves){
            cubeMoveToAdd = cubeQueue.front();
            cubeMoveToAdd <<= 8;
            cubeMoveToAdd |= move;
            if(!moveMap.count(cubeMoveToAdd)){
                currentCube = Cube(cubeQueue.front());
                currentCube.doMove(move);
                moveMap.insert(std::pair<compact_t,compact_t>(cubeMoveToAdd,currentCube.getCompact()));
                cubeQueue.push(currentCube.getCompact());
            }
        }

        cubeQueue.pop();
    }

    return moveMap;
}

void write_move_map(std::string filename, std::unordered_map<compact_t,compact_t> moveMap){
    std::ofstream fs;
    fs.open(filename, std::ios::out | std::ios::binary);

    uint64_t toWrite;

    for(std::pair<compact_t,compact_t> movePair : moveMap){
        toWrite = movePair.first >> 64;
        fs.write((char*)&toWrite,sizeof(toWrite));
        toWrite = movePair.first & 0xFFFFFFFFFFFFFFFF;
        fs.write((char*)&toWrite,sizeof(toWrite));

        toWrite = movePair.second >> 64;
        fs.write((char*)&toWrite,sizeof(toWrite));
        toWrite = movePair.second & 0xFFFFFFFFFFFFFFFF;
        fs.write((char*)&toWrite,sizeof(toWrite));
    }

    fs.close();
}

std::unordered_map<compact_t,compact_t> read_move_map(std::string filename){
    std::ifstream fs;
    fs.open(filename, std::ios::in | std::ios::binary);

    std::unordered_map<compact_t,compact_t> moveMap;

    uint64_t toRead;
    compact_t key;
    compact_t val;

    while(!fs.eof()){
        fs.read((char *)&toRead,sizeof(toRead));
        key = toRead;
        key <<= 64;
        fs.read((char *)&toRead,sizeof(toRead));
        key |= toRead;

        fs.read((char *)&toRead,sizeof(toRead));
        val = toRead;
        val <<= 64;
        fs.read((char *)&toRead,sizeof(toRead));
        val |= toRead;

        moveMap.insert(std::pair<compact_t,compact_t>(key,val));
    }

    fs.close();

    return moveMap;
}