#include <iostream>
#include <immintrin.h>
#include <limits>

void print_vec(const char* label, __m128i v) {
    alignas(16) int32_t values[4];
    _mm_store_si128((__m128i*)values, v);
    std::cout << label << ": [ ";
    for(int i=0; i<4; ++i) std::cout << values[i] << " ";
    std::cout << "]" << std::endl;
}

int main() {
    std::cout << "--- SIMD 'Integer' Division (via Float) ---" << std::endl;

    // [100, 100, 100, 100]
    __m128i a = _mm_set1_epi32(100);
    // [1, 2, 0, 4] -> Note the ZERO
    __m128i b = _mm_setr_epi32(1, 2, 0, 4);

    // 1. Convert to Float
    __m128 va = _mm_cvtepi32_ps(a);
    __m128 vb = _mm_cvtepi32_ps(b);

    // 2. Divide (Float Division by Zero = Inf)
    // No SIGFPE here!
    __m128 vres = _mm_div_ps(va, vb);

    // 3. Convert back to Int
    // Inf converts to 0x80000000 (INT_MIN)
    __m128i res = _mm_cvtps_epi32(vres);

    print_vec("Result", res);
    std::cout << "(Note: 3rd element is INT_MIN (0x80000000), representing Indefinite/Inf)" << std::endl;

    return 0;
}
