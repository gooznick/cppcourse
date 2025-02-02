#include <iostream>
#include <limits>

int main() {
    std::cout << "Size of float: " << sizeof(float) << " bytes\n";
    std::cout << "Size of double: " << sizeof(double) << " bytes\n";
    std::cout << "Size of long double: " << sizeof(long double) << " bytes\n";
    std::cout << "Long double max precision: " << std::numeric_limits<long double>::digits10 << " digits\n";
}
