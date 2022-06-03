#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
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

void korf_create_maps(){
    std::unordered_map<int, byte_t> edges1Map;
    std::unordered_map<int, byte_t> edges2Map;
    std::unordered_map<int, byte_t> cornersMap;

    std::vector<move_t> moves = get_moves_from_str("F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2");

    std::queue<Cube> cubeQueue;
    Cube cube, newCube;
    int cubeCornerInd, cubeEdge1Ind, cubeEdge2Ind;
    int newCubeCornerInd, newCubeEdge1Ind, newCubeEdge2Ind;
    bool search;

    cubeQueue.push(cube);

    edges1Map.insert(std::pair<int,byte_t>(0,0));
    edges2Map.insert(std::pair<int,byte_t>(0,0));
    cornersMap.insert(std::pair<int,byte_t>(0,0));

    while(cubeQueue.size()){
        cube = cubeQueue.front();

        cubeCornerInd = cube.korfGetCornerInd();
        cubeEdge1Ind = cube.korfGetEdge1Ind();
        cubeEdge2Ind = cube.korfGetEdge2Ind();

        for(move_t move : moves){
            search = false;
            newCube = Cube(cube);
            newCube.doMove(move);

            newCubeCornerInd = newCube.korfGetCornerInd();
            newCubeEdge1Ind = newCube.korfGetEdge1Ind();
            newCubeEdge2Ind = newCube.korfGetEdge2Ind();

            if(!cornersMap.count(newCubeCornerInd)) {
                cornersMap.insert(std::pair<int,byte_t>(newCubeCornerInd,cornersMap[cubeCornerInd]+1));
                search = true;
            }
            if(!edges1Map.count(newCubeEdge1Ind)) {
                edges1Map.insert(std::pair<int,byte_t>(newCubeEdge1Ind,edges1Map[cubeEdge1Ind]+1));
                search = true;
            }
            if(!edges2Map.count(newCubeEdge2Ind)) {
                edges2Map.insert(std::pair<int,byte_t>(newCubeEdge2Ind,edges2Map[cubeEdge2Ind]+1));
                search = true;
            }

            if(cornersMap[newCubeCornerInd] > maxDepth){
                maxDepth++;
                std::cout << maxDepth << std::endl;
            }

            if(search){
                cubeQueue.push(newCube);
            }
        }
        cubeQueue.pop();
    }

    /*for(int maxDepth = 0; maxDepth <= 12; maxDepth++){
        Cube cube;
        std::cout << maxDepth << std::endl;
        if(korf_create_maps_iddfs(cube, cornersMap, edges1Map, edges2Map, moves, 255, 0, maxDepth)) break;
    }*/

    std::ofstream cornerFile;
    std::ofstream edge1File;
    std::ofstream edge2File;
    cornerFile.open("korf_corners", std::ios::out | std::ios::binary);
    edge1File.open("korf_edges_1", std::ios::out | std::ios::binary);
    edge2File.open("kork_edges_2", std::ios::out | std::ios::binary);

    for(size_t ind = 0; ind < cornersMap.size(); ind++){
        cornerFile << cornersMap[ind];
    }
    for(size_t ind = 0; ind < edges1Map.size(); ind++){
        edge1File << edges1Map[ind];
    }
    for(size_t ind = 0; ind < edges2Map.size(); ind++){
        edge2File << edges2Map[ind];
    }
    
    cornerFile.close();
    edge1File.close();
    edge2File.close();

}

bool korf_create_maps_iddfs(Cube &cube, std::unordered_map<int,byte_t>& cornerMap, std::unordered_map<int,byte_t>& edges1Map, std::unordered_map<int,byte_t>& edges2Map, std::vector<move_t> &moves, move_t prevMove, int depth, int &maxDepth){
    
    bool finished = true;

    if(depth >= maxDepth){
        int cornerVal = cube.korfGetCornerInd();
        int edge1Val = cube.korfGetEdge1Ind();
        int edge2Val = cube.korfGetEdge2Ind();
        
        if(!cornerMap.count(cornerVal)) {
            cornerMap.insert(std::pair<int,byte_t>(cornerVal,depth));
            finished = false;
        }
        if(!edges1Map.count(edge1Val)) {
            edges1Map.insert(std::pair<int,byte_t>(edge1Val,depth));
            finished = false;
        }
        if(!edges2Map.count(edge2Val)) {
            edges2Map.insert(std::pair<int,byte_t>(edge2Val,depth));
            finished = false;
        }

        std::cout << cornerMap.size() << '\t' << edges1Map.size() << '\t' << edges2Map.size() << std::endl;

    } else {
        for(move_t move : moves){
            if(!thistlewaite_solve_pruner(move,prevMove)){
                cube.doMove(move,false);
                if(!korf_create_maps_iddfs(cube, cornerMap, edges1Map, edges2Map, moves, move, depth + 1, maxDepth) && finished) finished = false;
                cube.doMove(get_move_opposite(move),false);
            }
            
        }
        
    }

    return finished;

    
}
