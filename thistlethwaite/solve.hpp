#ifndef SOLVE_H
#define SOLVE_H

#include <vector>
#include <string>
#include <unordered_map>

#include "cube.hpp"

//std::vector<move_t> thistlewaite_solve_edges(Cube);
//std::vector<move_t> thistlewaite_solve_corners(Cube);
//std::vector<move_t> thistlewaite_solve_orbit(Cube);
//std::vector<move_t> thistlewaite_solve(Cube);

//std::vector<move_t> thistlewaite_solve(Cube cube, bool (Cube::*)() = Cube::isComplete, std::string = "", std::vector<move_t> = std::vector<move_t>());

std::vector<move_t> thistlewaite_solve(Cube);
bool thistlewaite_solve_iddfs(Cube, bool (Cube::*)(), std::vector<move_t>&, std::vector<move_t>&, std::unordered_map<compact_t,bool> &, int, int&);
bool thistlewaite_solve_pruner(move_t, move_t);

void write_map_thistlethwaite();
std::unordered_map<compact_t,byte_t> read_map_thistlethwaite();


void korf_create_maps();
bool korf_create_maps_iddfs(Cube &, std::unordered_map<int,byte_t>&, std::unordered_map<int,byte_t>&, std::unordered_map<int,byte_t>&, std::vector<move_t> &, move_t, int, int&);
std::unordered_map<int,byte_t> fork_read_map(std::string);
std::vector<move_t> korf_solve(Cube);

#endif