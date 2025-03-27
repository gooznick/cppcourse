#include <iostream>

// This version conflicts with myexe.cpp
#pragma detect_mismatch("VERSION", "1.0")

int giveMe5()
{
    std::cout << "Library version 1.0\n";
    return 5;
}
