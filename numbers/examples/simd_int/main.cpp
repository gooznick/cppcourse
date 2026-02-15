#include <iostream>
#include <vector>
#include <numeric>
#include <immintrin.h> // Header for intrinsics (SSE, AVX, etc.)

// Simple print helper
void print_vec(const char* label, __m128i v) {
    // Union to access elements safely
    alignas(16) int32_t values[4];
    _mm_store_si128((__m128i*)values, v);
    
    std::cout << label << ": [ ";
    for(int i=0; i<4; ++i) std::cout << values[i] << " ";
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "--- SIMD Integer Addition ---" << std::endl;

    // 1. Load data
    // Sets [1, 2, 3, 4]
    __m128i a = _mm_setr_epi32(1, 2, 3, 4); 
    // Sets [10, 20, 30, 40]
    __m128i b = _mm_setr_epi32(10, 20, 30, 40);

    // 2. Add
    __m128i sum = _mm_add_epi32(a, b);
    print_vec("Sum", sum);

    std::cout << "\n--- SIMD Overflow Behavior ---" << std::endl;
    
    // 3. Overflow
    int max = 2147483647; // INT_MAX
    // [INT_MAX, INT_MAX, INT_MAX, INT_MAX]
    __m128i max_vals = _mm_set1_epi32(max); 
    // [1, 1, 1, 1]
    __m128i ones = _mm_set1_epi32(1);
    
    // In scalar C++, max + 1 is UB.
    // In SIMD, it is well-defined wrapping (2's complement).
    __m128i overflow = _mm_add_epi32(max_vals, ones);
    
    print_vec("Overflow", overflow);
    std::cout << "(Note: Wraps to -2147483648, NO SIGFPE, NO UB)" << std::endl;

    return 0;
}

// Build:
// Linux: g++ main.cpp -msse2 -o simd_int
// Windows (MSVC): cl main.cpp (SSE2 is default on x64)
