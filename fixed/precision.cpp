#include <iostream>
#include <iomanip>
int main() {
    double a = 0.1 + 0.2;
    if (a == 0.3)
        std::cout << "0.1 + 0.2 == 0.3\n";
    else
        std::cout << "0.1 + 0.2 != 0.3 (Actual: " << std::setprecision(100) << a << ")\n";
}