#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <chrono>

#include "solve.hpp"

std::vector<move_t> solve_thistlethwaite(Cube cube){
    std::vector<move_t> moves;
    std::vector<move_t> finalMoves;
    std::vector<move_t> searchMoves;
    std::unordered_map<compact_t, bool> goalMap;
    //std::string searchMovesStr = "F F\' B B\' L L\' R R\' U U\' D D\' F2 B2 U2 D2 L2 R2";

    //TODO: Determine which of these two search move strings is better
    //std::array<std::string,4> searchMoveStrs = {"F F\' B B\' L L\' R R\' U U\' D D\'","F2 B2 L L\' R R\' U U\' D D\'","F2 B2 L2 R2 U U\' D D\'","F2 B2 L2 R2 U2 D2"};
    std::array<std::string,4> searchMoveStrs = {"F B L R U D","F2 B2 L R U D","F2 B2 L2 R2 U D","F2 B2 L2 R2 U2 D2"};
    std::array<bool (Cube::*)(),4> completeFuncs = {&Cube::edgesOriented, &Cube::cornersOriented, &Cube::cornersEdgesCorrectOrbit, &Cube::isComplete};
    int maxDepth;
    int64_t timeElapsed;

    for(int n = 0; n < 4; n++){

        searchMoves = get_moves_from_str(searchMoveStrs[n]);

        moves.clear();
        goalMap.clear();
        //searchMoves = get_moves_from_str(searchMovesStr);
        maxDepth = 0;

        auto msStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        while(!solve_thistlethwaite_iddfs(cube, completeFuncs[n], searchMoves, moves, goalMap, 0, maxDepth)) {
            maxDepth++;
            std::cout << "Increasing maximum depth to " << maxDepth << std::endl;
        }
        auto msEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        timeElapsed += msEnd - msStart;

        std::cout << "Time elapsed (ms): " << msEnd-msStart << std::endl;
        std::cout << "Maximum depth: " << maxDepth << std::endl;

        cube.doMoves(moves);
        cube.printCube();
        cube.printOrientations();
        //searchMovesStr = searchMovesStr.substr(10,searchMovesStr.size()-10);

        for(move_t move : moves){
            finalMoves.push_back(move);
        }
    }

    std::cout << "Solved in " << timeElapsed << " ms" << std::endl;

    return finalMoves;
}

bool solve_thistlethwaite_iddfs(Cube cube, bool (Cube::*completeFunc)(), std::vector<move_t> &searchMoves, std::vector<move_t> &moveList, std::unordered_map<compact_t,bool> &goalMap, int depth, int &maxDepth){
    if(depth >= maxDepth) {
        cube.setCompact();
        if(!goalMap.count(cube.getCompact())) goalMap.insert(std::pair<compact_t,bool>(cube.getCompact(),(cube.*completeFunc)()));
        return goalMap[cube.getCompact()];
    }

    //for(move_t move : moveList) std::cout << get_str_from_move(move) << ' ';
    //std::cout << std::endl;

    for(move_t move : searchMoves){
        if(!moveList.size() || !solve_thistlethwaite_pruner(move,moveList.back())){

            moveList.push_back(move);

            Cube newCube(cube);
            newCube.doMove(move);

            if(solve_thistlethwaite_iddfs(newCube,completeFunc,searchMoves,moveList,goalMap,depth+1,maxDepth)) {
                return true;
            }
            else moveList.pop_back();
        }
    }
    return false;
}
bool solve_thistlethwaite_pruner(move_t move, move_t prevMove){

    move >>= 2;
    prevMove >>= 2;

    if(move == prevMove) return true; //Redundant move (F F = F2, F' = F2 F, etc.)

    //Commutative move (F B ~ B F, L R ~ R L, U D ~ D U, etc.)
    if(move == cube_defs::kF && prevMove == cube_defs::kB) return true;
    //else if(move == cube_defs::kB && prevMove == cube_defs::kF) return true;
    if(move == cube_defs::kL && prevMove == cube_defs::kR) return true;
    //else if(move == cube_defs::kR && prevMove == cube_defs::kL) return true;
    if(move == cube_defs::kU && prevMove == cube_defs::kD) return true;
    //else if(move == cube_defs::kD && prevMove == cube_defs::kU) return true;
    //std::cout << (int) move << '\t' << (int)prevMove << std::endl;

    return false;
}