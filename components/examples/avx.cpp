// g++ avx.cpp -mavx512f -oavx


#include <immintrin.h>
#include <iostream>

int main() {
    // Initialize two arrays of floats
    alignas(64) float a[16] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f,
                              9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
    alignas(64) float b[16] = {16.0f, 15.0f, 14.0f, 13.0f, 12.0f, 11.0f, 10.0f, 9.0f,
                              8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    alignas(64) float result[16];

    // Load data into AVX-512 registers
    __m512 vec1 = _mm512_load_ps(a);
    __m512 vec2 = _mm512_load_ps(b);

    // Perform vectorized addition using AVX-512
    __m512 vec_sum = _mm512_add_ps(vec1, vec2);

    // Store the result back to the array
    _mm512_store_ps(result, vec_sum);

    // Display the result
    for (int i = 0; i < 16; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
