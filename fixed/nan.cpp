#include <iostream>
#include <cmath>
#include <limits>
int main() {
    double nan1 = 0.0 / 0.0;
    double nan2 = std::sqrt(-1.0);
    std::cout << "NaN example 1: " << nan1 << "\n";
    std::cout << "NaN example 2: " << nan2 << "\n";
    std::cout << "Is NaN? " << std::isnan(nan1) << "\n";
}