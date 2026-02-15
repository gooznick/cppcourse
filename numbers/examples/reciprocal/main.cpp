#include <iostream>
#include <iomanip>

int main() {
    float a = 1.2f;
    float div = a / 3.0f;
    float mul = a * (1.0f / 3.0f);

    std::cout << std::setprecision(20);
    std::cout << "1.2 / 3.0       = " << div << std::endl;
    std::cout << "1.2 * (1.0/3.0) = " << mul << std::endl;

    if (div == mul) {
        std::cout << "Equal!" << std::endl;
    } else {
        std::cout << "Not Equal!" << std::endl;
        std::cout << "Diff: " << (div - mul) << std::endl;
    }

    // Note: Compile with -freciprocal-math (or -Ofast) to see them become Equal!
    // The compiler will optimize division to multiplication by reciprocal.

    return 0;
}
