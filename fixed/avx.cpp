#include <immintrin.h>
#include <iostream>
int main() {
    __m128 a = _mm_set1_ps(10.0f);
    __m128 b = _mm_set1_ps(0.0f);
    __m128 c = _mm_div_ps(a, b);
    float result[4];
    _mm_storeu_ps(result, c);
    std::cout << "SSE result: " << result[0] << "\n";
}