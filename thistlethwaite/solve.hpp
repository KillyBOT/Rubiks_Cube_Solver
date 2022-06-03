#ifndef SOLVE_H
#define SOLVE_H

#include <vector>
#include <string>
#include <unordered_map>

#include "cube.hpp"

//std::vector<move_t> solve_thistlethwaite_edges(Cube);
//std::vector<move_t> solve_thistlethwaite_corners(Cube);
//std::vector<move_t> solve_thistlethwaite_orbit(Cube);
//std::vector<move_t> solve_thistlethwaite(Cube);

//std::vector<move_t> solve_thistlethwaite(Cube cube, bool (Cube::*)() = Cube::isComplete, std::string = "", std::vector<move_t> = std::vector<move_t>());

std::vector<move_t> solve_thistlethwaite(Cube);
bool solve_thistlethwaite_iddfs(Cube, bool (Cube::*)(), std::vector<move_t>&, std::vector<move_t>&, std::unordered_map<compact_t,bool> &, int, int&);
bool solve_thistlethwaite_pruner(move_t, move_t);

void write_map_thistlethwaite();
std::unordered_map<compact_t,byte_t> read_map_thistlethwaite();


void write_map_korf(std::string, int (Cube::*)(), int size);
bool write_map_korf_iddfs(Cube &, int (Cube::*)(), std::unordered_map<int,byte_t>&, std::vector<move_t> &, int, int&);
std::unordered_map<int,byte_t> read_map_korf(std::string);
std::vector<move_t> solve_korf(Cube);

#endif