#include "mylib.h"
#include <iostream>

// Conflicting mismatch directive!
#pragma detect_mismatch("VERSION", "2.0")

int main() {
    std::cout << "Main: calling lib\n";
    giveMe5();
    return 0;
}
