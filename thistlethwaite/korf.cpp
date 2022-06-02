#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>

#include "solve.hpp"

/*std::unordered_map<compact_t,byte_t> generate_map_korf(){
    std::unordered_map<compact_t,byte_t> movesMap;
    std::vector<Cube> cubeVec;
    std::vector<move_t> moves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    Cube currentCube;
    Cube newCube;

    movesMap.insert(std::pair<compact_t,byte_t>(currentCube.getCompact(),0));
    cubeVec.push_back(currentCube);

    while(cubeVec.size()){

        currentCube = cubeVec.back();
        cubeVec.pop_back();

        for(move_t move : moves) {
            newCube = currentCube;
            newCube.doMove(move);
            if(!movesMap.count(newCube.getCompact()) || movesMap[currentCube.getCompact()] + 1 < movesMap[newCube.getCompact()]){
                movesMap[newCube.getCompact()] = movesMap[currentCube.getCompact()] + 1;
                cubeVec.push_back(newCube);
            }
        }
        
    }

    return movesMap;
}
void write_map_korf(std::string filename, std::unordered_map<compact_t,byte_t> movesMap){
    std::ofstream fs;
    fs.open(filename, std::ios::out | std::ios::binary);

    unsigned long toWrite;

    for(std::pair<compact_t,byte_t> movePair : movesMap){
        fs.write((char*)&movePair.first,16);
        fs.write((char*)&movePair.second,1);
    }

    fs.close();
}
std::unordered_map<compact_t,byte_t> read_map_korf(std::string filename){
    std::unordered_map<compact_t, byte_t> movesMap;
    std::pair<compact_t, byte_t> movePair;

    std::ifstream fs;
    fs.open(filename, std::ios::in | std::ios::binary);

    while(!fs.eof()){
        fs.read((char*)&movePair.first,16);
        fs.read((char*)&movePair.second,1);
        movesMap.insert(movePair);
    }

    fs.close();

    return movesMap;
}
std::vector<move_t> solve_korf(Cube cube);*/