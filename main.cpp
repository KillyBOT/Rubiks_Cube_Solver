#include <iostream>
#include <string>

#include "cube.hpp"
#include "solve.hpp"

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
    std::cout << cube.isComplete() << std::endl;
    std::cout << cube.isOriented() << std::endl;
    //std::cout << cube.getFaceCol(FACE_DOWN, 1, 1) << std::endl;
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

    cube.randomize(6);
    std::cout << heuristic_orientation(cube) << std::endl;
    cube.printCube();
    //cube.doMoves(getReverseMoves(cube.getMoves()));
    //cube.printCube();

    //cube.doMoves(getMovesFromStr(z_perm));
    //cube.printCube();
    //cube.printMoves();
    /*for(byte_t face = 0; face < 6; face++){
        for(int row = 0; row < cube.getCubeSize(); row++){
            for(int col = 0; col < cube.getCubeSize(); col++){
                std::cout << (int)cube.getFaceCol(face, row, col) << " ";
            }
            std::cout << std::endl;
        }
    }*/
    //std::cout << cube.score << std::endl;

    //Cube solvedCube = solve_bfs(cube);
    //solvedCube.printMoves();

    //Cube orientedCube = orient_astar(cube);
    //orientedCube.printCube();
    //orientedCube.printMoves();

    Cube solvedCube = solve_astar(cube);
    //Cube solvedCube = solve_astar_3(cube);
    solvedCube.printMoves();

    return 0;
}