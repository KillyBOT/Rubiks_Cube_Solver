#include <iostream>
#include <vector>
#include <string>

#include <cstdlib>
#include <ctime>

#include "cubie.hpp"
#include "cube.hpp"
#include "solve.hpp"

Cube::Cube(int size) : cubeSize(size), faceSize(size * size) {
    //6x^2 - 12x + 8 gives you the number of cubies required for cubeSize x, but I'm just doing x^3 since that's easier to program, despite it creating cubies that won't ever be used.
    //The cubies are all only 4 bytes large though, so it's not that big of a deal
    for(int ind = 0; ind < this->faceSize * this->cubeSize; ind++){
        this->cubies.push_back(newCubie());
    }

    this->rings = new std::vector<std::vector<int>>[this->cubeSize * 3];

    for(int dir = 0; dir < 3; dir++){
        for(int depth = 0; depth < this->cubeSize; depth++){
            this->rings[dir * this->cubeSize + depth] = this->makeRing(dir, depth);

            /*for(std::vector<int> ring : this->rings[dir * this->cubeSize + depth]){
                for(int ind : ring)
                    std::cout << ind << " ";
                std::cout << std::endl;
            }*/
        }
    }

    this->heuristicFunc = heuristic_basic;
    //this->score = this->heuristicFunc(*this);
}
Cube::Cube(int size, int (*hFunc)(Cube)) : cubeSize(size), faceSize(size * size), heuristicFunc(hFunc){
    //6x^2 - 12x + 8 gives you the number of cubies required for cubeSize x, but I'm just doing x^3 since that's easier to program, despite it creating cubies that won't ever be used.
    //The cubies are all only 4 bytes large though, so it's not that big of a deal
    for(int ind = 0; ind < this->faceSize * this->cubeSize; ind++){
        this->cubies.push_back(newCubie());
    }

    this->rings = new std::vector<std::vector<int>>[this->cubeSize * 3];

    for(int dir = 0; dir < 3; dir++){
        for(int depth = 0; depth < this->cubeSize; depth++){
            this->rings[dir * this->cubeSize + depth] = this->makeRing(dir, depth);

            /*for(std::vector<int> ring : this->rings[dir * this->cubeSize + depth]){
                for(int ind : ring)
                    std::cout << ind << " ";
                std::cout << std::endl;
            }*/
        }
    }
    //this->score = this->heuristicFunc(*this);
}
Cube::Cube(const Cube &cube) : cubeSize(cube.cubeSize) , faceSize(cube.faceSize) {

    this->rings = new std::vector<std::vector<int>>[this->cubeSize * 3];

    for(int dir = 0; dir < 3; dir++){
        for(int depth = 0; depth < this->cubeSize; depth++){
            this->rings[dir * this->cubeSize + depth] = cube.rings[dir * this->cubeSize + depth];
        }
    }

    this->cubies = cube.cubies;
    this->moves = cube.moves;
    this->heuristicFunc = cube.heuristicFunc;
    //this->score = cube.score;
}
Cube::~Cube(){
    delete[] this->rings;
}

bool Cube::operator==(const Cube& cube){
    return this->cubies == cube.cubies;
}

std::vector<int> Cube::getFaceInds(byte_t dir, int depth){
    std::vector<int> inds;
    int startInd, dRow, dCol;

    dRow = this->faceSize;
    dCol = 1;

    switch(dir){
        case DIR_X:
        default:
        startInd = this->faceSize - ((depth + 1)* this->cubeSize);
        //dRow = this->faceSize;
        //dCol = 1;
        break;
        case DIR_Y:
        startInd = depth * this->faceSize;
        dRow = this->cubeSize;
        //dCol = 1;
        break;
        case DIR_Z:
        startInd = depth;
        //dRow = this->faceSize;
        dCol = this->cubeSize;
        break;
    }

    for(int row = 0; row < this->cubeSize; row++){
        for(int col = 0; col < this->cubeSize; col++){
            inds.push_back(startInd + row*dRow + col*dCol);
        }
    }

    return inds;
}
std::vector<std::vector<int>> Cube::makeRing(byte_t dir, int depth){
    std::vector<int> faceInds = this->getFaceInds(dir, depth);
    std::vector<std::vector<int>> rings;
    int startInd = 0;
    int ind;

    for(int n = 0; n < (this->cubeSize + 1)/ 2; n++){
        std::vector<int> ring;

        ind = startInd;

        if(this->cubeSize-n-1 == n) ring.push_back(faceInds[ind]);

        else{
            for(int x = n; x < this->cubeSize-n-1; x++){
                ring.push_back(faceInds[ind]);
                ind++;
            }
            for(int x = n; x < this->cubeSize-n-1; x++){
                ring.push_back(faceInds[ind]);
                ind += this->cubeSize;
            }
            for(int x = n; x < this->cubeSize-n-1; x++){
                ring.push_back(faceInds[ind]);
                ind--;
            }
            for(int x = n ; x < this->cubeSize-n-1; x++){
                ring.push_back(faceInds[ind]);
                ind -= this->cubeSize;
            }
        }

        startInd += this->cubeSize + 1;
        rings.push_back(ring);
    }

    return rings;
}
std::vector<std::vector<int>> Cube::getRing(byte_t dir, int depth){
    return this->rings[dir * this->cubeSize + depth];
}
col_t Cube::getFaceCol(byte_t face, int row, int col){

    while(row < 0) row += this->cubeSize;
    while(col < 0) col += this->cubeSize;
    while(row >= this->cubeSize) row -= this->cubeSize;
    while(col >= this->cubeSize) col -= this->cubeSize;

    std::vector<int> inds = this->getFaceInds(face / 2, (face % 2) * (this->cubeSize - 1));

    int startInd = 0;

    if(face == FACE_BACK || face == FACE_RIGHT){
        startInd = this->cubeSize - 1;
        col *= -1;
    }
    if(face == FACE_DOWN){
        startInd = this->faceSize - this->cubeSize;
        row *= -1;
    }

    return getCubieFace(this->cubies[inds[startInd + row * this->cubeSize + col]],face);
}

col_t Cube::getCubieFaceCol(byte_t face, int x, int y, int z){
    while(x < 0) x += this->cubeSize;
    while(y < 0) y += this->cubeSize;
    while(z < 0) z += this->cubeSize;

    while(x >= this->cubeSize) x -= this->cubeSize;
    while(y >= this->cubeSize) y -= this->cubeSize;
    while(z >= this->cubeSize) z -= this->cubeSize;

    return getCubieFace(this->cubies[y * this->faceSize + x * this->cubeSize + z], face);
}

void Cube::printFace(byte_t dir, int depth, byte_t face){
    std::vector<int> inds = this->getFaceInds(dir, depth);
    /*for(int ind : inds){
        std::cout << ind << " ";
    }
    std::cout << std::endl;*/

    switch(face){
        case FACE_UP:
        case FACE_LEFT:
        case FACE_FRONT:
        default:
            for(int row = 0; row < this->cubeSize; row++){
                for(int col = 0; col < this->cubeSize; col++){
                    std::cout << colStrings[getCubieFace(this->cubies[inds[row * this->cubeSize + col]],face)] << " ";
                }
                std::cout << std::endl;
            }
            break;
        case FACE_BACK:
        case FACE_RIGHT:
            for(int row = 0; row < this->cubeSize; row++){
                for(int col = this->cubeSize - 1; col >= 0; col--){
                    std::cout << colStrings[getCubieFace(this->cubies[inds[row * this->cubeSize + col]],face)] << " ";
                }
                std::cout << std::endl;
            }
            break;
        case FACE_DOWN:
            for(int row = this->cubeSize - 1; row >= 0; row--){
                for(int col = 0; col < this->cubeSize; col++){
                    std::cout << colStrings[getCubieFace(this->cubies[inds[row * this->cubeSize + col]],face)] << " ";
                }
                std::cout << std::endl;
            }
            break;
    }

    
}
void Cube::printCube(){
    std::cout << "FRONT:" << std::endl;
    this->printFace(DIR_X,0,FACE_FRONT);
    std::cout << "UP:" << std::endl;
    this->printFace(DIR_Y,0,FACE_UP);
    std::cout << "LEFT:" << std::endl;
    this->printFace(DIR_Z,0,FACE_LEFT);
    std::cout << "BACK:" << std::endl;
    this->printFace(DIR_X,this->cubeSize-1,FACE_BACK);
    std::cout << "DOWN:" << std::endl;
    this->printFace(DIR_Y,this->cubeSize-1,FACE_DOWN);
    std::cout << "RIGHT:" << std::endl;
    this->printFace(DIR_Z,this->cubeSize-1,FACE_RIGHT);
    //std::cout << this->score << std::endl;
    std::cout << std::endl;
}

void Cube::setHeuristicFunc(int (*hFunc)(Cube)){
    this->heuristicFunc = hFunc;
    //this->score = this->heuristicFunc(*this);
}
void Cube::rotateFace(byte_t dir, int depth, bool counterClockwise){
    
    if(depth < 0) depth += this->cubeSize;
    
    int ringsInd = dir * this->cubeSize + depth;
    //int ind;
    cubie_t temp;

    for(std::vector<int> ring : this->rings[ringsInd]){

        for(int n = 1; n < this->cubeSize; n++){
            if(counterClockwise){
                temp = this->cubies[ring.front()];

                for(unsigned long ind = 0; ind < ring.size() - 1; ind++ ){
                    this->cubies[ring[ind]] = this->cubies[ring[ind+1]];
                }

                this->cubies[ring.back()] = temp;
                
            } else {
                temp = this->cubies[ring.back()];

                for(unsigned long ind = ring.size() - 1; ind > 0; ind--){
                    this->cubies[ring[ind]] = this->cubies[ring[ind-1]];
                }

                this->cubies[ring.front()] = temp;
            }
        }

        for(int ind : ring) rotateCubie(this->cubies[ind],dir,counterClockwise);
    }

}
void Cube::rotateCube(byte_t dir, bool counterClockwise){
    for(int depth = 0; depth < this->cubeSize; depth++) rotateFace(dir, depth, counterClockwise);
}
void Cube::fixOrientation(){
    for(int i = 0; i < 4; i++){
        if(this->getFaceCol(FACE_FRONT,1,1) == COL_RED) break;
        this->rotateCube(DIR_Y,false);
    }
    this->rotateCube(DIR_X, false);
    for(int i = 0; i < 4; i++){
        if(this->getFaceCol(FACE_FRONT,1,1) == COL_RED) break;
        this->rotateCube(DIR_Y,false);
    }
    while(this->getFaceCol(FACE_UP,1,1) != COL_YELLOW) this->rotateCube(DIR_X, false);
}
void Cube::doMove(Move move, bool addToMoves){
    
    if(move.depth >= this->cubeSize || -move.depth >= this->cubeSize) return;

    for(int n = 0; n < move.num; n++) this->rotateFace(move.dir, move.depth, move.ccw);
    if(addToMoves) this->moves.push_back(move);

    //this->score = this->heuristicFunc(*this);
    
    //move.printMove();
    //this->printCube();
}
void Cube::doMoves(std::vector<Move> moves, bool addToMoves){
    for(Move move : moves)this->doMove(move, addToMoves);
}
void Cube::randomize(int randomMoveNum){
    std::srand(std::time(nullptr));
    std::vector<Move> moves;

    for(int n = 0; n < randomMoveNum; n++)moves.push_back(Move(rand() % 3, rand() % this->cubeSize - this->cubeSize / 2, (rand() % 2) + 1, rand() % 2));
    this->doMoves(moves, false);

    for(Move move: moves){
        move.printMove();
    }
    std::cout << std::endl;
}

/*int Cube::heuristic_stupid(){

    int lowestScore = 6 * this->faceSize - 12 * this->cubeSize + 9;
    int score;
    cubie_t testCubie = newCubie();

    for(int dir = 0; dir < 3; dir++){
        for(int n = 0; n < 4; n++){
            score = 6 * this->faceSize - 12 * this->cubeSize + 8;

            for(int ind : this->getFaceInds(0,0)){
                if(this->cubies[ind] == testCubie) score--;
                if(this->cubies[ind - this->cubeSize * (this->cubeSize - 1)] == testCubie) score--;
            }

            for(int depth = 1; depth < this->cubeSize - 1; depth++){
                for(int ind : this->rings[depth][0]){
                    if(this->cubies[ind] == testCubie) score--;
                }
            }

            if(score < lowestScore) lowestScore = score;

            rotateCubie(testCubie,dir,false);
        }
    }

    int lowestScore = 6 * this->faceSize;

    for(int dir = 0; dir < 3; dir++){
        for(int side = 0; side < 2; side++){
            for(int ind : this->getFaceInds(dir,side * (this->cubeSize - 1))){
                if(getCubieFace(this->cubies[ind], dir * 2 + side ) == dir * 2 + side) lowestScore--;
            }
        }
    }

    //return this->moves.size();
    return lowestScore + this->moves.size();
}
*/

bool Cube::isComplete(){
    /*cubie_t checkCubie = this->cubies[0];

    for(int ind = 1; ind < this->cubeSize * this->faceSize; ind++){
        if(this->cubies[ind] != checkCubie) return false;
    }

    return true;*/

    /*cubie_t checkCubie = newCubie();

    for(cubie_t cubie : this->cubies){
        if(cubie != checkCubie) return false;
    }
    return true;*/

    return !(heuristic_basic(*this) - 4 * this->cubeSize * this->moves.size());
}

bool Cube::isOriented(){
    return !(heuristic_orientation(*this) - this->moves.size());
}

void Cube::printMoves(){
    for(Move move: this->moves){
        move.printMove();
    }
    std::cout << std::endl;
}

void Cube::reverseFromMoves(){
    int moveNum = this->moves.size();
    Move move;

    for(int x = 0; x < moveNum; x++){
        move = this->moves.back();
        this->rotateFace(move.dir,move.depth,!move.ccw);
        this->moves.pop_back();
    }

    //this->score = this->heuristicFunc(*this);
}
