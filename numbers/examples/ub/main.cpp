#include <iostream>
#include <limits>

int main() {
  char buf[50] = "y";
  for (int j = 0; j < 9; ++j) {
    std::cout << (j * 0x20000001) << std::endl;
    if (buf[0] == 'x') break;
  }
}

// Build:
// g++ -O0 main.cpp -o ub_O0
// g++ -O3 main.cpp -o ub_O3
