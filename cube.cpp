#include <iostream>
#include <vector>
#include <string>

#include "cube.hpp"

static std::string faceStrings[] = {"Front", "Back", "Top", "Right", "Left", "Bottom"};
static std::string colStrings[] = {"R","O","Y","G","B","W"};

static byte_t xFaces[] = {FACE_UP, FACE_LEFT, FACE_DOWN, FACE_RIGHT};
static byte_t yFaces[] = {FACE_BACK, FACE_LEFT, FACE_FRONT, FACE_RIGHT};
static byte_t zFaces[] = {FACE_UP, FACE_BACK, FACE_DOWN, FACE_FRONT};
/*
std::vector<int> Cube::getFaceInds(byte_t face){
    std::vector<int> inds;
    int startInd, dir, dirV, dirH; //0 = XY, 1 = YZ, 2 = XZ, 

    switch(face){
        case FACE_FRONT:
            startInd = this->faceSize - this->cubeSize;
            dir = 0;
            dirV = 1;
            dirH = 1;
        case FACE_UP:
            startInd = 0;
            dir = 2;
            dirV = 1;
            dirH = 1;
        case FACE_DOWN:
            startInd = this->faceSize * this->cubeSize - this->cubeSize;
            dir = 2;
            dirV = -1;
            dirH = 1;
        case FACE_LEFT:
            startInd = 0;
            dir = 1;
            dirV = 1;
            dirH = 1;
        case FACE_BACK:
            startInd = this->cubeSize - 1;
            dir = 0;
            dirV = 1;
            dirH = -1;
        case FACE_RIGHT:
            startInd = this->faceSize * this->cubeSize - 1;
            dir = 0;
            dirV = 1;
            dirH = -1;

    }
}*/

Cube::Cube(int size) : cubeSize(size), faceSize(size * size) {
    //6x^2 - 12x + 8 gives you the number of cubies required for cubeSize x, but I'm just doing x^3 since that's easier to program, despite it creating cubies that won't ever be used.
    //The cubies are all only 4 bytes large though, so it's not that big of a deal
    this->cubies = new cubie_t[this->faceSize * this->cubeSize];

    for(int ind = 0; ind < this->faceSize * this->cubeSize; ind++){
        this->cubies[ind] = newCubie();
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
}

Cube::~Cube(){
    delete[] this->cubies;
    delete[] this->rings;
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

    for(int n = 0; n < this->cubeSize / 2; n++){
        std::vector<int> ring;

        ind = startInd;

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

        startInd += this->cubeSize + 1;
        rings.push_back(ring);
    }

    return rings;
}

std::vector<std::vector<int>> Cube::getRing(byte_t dir, int depth){
    return this->rings[dir * this->cubeSize + depth];
}

void Cube::printFace(byte_t dir, int depth, byte_t face){
    std::vector<int> inds = this->getFaceInds(dir, depth);
    /*for(int ind : inds){
        std::cout << ind << " ";
    }
    std::cout << std::endl;*/

    for(int row = 0; row < this->cubeSize; row++){
        for(int col = 0; col < this->cubeSize; col++){
            std::cout << colStrings[getCubieFace(this->cubies[inds[row * this->cubeSize + col]],face)] << " ";
        }
        std::cout << std::endl;
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
}

void Cube::rotateFace(byte_t dir, int depth, bool counterClockwise){
    int ringsInd = dir * this->cubeSize + depth;
    int ind;

    for(int ringInd = 0; ringInd < this->rings[ringInd].size(); ringInd++){
        for(int n = 0; n < this->cubeSize - 1; n++){
            if(counterClockwise){
                ind = this->rings[ringsInd][ringInd].front();
                this->rings[ringsInd][ringInd].erase(this->rings[ringsInd][ringInd].begin());
                this->rings[ringsInd][ringInd].push_back(ind);
            } else {
                /*ind = ring.back();
                ring.erase(ring.end());
                ring.insert(ring.begin(),ind);*/
                ind = this->rings[ringsInd][ringInd].back();
                this->rings[ringsInd][ringInd].pop_back();
                this->rings[ringsInd][ringInd].insert(this->rings[ringsInd][ringInd].begin(),ind);
            }
        }

        for(int cubieInd : this->rings[ringsInd][ringInd]) rotateCubie(this->cubies[cubieInd],dir,counterClockwise);
    }

}

void Cube::doMove(move_t move){
    this->rotateFace(DIR_X, 0, false);
}

void printCubie(cubie_t &cubie){
    std::cout << " " << colStrings[getCubieFace(cubie,FACE_UP)] << std::endl;
    std::cout << colStrings[getCubieFace(cubie,FACE_LEFT)] << colStrings[getCubieFace(cubie,FACE_FRONT)] << colStrings[getCubieFace(cubie,FACE_RIGHT)] << colStrings[getCubieFace(cubie,FACE_BACK)] << std::endl;
    std::cout << " " << colStrings[getCubieFace(cubie,FACE_DOWN)] << std::endl;
}
cubie_t newCubie(){
    cubie_t cubie = 0;
    for(int x = FACE_NUM - 1; x >= 0; x--){
        cubie <<= 5;
        cubie |= x;
    }

    return cubie;
}
col_t getCubieFace(cubie_t cubie, byte_t face){
    cubie >>= face * 5;
    cubie &= 0b11111;
    return (col_t)cubie;
}
void setCubieFace(cubie_t &cubie, byte_t face, col_t col){
    cubie &= ~(0b11111 << (face * 5));
    cubie |= ((cubie_t)col) << (face * 5);
}
void rotateCubie(cubie_t &cubie, byte_t dir, bool counterClockwise){
    col_t temp;
    byte_t *rotType;

    switch(dir){
        case DIR_X:
        default:
        rotType = xFaces;
        break;
        case DIR_Y:
        rotType = yFaces;
        break;
        case DIR_Z:
        rotType = zFaces;
        break; 
    }

    if(counterClockwise){
        temp = getCubieFace(cubie,rotType[3]);
        for(int x = 3; x > 0; x--) setCubieFace(cubie,rotType[x],getCubieFace(cubie,rotType[x-1]));
        setCubieFace(cubie,rotType[0],temp);
    } else {
        temp = getCubieFace(cubie,rotType[0]);
        for(int x = 0; x < 3; x++) setCubieFace(cubie,rotType[x],getCubieFace(cubie,rotType[x+1]));
        setCubieFace(cubie,rotType[3],temp);
    }
    
}