#ifndef SOLVE_H
#define SOLVE_H

#include <vector>
#include <string>

#include "cube.hpp"

int heuristic_basic(Cube);
bool compare_cube(Cube *c1, Cube *c2);

struct cubieHash {
    size_t operator()(const std::vector<cubie_t>&) const;
};

Cube solve_bfs(Cube);
Cube solve_astar(Cube);
//Cube solve_astar_3(Cube); //Astar, but only for a 3x3 cube.

Cube orient_astar_3(Cube);
Cube solve_orientd_astar(Cube);

std::vector<Move> getReverseMoves(std::vector<Move> moves);

//Commonly used algorithms, according to ruwix.com

const std::string sune = "R U R\' U R U2 R\'";
const std::string sledgehammer = "R\' F R F\'";
const std::string sexy = "R U R\' U\'";
const std::string u_perm = "2L2 U 2L U2 2L\' U 2L2";
const std::string t_perm = "R U R\' U R\' F R2 U\' R\' U\' R U R\' F\'";
const std::string j_perm_a = "R\' U L\' U2 R U\' R\' U2 R L";
const std::string j_perm_b = "R U R\' F\' R U R\' U\' R\' F R2 U\' R\' U\'";
const std::string h_perm = "2L2 U\' 2L2 U2 2L2 U\' 2L2";
const std::string oll_key = "R U R\' U\' R\' F R F\'";
const std::string oll_bottlecap = "F U R U\' R\' U R U\' R\' F\'";
const std::string oll_t = "F R U R\' U\' F\'";
const std::string checkerboard = "R2 L2 U2 D2 F2 B2";
const std::string z_perm = "2L\' U\' 2L2 U\' 2L2 U\' 2L\' U2 2L2 U";

#endif