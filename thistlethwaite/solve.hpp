#ifndef SOLVE_H
#define SOLVE_H

#include <vector>
#include <string>
#include <unordered_map>

#include "cube.hpp"

//std::vector<move_t> thistlethwaite_solve_edges(Cube);
//std::vector<move_t> thistlethwaite_solve_corners(Cube);
//std::vector<move_t> thistlethwaite_solve_orbit(Cube);
//std::vector<move_t> thistlethwaite_solve(Cube);

//std::vector<move_t> thistlethwaite_solve(Cube cube, bool (Cube::*)() = Cube::isComplete, std::string = "", std::vector<move_t> = std::vector<move_t>());

std::vector<move_t> thistlethwaite_solve(Cube);
bool thistlethwaite_solve_iddfs(Cube &, bool (Cube::*)(), std::vector<move_t>&, std::vector<move_t>&, std::unordered_map<compact_t,bool> &, int, int&);
std::vector<move_t> thistlethwaite_solve_bfs(Cube, bool (Cube::*)(), std::vector<move_t> &);
bool solve_pruner(move_t, move_t);

void thistlethwaite_write_map(std::string, int (Cube::*)());
void thistlethwaite_write_maps();
std::unordered_map<int,std::pair<int,compact_t>> thistlethwaite_read_map(std::string);

void korf_create_map(std::string, int (Cube::*)(), byte_t);
void korf_create_map_dfs(Cube &, int (Cube::*)(), std::unordered_map<int,byte_t>&, std::vector<move_t>&, move_t, byte_t, byte_t&);
void korf_create_maps();
void korf_write_map(std::string,std::unordered_map<int,byte_t>&);
std::unordered_map<int,byte_t> korf_read_map(std::string);

std::vector<move_t> korf_solve(Cube);
bool korf_solve_ida(Cube &, std::vector<move_t> &, std::vector<move_t> &, int, int);

#endif