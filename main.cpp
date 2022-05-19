#include <iostream>

#include "cube.hpp"

int main()
{
    cubie_t testCubie = newCubie();
    printCubie(testCubie);

    /*for(int dir = 0; dir < 3; dir++){
        rotateCubie(testCubie,dir,false);
        printCubie(testCubie);
        rotateCubie(testCubie,dir,true);
        printCubie(testCubie);
    }*/
    /*Cube cube(10);
    cube.printCube();
    std::cout << cube.isComplete() << std::endl;
    cube.randomize(50);
    cube.printCube();
    std::cout << cube.isComplete() << std::endl;

    cube.reverseFromMoves();
    cube.printCube();
    std::cout << cube.isComplete() << std::endl;*/

    Cube cube(3);
    cube.printCube();
    std::cout << cube.score << std::endl;
    cube.randomize(50);
    cube.printCube();
    std::cout << cube.score << std::endl;

    //cube.solveStupid();

    return 0;
}