#include <iostream>

#include "cube.hpp"

int main()
{
   cubie_t testCubie = newCubie();
   printCubie(testCubie);

   /*for(int dir = 0; dir < 3; dir++){
       rotateCubie(testCubie,dir,false);
       printCubie(testCubie);
       rotateCubie(testCubie,dir,true);
       printCubie(testCubie);
   }*/

   Cube cube(6);
   cube.printCube();
   cube.doMove(0);
   cube.printCube();
   //std::cout << cube.isComplete() << std::endl;
   return 0; 
}