#include <iostream>

#include "cube.hpp"

int main()
{
   cubie_t testCubie = newCubie();
   printCubie(testCubie);

   for(int x = 0; x < 4; x++){
       rotateCubie(testCubie, x);
       printCubie(testCubie);
   }

   return 0; 
}