#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>

#include "cube.hpp"
#include "solve.hpp"

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

    std::map<std::vector<cubie_t>,size_t> gScores;
    Cube *currentCube;
    bool search;

    cubeHeap.push_back(new Cube(cube));

    while(cubeHeap.size()){

        currentCube = cubeHeap.front();

        std::pop_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);
        cubeHeap.pop_back();

        search = !gScores.count(currentCube->getCubies()) || currentCube->getMoves().size() < gScores[currentCube->getCubies()];

        if(search){

            //currentCube->printCube();

            if(currentCube->isComplete()){
                for(unsigned long i = 0; i < cubeHeap.size(); i++){
                    delete cubeHeap[i];
                }
                Cube retCube(*currentCube);
                delete currentCube;
                return retCube;
            }

            gScores[currentCube->getCubies()] = currentCube->getMoves().size();

            for(int dir = 0; dir < 3; dir++){
                for(int depth = 0; depth < currentCube->getCubeSize(); depth++){
                    for(int ccw = 0; ccw < 2; ccw++){
                        Cube *newCube = new Cube(*currentCube);
                        newCube->doMove(Move(dir, depth, ccw));
                        cubeHeap.push_back(newCube);
                        std::push_heap(cubeHeap.begin(),cubeHeap.end(), compare_cube);

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

Cube solve_astar_3(Cube cube){

    for(int i = 0; i < 4; i++){
        if(cube.getFaceCol(FACE_FRONT,1,1) == COL_RED) break;
        cube.rotateCube(DIR_Y,false);
    }
    cube.rotateCube(DIR_X, false);
    for(int i = 0; i < 4; i++){
        if(cube.getFaceCol(FACE_FRONT,1,1) == COL_RED) break;
        cube.rotateCube(DIR_Y,false);
    }
    while(cube.getFaceCol(FACE_UP,1,1) != COL_YELLOW) cube.rotateCube(DIR_X, false);

    std::vector<Cube*> cubeHeap;

    std::vector<Move> moves = getMovesFromStr("FF\'BB\'UU\'DD\'LL\'RR\'");

    std::map<std::vector<cubie_t>,int> cubeScores;
    Cube *currentCube;
    bool search;

    cubeHeap.push_back(new Cube(cube));

    while(cubeHeap.size()){

        currentCube = cubeHeap.front();

        std::pop_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);
        cubeHeap.pop_back();

        search = !cubeScores.count(currentCube->getCubies()) || currentCube->getScore() < cubeScores[currentCube->getCubies()];

        if(search){

            //currentCube->printCube();

            if(currentCube->isComplete()){
                for(unsigned long i = 0; i < cubeHeap.size(); i++){
                    delete cubeHeap[i];
                }
                Cube retCube(*currentCube);
                delete currentCube;
                return retCube;
            }

            /*for(int dir = 0; dir < 3; dir++){
                for(int n = 0; n < 4; n++){
                    cubieSet.insert(currentCube->getCubies());
                    currentCube->rotateCube(dir,false);
                }
            }*/
            cubeScores.insert(std::pair<std::vector<cubie_t>,int>(currentCube->getCubies(),currentCube->getScore()));
            for(Move move: moves){
                Cube *newCube = new Cube(*currentCube);
                newCube->doMove(move);
                cubeHeap.push_back(newCube);
                std::push_heap(cubeHeap.begin(), cubeHeap.end(), compare_cube);
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