#ifndef CUBIE_H
#define CUBIE_H

#define FACE_NUM 6
#define FACE_FRONT 0
#define FACE_BACK 1
#define FACE_UP 2
#define FACE_DOWN 3
#define FACE_LEFT 4
#define FACE_RIGHT 5

#define COL_RED 0
#define COL_ORANGE 1
#define COL_YELLOW 2
#define COL_WHITE 3
#define COL_BLUE 4
#define COL_GREEN 5

#define DIR_X 0
#define DIR_Y 1
#define DIR_Z 2

const std::string faceStrings[] = {"Front", "Back", "Top", "Bottom", "Left", "Right"};
const std::string colStrings[] = {"R","O","Y","W","B","G"};

using byte_t = unsigned char;
using cubie_t = uint32_t;
using col_t = byte_t;

cubie_t newCubie();
void printCubie(cubie_t&);
col_t getCubieFace(cubie_t, byte_t);
void setCubieFace(cubie_t&, byte_t, col_t);
void rotateCubie(cubie_t&, byte_t, bool);

bool cubieOriented(cubie_t);

#endif