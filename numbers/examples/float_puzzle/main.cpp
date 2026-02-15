#include <iostream>
#include <iomanip>
#include <limits>

int main() {
    std::cout << std::setprecision(20);

    std::cout << "--- Puzzle 1: .1 + .2 == .3? ---" << std::endl;
    double a = 0.1;
    double b = 0.2;
    double c = 0.3;
    
    if (a + b == c) {
        std::cout << "Equal!" << std::endl;
    } else {
        std::cout << "Not Equal!" << std::endl;
        std::cout << "0.1 + 0.2 = " << (a + b) << std::endl;
        std::cout << "0.3       = " << c << std::endl;
    }

    std::cout << "\n--- Puzzle 2: Associativity ---" << std::endl;
    // (a + b) + c != a + (b + c)
    // Use small and large numbers to lose precision
    float x = 1.0f;
    float y = 1e20f;
    float z = -1e20f;

    float r1 = (x + y) + z; // (1 + 1e20) - 1e20 = 1e20 - 1e20 = 0 (1 is lost)
    float r2 = x + (y + z); // 1 + (1e20 - 1e20) = 1 + 0 = 1

    std::cout << "(1 + 1e20) - 1e20 = " << r1 << std::endl;
    std::cout << "1 + (1e20 - 1e20) = " << r2 << std::endl;

    return 0;
}
