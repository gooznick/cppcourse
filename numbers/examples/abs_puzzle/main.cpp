#include <iostream>
#include <iomanip>
#include <cstring>

// 1. The "Hack" (Undefined Behavior in C++ due to Strict Aliasing)
float abs_hack(float f) {
    int* p = (int*)&f; 
    *p &= 0x7FFFFFFF; // Clear sign bit
    return f;
}

// 2. The "Legal" Bit Hack (C++20 std::bit_cast or memcpy)
float abs_legal_hack(float f) {
    unsigned int i;
    std::memcpy(&i, &f, sizeof(float));
    i &= 0x7FFFFFFF;
    float result;
    std::memcpy(&result, &i, sizeof(float));
    return result;
}

int main() {
    float val = -42.5f;

    std::cout << "Original: " << val << std::endl;
    
    // Note: This might work on x86 gcc/clang but is Technically Undefined Behavior!
    float res1 = abs_hack(val);
    std::cout << "Hack:     " << res1 << std::endl;

    float res2 = abs_legal_hack(val);
    std::cout << "Legal:    " << res2 << std::endl;

    return 0;
}
