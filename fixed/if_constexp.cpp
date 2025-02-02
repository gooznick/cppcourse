// g++ if_constexp.cpp -std=c++17

#include <iostream>
#include <type_traits>

template <typename T>
T safe_subtract(T a, T b) {
    if constexpr (std::is_unsigned_v<T>) {
        if (b > a) {
            std::cerr << "Underflow prevented! Returning 0.\n";
            return 0; // Prevent underflow for unsigned types
        }
    }
    return a - b; // Safe for signed types
}

int main() {
    unsigned int u1 = 5, u2 = 10;
    int s1 = 5, s2 = 10;

    std::cout << "Unsigned: " << safe_subtract(u1, u2) << "\n"; // Prevents underflow, prints 0
    std::cout << "Signed: " << safe_subtract(s1, s2) << "\n";   // No check needed, prints -5

    return 0;
}
