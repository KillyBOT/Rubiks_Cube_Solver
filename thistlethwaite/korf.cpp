#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

#include "solve.hpp"

unsigned long statesVisited;

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

void korf_create_maps(){
    std::unordered_map<int, byte_t> edges1Map;
    std::unordered_map<int, byte_t> edges2Map;
    std::unordered_map<int, byte_t> cornersMap;

    std::vector<move_t> moves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    std::queue<Cube> cubeQueue;
    Cube cube, newCube;
    int cornerInd, edge1Ind, edge2Ind;
    size_t size;

    cubeQueue.push(cube);

    edges1Map.insert(std::pair<int,byte_t>(0,0));
    edges2Map.insert(std::pair<int,byte_t>(0,0));
    cornersMap.insert(std::pair<int,byte_t>(0,0));

    for(byte_t depth = 0; depth < 12; depth++){
        size = cubeQueue.size();
        //std::cout << (int)depth << '\t' << size << std::endl;
        for(size_t i = 0; i < size; i++){

            std::cout << (int)depth << '\t' << cornersMap.size() << '\t' << edges1Map.size() << '\t' << edges2Map.size() << std::endl;

            cube = cubeQueue.front();
            cubeQueue.pop();

            cornerInd = cube.korfGetCornerInd();
            edge1Ind = cube.korfGetEdge1Ind();
            edge2Ind = cube.korfGetEdge2Ind();

            if(!cornersMap.count(cornerInd)) cornersMap.insert(std::pair<int,byte_t>(cornerInd,depth));
            if(!edges1Map.count(edge1Ind)) edges1Map.insert(std::pair<int,byte_t>(edge1Ind,depth));
            if(!edges2Map.count(edge2Ind)) edges2Map.insert(std::pair<int,byte_t>(edge2Ind,depth));

            if(depth < 11){
                for(move_t move : moves){
                    newCube = Cube(cube);
                    newCube.doMove(move);

                    cubeQueue.push(newCube);                    
                }
            }
        }
    }

    korf_write_map("korf_corners",cornersMap);
    korf_write_map("korf_edges_1",edges1Map);
    korf_write_map("korf_edges_2",edges2Map);

}

void korf_create_map(std::string filename, int(Cube::*indFunc)(), byte_t maxDepth){
    std::unordered_map<int,byte_t> moveMap;
    std::vector<move_t> moves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    statesVisited = 0;

    Cube cube;
    korf_create_map_dfs(cube, indFunc, moveMap, moves, 255, 0, maxDepth);
    std::cout << moveMap.size() << std::endl;

    korf_write_map(filename,moveMap);
}

void korf_create_map_dfs(Cube &cube, int (Cube::*indFunc)(), std::unordered_map<int,byte_t>& moveMap, std::vector<move_t> &moves, move_t prevMove, byte_t depth, byte_t &maxDepth){

    int ind = (cube.*indFunc)();
    bool doSearch = false;

    if(!moveMap.count(ind)) {
        moveMap.insert(std::pair<int,byte_t>(ind,depth));
        std::cout << moveMap.size() << std::endl;
        doSearch = true;
    } else if (depth < moveMap[ind]){
        moveMap[ind] = depth;
        doSearch = true;
    }

    if(depth < maxDepth && doSearch){
        for(move_t move : moves){
            if(!solve_pruner(move, prevMove)){
                cube.doMove(move,false);
                korf_create_map_dfs(cube, indFunc, moveMap, moves, move, depth + 1, maxDepth);
                cube.doMove(get_move_opposite(move),false);
            }
        }
    }
}

void korf_write_map(std::string filename, std::unordered_map<int,byte_t> &moveMap){
    std::ofstream fs;
    fs.open(filename, std::ios::out | std::ios::binary);

    byte_t toWrite;
    for(size_t ind = 0; ind < moveMap.size(); ind += 2){
        toWrite = (moveMap.count(ind) ? moveMap[ind] : 0);
        toWrite <<= 4;
        toWrite |= (moveMap.count(ind+1) ? moveMap[ind+1] : 0);
        fs.write((char*)&toWrite,1);
    }
    if(moveMap.size() % 2){
        toWrite = moveMap[moveMap.size() - 1] << 4;
        fs.write((char*)&toWrite,1);
    }

    fs.close();
}

std::unordered_map<int, byte_t> korf_read_map(std::string filename){
    std::unordered_map<int, byte_t> moveMap;

    std::ifstream fs;
    fs.open(filename, std::ios::in | std::ios::binary );

    int ind = 0;
    byte_t minMoves;
    while(!fs.eof()){
        fs.read((char*)&minMoves,1);
        moveMap.insert(std::pair<int,byte_t>(ind,minMoves >> 4));
        moveMap.insert(std::pair<int,byte_t>(ind+1,minMoves & 0b1111));
        ind+=2;
    }

    fs.close();

    return moveMap;
}

std::vector<move_t> korf_solve(Cube cube){
    std::unordered_map<int,byte_t> cornersMap = korf_read_map("korf_corners");
    std::cout << "Corners read" << std::endl;
    std::unordered_map<int,byte_t> edges1Map = korf_read_map("korf_edges_1");
    std::cout << "Edges 1 read" << std::endl;
    std::unordered_map<int,byte_t> edges2Map = korf_read_map("korf_edges_2");
    std::cout << "Edges 2 read" << std::endl;

    std::vector<move_t> moves;
    std::vector<move_t> searchMoves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    int maxDepth = 0;

    while(!korf_solve_ida(cube, moves, searchMoves, cornersMap, edges1Map, edges2Map, 0, maxDepth)){
        maxDepth++;
        std::cout << "Increasing max depth to " << maxDepth << std::endl;
    }

    return moves;

}

bool korf_solve_ida(Cube &cube, std::vector<move_t> &moves, std::vector<move_t> &searchMoves, std::unordered_map<int,byte_t> &cornersMap, std::unordered_map<int,byte_t> &edges1Map, std::unordered_map<int,byte_t> &edges2Map, int depth, int &maxDepth){

    if(depth == maxDepth){
        return cube.isComplete();
    }

    std::vector<std::pair<byte_t,move_t>> moveList;
    byte_t cornerVal, edge1Val, edge2Val;

    for(move_t move : searchMoves){
        if(!moves.size() || !solve_pruner(move, moves.back())){

            cube.doMove(move,false);

            cornerVal = cornersMap[cube.korfGetCornerInd()];
            edge1Val = edges1Map[cube.korfGetEdge1Ind()];
            edge2Val = edges2Map[cube.korfGetEdge2Ind()];

            moveList.push_back(std::pair<byte_t,move_t>(std::max(std::max(edge1Val,edge2Val),cornerVal),move));

            if(moveList.back().first < moves.size()) moveList.pop_back();

            cube.doMove(get_move_opposite(move),false);
        }
    }

    while(!moveList.empty()){
        //Find minimum move
        size_t minInd = 0;
        for(size_t ind = 1; ind < moveList.size(); ind++){
            if(moveList[ind].first < moveList[minInd].first) minInd = ind;
        }

        //Do that move, if it's not correct then remove it from the moves to do and repeat
        cube.doMove(moveList[minInd].second,false);
        moves.push_back(moveList[minInd].second);
        if(korf_solve_ida(cube, moves, searchMoves, cornersMap, edges1Map, edges2Map, depth+1, maxDepth)) return true;
        else{
            cube.doMove(get_move_opposite(moveList[minInd].second),false);
            moves.pop_back();
            moveList.erase(moveList.begin() + minInd);
        }
    }

    return false;

}