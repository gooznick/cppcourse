#include <iostream>


#define DEBUG_PRINT(x) std::cout << "DEBUG: " << #x " = " << x << " in line " << __LINE__ << std::endl;

int main()
{
  int x = 9;
  DEBUG_PRINT(x);
  return 0;
}