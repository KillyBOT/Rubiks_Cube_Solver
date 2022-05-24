#include <iostream>
#include <vector>
#include <string>

#include "move.hpp"
#include "cubie.hpp"

Move::Move(byte_t dir, int depth, bool ccw){
    this->dir = dir;
    this->depth = depth;
    this->ccw = ccw;
    this->num = 1;
    
    this->moveStr = "";
    if(this->depth > 0){
        moveStr += std::to_string(depth + 1);
    } else if (this->depth < -1){
        moveStr += std::to_string(-depth);
    }

    switch(dir){
        case DIR_X:
        default:
        if(this->depth >= 0) moveStr += "F";
        else moveStr += "B";
        break;
        case DIR_Y:
        if(this->depth >= 0) moveStr += "U";
        else moveStr += "D";
        break;
        case DIR_Z:
        if(this->depth >= 0) moveStr += "L";
        else moveStr += "R";
    }
    if((this->depth < 0 && !ccw) || (this->depth >= 0 && ccw)) moveStr += "\'";
}
Move::Move(byte_t dir, int depth, int num, bool ccw){
    this->dir = dir;
    this->depth = depth;
    this->ccw = ccw;
    this->num = num;
    
    this->moveStr = "";
    if(this->depth > 0){
        moveStr += std::to_string(depth + 1);
    } else if (this->depth < -1){
        moveStr += std::to_string(-depth);
    }

    switch(dir){
        case DIR_X:
        default:
        if(this->depth >= 0) moveStr += "F";
        else moveStr += "B";
        break;
        case DIR_Y:
        if(this->depth >= 0) moveStr += "U";
        else moveStr += "D";
        break;
        case DIR_Z:
        if(this->depth >= 0) moveStr += "L";
        else moveStr += "R";
    }
    if(this->num > 1) moveStr += std::to_string(this->num);
    if((this->depth < 0 && !ccw) || (this->depth >= 0 && ccw)) moveStr += "\'";
}
Move::Move(std::string str){

    this->moveStr = str;

    size_t p = 0;
    size_t q = 0;

    while(!isalpha(str[p])) p++;
    this->depth = p ? std::stoi(str.substr(0,p)) - 1 : 0;

    this->ccw = false;
    this->num = 1;
    
    switch(str[p]){
        case 'F':
        default:
        this->dir = DIR_X;
        break;
        case 'R':
        this->dir = DIR_Z;
        this->ccw = true;
        this->depth *= -1;
        this->depth--;
        break;
        case 'U':
        this->dir = DIR_Y;
        break;
        case 'L':
        this->dir = DIR_Z;
        break;
        case 'D':
        this->dir = DIR_Y;
        this->ccw = true;
        this->depth *= -1;
        this->depth--;
        break;
        case 'B':
        this->dir = DIR_X;
        this->ccw = true;
        this->depth *= -1;
        this->depth--;
        break;
    }

    p++;
    while(p+q < str.length() && str[p+q] != '\'') q++;
    if(q > 0) this->num = std::stoi(str.substr(p,q));
    p += q;
    if(p < str.length()) this->ccw = !this->ccw;
}
Move::Move(){
    this->dir = DIR_X;
    this->depth = 0;
    this->ccw = false;
    this->moveStr = "F";
    this->num = 1;
}
void Move::printMove(){
    std::cout << this->moveStr << " ";
    //std::cout << this->moveStr << " " << (int)this->dir << " " << this->depth << " " << this->num << " " << this->ccw << " ";
}
std::vector<Move> getMovesFromStr(std::string str){
    std::vector<Move> moves;

    size_t start = 0;
    size_t len;

    do{
        len = 0;
        while(start + len < str.size() && str[start + len] != ' ') len++;

        moves.push_back(Move(str.substr(start,len)));
        start += len + 1;
    } while (start < str.size());

    return moves;
}

Move3::Move3(byte_t face, int num, bool ccw){
    this->face = face;
    this->num = num;
    this->ccw = ccw;

    this->moveStr = "";
    this->moveStr += faceStrings[face];
    if(num > 1) this->moveStr += std::to_string(num);
    if(ccw) this->moveStr += "\'";
}

Move3::Move3(std::string moveStr){
    this->moveStr = moveStr;

    size_t p = 0;

    switch(moveStr[0]){
        case 'F':
        default:
        this->face = FACE_FRONT;
        break;
        case 'B':
        this->face = FACE_BACK;
        break;
        case 'U':
        this->face = FACE_UP;
        break;
        case 'D':
        this->face = FACE_DOWN;
        break;
        case 'L':
        this->face = FACE_LEFT;
        break;
        case 'R':
        this->face = FACE_RIGHT;
        break;
    }

    while(p+1 < moveStr.size() && moveStr[p+1] != '\'') p++;
    this->num = p ? std::stoi(moveStr.substr(1,p)) : 1;
    this->ccw = (moveStr.back() == '\'');
}

void Move3::printMove(){
    std::cout << this->moveStr << " ";
}

std::vector<Move3> getMove3FromStr(std::string str){
    std::vector<Move3> moves;

    size_t start = 0;
    size_t len;

    do{
        len = 0;
        while(start + len < str.size() && str[start + len] != ' ') len++;

        moves.push_back(Move3(str.substr(start,len)));
        start += len + 1;
    } while (start < str.size());

    return moves;
}