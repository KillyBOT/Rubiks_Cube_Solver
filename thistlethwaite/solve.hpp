#ifndef SOLVE_H
#define SOLVE_H

#include <vector>
#include <string>
#include <unordered_map>

#include "cube.hpp"

namespace solve_defs {
    const byte_t kG1 = 0;
    const byte_t kG2 = 1;
    const byte_t kG3 = 2;
    const byte_t kG4 = 3;
};

//std::vector<move_t> solve_thistlethwaite_edges(Cube);
//std::vector<move_t> solve_thistlethwaite_corners(Cube);
//std::vector<move_t> solve_thistlethwaite_orbit(Cube);
//std::vector<move_t> solve_thistlethwaite(Cube);

//std::vector<move_t> solve_thistlethwaite(Cube cube, bool (Cube::*)() = Cube::isComplete, std::string = "", std::vector<move_t> = std::vector<move_t>());

std::vector<move_t> solve_thistlethwaite(Cube);
bool solve_thistlethwaite_helper(Cube, bool (Cube::*)(), std::vector<move_t>&, std::vector<move_t>&, std::unordered_map<compact_t,bool> &, int, int);
bool solve_thistlethwaite_pruner(move_t, move_t);

std::unordered_map<compact_t,std::vector<move_t>> generate_map_thistlethwaite();
void write_map_thistlethwaite(std::string, std::unordered_map<compact_t,std::vector<move_t>>);
std::unordered_map<compact_t,std::vector<move_t>> read_map_thistlethwaite(std::string);



#endif