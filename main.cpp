#include <iostream>
#include <string>

#include "cube.hpp"

int main()
{
    /*cubie_t testCubie = newCubie();
    printCubie(testCubie);

    for(int dir = 0; dir < 3; dir++){
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
    //std::cout << cube.score << std::endl;

    /*for(int dir = 0; dir < 3; dir++){
        for(int depth = 0; depth < 3; depth++){
            cube.doMove(Move(dir, depth, false));
            cube.printCube();
        }
    }*/

    //cube.doMove(Move(DIR_X,0,false)); // F
    //cube.doMove(Move(DIR_Z,2,true)); //R
    //cube.doMove(Move(DIR_X,1,false)); //F2
    //cube.doMove(Move(DIR_Z,1,false)); //L2
    //cube.printCube();

    /*Move testMove(std::string("2R\'"));
    testMove.printMove();
    std::cout << std::endl;*/

    cube.randomize(3);
    cube.printCube();
    //std::cout << cube.score << std::endl;

    Cube solvedCube = solve_stupid(cube);
    solvedCube.printMoves();

    return 0;
}