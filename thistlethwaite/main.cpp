#include <iostream>

#include "cube.hpp"
#include "solve.hpp"

int main()
{
    Cube cube;

    /*for(move_t face = 0; face < 6; face++){
        move_t move = face << 2;
        cube.doMove(move);
        std::cout << get_str_from_move(move) << std::endl;
        cube.printCube();
        cube.printOrientations();
        cube = Cube();
        move |= 0b10;
        cube.doMove(move);
        std::cout << get_str_from_move(move) << std::endl;
        cube.printCube();
        cube.printOrientations();
        cube = Cube();
        move &= 0b11111100;
        move |= 0b1;
        cube.doMove(move);
        std::cout << get_str_from_move(move) << std::endl;
        cube.printCube();
        cube.printOrientations();
        cube = Cube();
    }*/

    /*compact_t compact;
    compact = cube.getCompact();
    Cube cube2(compact);
    cube2.printCube();
    cube2.printOrientations();*/

    /*move_t move = get_move_from_str("U2");
    std::cout << get_str_from_move(move) << std::endl;
    cube.doMove(move);
    cube.printCube();
    cube.printOrientations();*/


    //cube.printCube();
    //cube.printOrientations();
    /*std::cout << std::hex << (unsigned long)(cube.getCompact() >> 64) << std::endl << (unsigned long)(cube.getCompact() & 0xFFFFFFFFFFFFFFFF)<< std::endl;
    cube.printOrientations();
    cube.doMoves(get_moves_from_str("U2 U2"));
    cube.printCube();*/

    //F B' L2 is a quick and easy randomization to check
    cube.randomize();
    cube.printCube();
    cube.printOrientations();

    std::vector<move_t> moves = thistlethwaite_solve(cube);
    for(move_t move: moves) std::cout << get_str_from_move(move) << ' ';
    std::cout << std::endl;
    cube.doMoves(moves);
    cube.printCube();
    cube.printOrientations();

    //korf_create_maps();

    return 0;
}