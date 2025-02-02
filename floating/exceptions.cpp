#include <cfenv>
#include <iostream>
int main() {
    std::feclearexcept(FE_ALL_EXCEPT);
    double a = 1.0 / 0.0; // Infinity
    if (fetestexcept(FE_DIVBYZERO))
        std::cout << "Division by zero detected!\n";
}