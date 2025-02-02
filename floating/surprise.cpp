#include <iostream>
int main() {
    double a = 1e16;
    double b = 1.0;
    std::cout << "(a + b) - a = " << ((a + b) - a) << "\n";
}