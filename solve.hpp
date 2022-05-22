#ifndef SOLVE_H
#define SOLVE_H

#include <vector>
#include <string>

#include "cube.hpp"

int heuristic_basic(Cube);
bool compare_cube(Cube *c1, Cube *c2);

Cube solve_bfs(Cube);
Cube solve_astar(Cube);

void reduce_to_3x3(Cube&);
void bottom_cross(Cube&);

std::vector<Move> getReverseMoves(std::vector<Move> moves);

//Commonly used algorithms, according to ruwix.com

const std::string sune = "RUR\'URUUR\'";
const std::string sledgehammer = "R\'FRF\'";
const std::string sexy = "RUR\'U\'";
const std::string u_perm_a = "2L2LU2LUU2L\'U2L2L";
const std::string u_perm_b = "2L2LU\'2LUU2L\'U\'2L2L";
const std::string t_perm = "RUR\'UR\'FRRU\'R\'U\'RUR\'F\'";
const std::string j_perm_a = "R\'UL\'UURU\'R\'UURL";
const std::string j_perm_b = "RUR\'F\'RUR\'U\'R\'FRRU\'R\'U\'";
const std::string h_perm = "2L2LU\'2L2LUU2L2LU\'2L2L";
const std::string oll_key = "RUR\'U\'R\'FRF\'";
const std::string oll_bottlecap = "FURU\'R\'URU\'R\'F\'";
const std::string oll_t = "FRUR\'U\'F\'";
const std::string checkerboard = "RRLLUUDDFFBB";
const std::string z_perm = "2L\'U\'2L2LU\'2L2LU\'2L\'UU2L2LU";

#endif