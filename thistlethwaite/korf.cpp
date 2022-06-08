#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <chrono>

#include "solve.hpp"

std::unordered_map<int,byte_t> gCornersMap;
std::unordered_map<int,byte_t> gEdges1Map;
std::unordered_map<int, byte_t> gEdges2Map;

std::ifstream gCornerFS;
std::ifstream gEdges1FS;
std::ifstream gEdges2FS;

struct KorfPriority {
    byte_t max;
    move_t move;
};
struct KorfPriorityCompare {
    bool operator()(const KorfPriority &lhs, const KorfPriority &rhs) const{
        return lhs.max < rhs.max;
    }
};

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
        toWrite = (moveMap.count(ind) ? moveMap[ind] & 0b1111: 0);
        toWrite <<= 4;
        toWrite |= (moveMap.count(ind+1) ? moveMap[ind+1] & 0b1111: 0);
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

byte_t korf_read_map_fs(std::ifstream &fs, int ind){
    fs.seekg(ind >> 1, std::ios::beg);
    byte_t buff;
    fs.read((char*)&buff,1);
    if(ind % 2) return buff & 0b1111;
    else return buff >> 4;
}

std::vector<move_t> korf_solve(Cube cube, bool printMoves){
    //gCornersMap = korf_read_map("korf_corners");
    //std::cout << "Corners read" << std::endl;
    //gEdges1Map = korf_read_map("korf_edges_1");
    //std::cout << "Edges 1 read" << std::endl;
    //gEdges2Map = korf_read_map("korf_edges_2");
    //std::cout << "Edges 2 read" << std::endl;
    gCornerFS.open("korf_corners", std::ios::binary | std::ios::in);
    gEdges1FS.open("korf_edges_1", std::ios::binary | std::ios::in);
    gEdges2FS.open("korf_edges_2", std::ios::binary | std::ios::in);

    std::vector<move_t> moves;
    std::vector<move_t> searchMoves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    //int bound = std::max(std::max(gEdges1Map[cube.korfGetEdge1Ind()],gEdges2Map[cube.korfGetEdge2Ind()]),gCornersMap[cube.korfGetCornerInd()]);
    int bound = std::max(std::max(korf_read_map_fs(gEdges1FS,cube.korfGetEdge1Ind()),korf_read_map_fs(gEdges2FS,cube.korfGetEdge2Ind())),korf_read_map_fs(gCornerFS,cube.korfGetCornerInd()));
    int nextBound = 256;

    std::cout << "Starting bound of " << bound << std::endl;
    
    auto msStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    while(!korf_solve_ida(cube, moves, searchMoves, 0, bound, nextBound)){
        bound = nextBound;
        nextBound = 256;
        std::cout << "Bound increased to " << bound << std::endl;
    }

    auto msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Solve time of " << (msEnd - msStart) << " ms" << std::endl;
    std::cout << "Average time per move of " << (msEnd - msStart) << " ms" << std::endl;
    if(printMoves){
        for(move_t move : moves) std::cout << get_str_from_move(move) << ' ';
        std::cout << std::endl;
    }

    gCornerFS.close();
    gEdges1FS.close();
    gEdges2FS.close();

    return moves;
}

bool korf_solve_ida(Cube &cube, std::vector<move_t> &moves, std::vector<move_t> &searchMoves, int depth, int &bound, int &nextBound){
    if(depth == bound) return cube.isComplete();

    //std::cout << depth << '\t' << bound << std::endl;
    std::priority_queue<KorfPriority, std::vector<KorfPriority>, KorfPriorityCompare> pQueue;

    for(move_t move : searchMoves){
        if(!depth || !solve_pruner(move, moves.back())){

            cube.doMove(move,false);

            KorfPriority p;
            p.move = move;
            //p.max = std::max(std::max(gEdges1Map[cube.korfGetEdge1Ind()],gEdges2Map[cube.korfGetEdge2Ind()]),gCornersMap[cube.korfGetCornerInd()]) + depth + 1;
            p.max = std::max(std::max(korf_read_map_fs(gEdges1FS,cube.korfGetEdge1Ind()),korf_read_map_fs(gEdges2FS,cube.korfGetEdge2Ind())),korf_read_map_fs(gCornerFS,cube.korfGetCornerInd())) + depth + 1;

            cube.doMove(get_move_opposite(move),false);
            
            if(p.max <= bound) pQueue.push(p);
            else if(p.max < nextBound) nextBound = p.max;
        }
    }


    while(!pQueue.empty()){
        cube.doMove(pQueue.top().move, false);
        moves.push_back(pQueue.top().move);

        if(korf_solve_ida(cube, moves, searchMoves, depth+1, bound, nextBound)) return true;
        else{
            cube.doMove(get_move_opposite(pQueue.top().move), false);
            moves.pop_back();
            pQueue.pop();
        }
    }

    return false;
}