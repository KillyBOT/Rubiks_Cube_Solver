#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

#include "cube.hpp"

Cube solve_stupid(Cube cube){
    std::vector<Cube*> cubeHeap;

    std::set<std::vector<cubie_t>> cubieSet;
    Cube *currentCube;
    bool search;

    cubeHeap.push_back(new Cube(cube));

    while(cubeHeap.size()){

        currentCube = cubeHeap.front();

        std::pop_heap(cubeHeap.begin(), cubeHeap.end(), compare_stupid);
        cubeHeap.pop_back();

        search = !cubieSet.count(currentCube->getCubies());

        if(search){

            //currentCube->printCube();

            if(currentCube->isComplete()){
                for(unsigned long i = 0; i < cubeHeap.size(); i++){
                    delete cubeHeap[i];
                }
                return *currentCube;
            }

            for(int dir = 0; dir < 3; dir++){
                for(int n = 0; n < 4; n++){
                    cubieSet.insert(currentCube->getCubies());
                    currentCube->rotateCube(dir,false);
                }
            }
            cubieSet.insert(currentCube->getCubies());
            for(int dir = 0; dir < 3; dir++){
                for(int depth = 0; depth < currentCube->getCubeSize(); depth++){
                    for(int ccw = 0; ccw < 2; ccw++){
                        Cube *newCube = new Cube(*currentCube);
                        newCube->doMove(Move(dir, depth, ccw));
                        cubeHeap.push_back(newCube);
                        std::push_heap(cubeHeap.begin(),cubeHeap.end(), compare_stupid);

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

bool compare_stupid(const Cube *c1, const Cube *c2){
    return c1->score > c2->score;
}