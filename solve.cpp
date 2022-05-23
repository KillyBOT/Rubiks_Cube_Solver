#include <iostream>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>

#include "cube.hpp"
#include "solve.hpp"

//Taken from stackoverflow from user named Thomas Mueller
size_t cubieHash::operator()(const std::vector<cubie_t> &cubies) const {
    size_t seed = cubies.size();
    for(cubie_t cubie : cubies){
        cubie = ((cubie >> 16) ^ cubie) * 0x45d9f3b;
        cubie = ((cubie >> 16) ^ cubie) * 0x45d9f3b;
        cubie = (cubie >> 16) ^ cubie;
        seed ^= cubie + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
}

bool compare_cube(Cube *c1, Cube *c2){
    return c1->getScore() > c2->getScore();
}
int heuristic_basic(Cube cube){

    /*int score = cube.getCubeSize() * cube.getCubeSize() * cube.getCubeSize();
    cubie_t testCubie = newCubie();

    for(cubie_t cubie : cube.getCubies()){
        if(cubie == testCubie) score--;
    }

    return score + cube.getMoves().size();*/

    int score = 6 * cube.getCubeSize() * cube.getCubeSize();

    for(byte_t face = 0; face < 6; face++){
        for(int row = 0; row < cube.getCubeSize(); row++){
            for(int col = 0; col < cube.getCubeSize(); col++){
                if(face == cube.getFaceCol(face, row, col)) score--;
            }
        }
    }

    return score + cube.getMoves().size() * 4 * cube.getCubeSize();
}

int heuristic_orientation(Cube cube){
    
    int movesLeft = 0;
    col_t c;

    for(int row = 0; row < cube.cubeSize; row++){
        for(int col = 0; col < cube.cubeSize; col++){
            c = cube.getFaceCol(FACE_FRONT,row,col);
            if(c > COL_ORANGE) movesLeft++;
            c = cube.getFaceCol(FACE_BACK,row,col);
            if(c > COL_ORANGE) movesLeft++;
        }
    }

    for(int depth = 1; depth < cube.cubeSize - 1; depth++){
        for(int ind : cube.rings[depth][0]){
            c = getCubieFace(cube.cubies[ind],FACE_FRONT);
            if(c > COL_ORANGE) movesLeft++;
        }
    }

    return movesLeft + cube.getMoves().size();
}

Cube solve_bfs(Cube cube){
    std::queue<Cube*> cubeQueue;

    std::set<std::vector<cubie_t>> cubieSet;
    Cube *currentCube;
    bool search;

    cubeQueue.push(new Cube(cube));

    while(cubeQueue.size()){

        currentCube = cubeQueue.front();
        cubeQueue.pop();

        search = !cubieSet.count(currentCube->getCubies());

        if(search){

            //currentCube->printCube();

            if(currentCube->isComplete()){

                Cube retCube(*currentCube);
                delete currentCube;

                while(cubeQueue.size()){
                    currentCube = cubeQueue.front();
                    cubeQueue.pop();
                    delete currentCube;
                }

                return retCube;
            }
            cubieSet.insert(currentCube->getCubies());

            for(int dir = 0; dir < 3; dir++){
                for(int depth = 0; depth < currentCube->getCubeSize(); depth++){
                    for(int ccw = 0; ccw < 2; ccw++){
                        Cube *newCube = new Cube(*currentCube);
                        newCube->doMove(Move(dir, depth, ccw));
                        cubeQueue.push(newCube);

                        //for(Cube c : cubeHeap) c.printCube();
                    }
                }
            }
        }

        delete currentCube;

    }

    std::cout << "You should never see this" << std::endl;
    return cube;

}

Cube solve_astar(Cube cube){

    std::vector<Cube*> cubeHeap;

    std::unordered_set<std::vector<cubie_t>,cubieHash> cubeSet;
    std::unordered_set<std::vector<cubie_t>,cubieHash> gScoresSet;
    std::unordered_map<std::vector<cubie_t>,size_t,cubieHash> gScores;
    
    Cube *currentCube;
    Move lastMove;

    cubeSet.insert(cube.getCubies());
    gScoresSet.insert(cube.getCubies());
    gScores[cube.getCubies()] = 0;

    cubeHeap.push_back(new Cube(cube));

    while(cubeHeap.size()){

        currentCube = cubeHeap.front();

        std::pop_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);
        cubeHeap.pop_back();

        if(currentCube->isComplete()){
            for(unsigned long i = 0; i < cubeHeap.size(); i++){
                delete cubeHeap[i];
            }
            Cube retCube(*currentCube);
            delete currentCube;
            return retCube;
        }

        cubeSet.erase(currentCube->getCubies());

        if(currentCube->getMoves().size()) lastMove = currentCube->getMoves().back();

        for(int dir = 0; dir < 3; dir++){
            for(int depth = 0; depth < currentCube->getCubeSize(); depth++){

                if(!currentCube->getMoves().size() || (dir != lastMove.dir || depth != lastMove.depth)){

                    for(int ccw = 0; ccw < 3; ccw++){

                        Cube *newCube = new Cube(*currentCube);
                        newCube->doMove(Move(dir, depth, (ccw / 2) + 1, ccw % 2));

                        if(!gScoresSet.count(newCube->getCubies()) || newCube->getMoves().size() < gScores[newCube->getCubies()]){
                            gScoresSet.insert(newCube->getCubies());
                            gScores.insert(std::pair<std::vector<cubie_t>,size_t>(newCube->getCubies(),newCube->getMoves().size()));

                            if(!cubeSet.count(newCube->getCubies())){
                                cubeSet.insert(newCube->getCubies());

                                cubeHeap.push_back(newCube);
                                std::push_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);

                            } else {
                                delete newCube;
                            }
                        } else {
                            delete newCube;
                        }
                        
                        //for(Cube *c : cubeHeap) c->printCube();
                    }
                }
            }
        }

        delete currentCube;

    }

    std::cout << "You should never see this" << std::endl;
    return cube;

}

Cube orient_astar(Cube cube){

    cube.setHeuristicFunc(heuristic_orientation);

    std::vector<Cube*> cubeHeap;

    std::unordered_set<std::vector<cubie_t>,cubieHash> cubeSet;
    std::unordered_set<std::vector<cubie_t>,cubieHash> gScoresSet;
    std::unordered_map<std::vector<cubie_t>,size_t,cubieHash> gScores;
    
    Cube *currentCube;
    Move lastMove;

    cubeSet.insert(cube.getCubies());
    gScoresSet.insert(cube.getCubies());
    gScores[cube.getCubies()] = 0;

    cubeHeap.push_back(new Cube(cube));

    while(cubeHeap.size()){

        currentCube = cubeHeap.front();

        std::pop_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);
        cubeHeap.pop_back();

        if(currentCube->isOriented()){
            for(unsigned long i = 0; i < cubeHeap.size(); i++){
                delete cubeHeap[i];
            }
            Cube retCube(*currentCube);
            delete currentCube;
            return retCube;
        }

        cubeSet.erase(currentCube->getCubies());

        if(currentCube->getMoves().size()) lastMove = currentCube->getMoves().back();

        for(int dir = 0; dir < 3; dir++){
            for(int depth = 0; depth < currentCube->getCubeSize(); depth++){
                if(!currentCube->getMoves().size() || dir != lastMove.dir || depth != lastMove.depth){
                    for(int ccw = 0; ccw < 2; ccw++){

                        Cube *newCube = new Cube(*currentCube);
                        newCube->doMove(Move(dir, depth, ccw % 2));

                        if(!gScoresSet.count(newCube->getCubies()) || newCube->getMoves().size() < gScores[newCube->getCubies()]){
                            gScoresSet.insert(newCube->getCubies());
                            gScores.insert(std::pair<std::vector<cubie_t>,size_t>(newCube->getCubies(),newCube->getMoves().size()));

                            if(!cubeSet.count(newCube->getCubies())){
                                cubeSet.insert(newCube->getCubies());

                                cubeHeap.push_back(newCube);
                                std::push_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);

                            } else {
                                delete newCube;
                            }
                        } else {
                            delete newCube;
                        }
                        
                        //for(Cube *c : cubeHeap) c->printCube();
                    }
                }
            }
        }

        delete currentCube;

    }

    std::cout << "You should never see this" << std::endl;
    return cube;

}

Cube solve_astar_3(Cube cube){

    cube.fixOrientation(); 
    cube.printCube();

    std::vector<Move> moves = getMovesFromStr("F F2 F\' B B2 B\' U U2 U\' D D2 D\' L L2 L\' R R2 R\'");

    std::vector<Cube*> cubeHeap;

    std::unordered_set<std::vector<cubie_t>,cubieHash> cubeSet;
    std::unordered_set<std::vector<cubie_t>,cubieHash> gScoresSet;
    std::unordered_map<std::vector<cubie_t>,size_t,cubieHash> gScores;

    Cube *currentCube;
    Move lastMove;

    cubeSet.insert(cube.getCubies());
    gScoresSet.insert(cube.getCubies());
    gScores[cube.getCubies()] = 0;

    cubeHeap.push_back(new Cube(cube));

    while(cubeHeap.size()){

        currentCube = cubeHeap.front();

        std::pop_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);
        cubeHeap.pop_back();

        if(currentCube->isComplete()){
            for(unsigned long i = 0; i < cubeHeap.size(); i++){
                delete cubeHeap[i];
            }
            Cube retCube(*currentCube);
            delete currentCube;
            return retCube;
        }

        cubeSet.erase(currentCube->getCubies());

        if(currentCube->getMoves().size()) lastMove = currentCube->getMoves().back();

        for(Move move: moves) {

            if(!currentCube->getMoves().size() || move.dir != lastMove.dir || move.depth != lastMove.depth){
                Cube *newCube = new Cube(*currentCube);
                newCube->doMove(move);

                if(!gScoresSet.count(newCube->getCubies()) || newCube->getMoves().size() < gScores[newCube->getCubies()]){
                    gScoresSet.insert(newCube->getCubies());
                    gScores.insert(std::pair<std::vector<cubie_t>,size_t>(newCube->getCubies(),newCube->getMoves().size()));

                    if(!cubeSet.count(newCube->getCubies())){
                        cubeSet.insert(newCube->getCubies());

                        cubeHeap.push_back(newCube);
                        std::push_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);

                    } else {
                        delete newCube;
                    }
                } else {
                    delete newCube;
                }


            //for(Cube *c : cubeHeap) c->printCube();
            }
        }

        delete currentCube;

    }

    std::cout << "You should never see this" << std::endl;
    return cube;

}

std::vector<Move> getReverseMoves(std::vector<Move> moves){
    std::vector<Move> revMoves;

    while(moves.size()){
        revMoves.push_back(moves.back());
        moves.pop_back();
    }
    for(size_t moveInd = 0; moveInd < revMoves.size(); moveInd++){
        revMoves[moveInd].ccw = !revMoves[moveInd].ccw;
    }

    return revMoves;
}