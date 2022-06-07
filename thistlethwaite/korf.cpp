#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

#include "solve.hpp"

std::unordered_map<int,byte_t> gCornersMap;
std::unordered_map<int,byte_t> gEdges1Map;
std::unordered_map<int, byte_t> gEdges2Map;

struct KorfPriority {
    byte_t max;
    move_t move;
};
struct KorfPriorityCompare {
    bool operator()(const KorfPriority &lhs, const KorfPriority &rhs) const{
        return lhs.max < rhs.max;
    }
};

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
    gCornersMap = korf_read_map("korf_corners");
    std::cout << "Corners read" << std::endl;
    gEdges1Map = korf_read_map("korf_edges_1");
    std::cout << "Edges 1 read" << std::endl;
    gEdges2Map = korf_read_map("korf_edges_2");
    std::cout << "Edges 2 read" << std::endl;

    std::vector<move_t> moves;
    std::vector<move_t> searchMoves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    //TODO: write A* search here
    korf_solve_ida(cube, moves, searchMoves, 0, std::max(std::max(gEdges1Map[cube.korfGetEdge1Ind()],gEdges2Map[cube.korfGetEdge2Ind()]),gCornersMap[cube.korfGetCornerInd()]));

    return moves;

}

bool korf_solve_ida(Cube &cube, std::vector<move_t> &moves, std::vector<move_t> &searchMoves, int depth, int bound){
    if(depth == bound) return cube.isComplete();

    std::priority_queue<KorfPriority, std::vector<KorfPriority>, KorfPriorityCompare> pQueue;

    for(move_t move : moves){
        if(moves.empty() || !solve_pruner(move, moves.back())){

            cube.doMove(move,false);

            KorfPriority p;
            p.move = move;
            p.max = std::max(std::max(gEdges1Map[cube.korfGetEdge1Ind()],gEdges2Map[cube.korfGetEdge2Ind()]),gCornersMap[cube.korfGetCornerInd()]);
            cube.doMove(get_move_opposite(move),false);
            
            if(p.max + depth + 1 <= bound) pQueue.push(p);
        }
    }


    while(!pQueue.empty()){
        cube.doMove(pQueue.top().move, false);
        moves.push_back(pQueue.top().move);

        if(korf_solve_ida(cube, moves, searchMoves, depth+1, pQueue.top().max + depth + 1 < bound ? pQueue.top().max + depth + 1 : bound)) return true;
        else{
            cube.doMove(get_move_opposite(pQueue.top().move), false);
            moves.pop_back();
            pQueue.pop();
        }
    }

    return false;
}