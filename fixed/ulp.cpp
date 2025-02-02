#include <iostream>
#include <cmath>
int main() {
    double x = 1.0;
    std::cout << "Nextafter: " << std::nextafter(x, 2.0) - x << "\n";
}