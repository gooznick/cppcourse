#include <iostream>
#include <vector>
#include <chrono>
#include <limits>
#include <xmmintrin.h> // For _MM_SET_FLUSH_ZERO_MODE
#include <pmmintrin.h> // For _MM_SET_DENORMALS_ZERO_MODE (SSE3)

// Benchmark function
// Uses a small buffer to ensure we measure ALU speed, not Memory Bandwidth (L1 Cache hit)
void run_benchmark(const char* label, float start_msg, float mul_factor) {
    const int size = 4096; // 16KB (Fits in L1 of most CPUs)
    std::vector<float> data(size, start_msg);

    auto start = std::chrono::high_resolution_clock::now();

    // Heavy compute loop
    // Performed enough times to be measurable
    volatile float sum = 0.0f; // Prevent optimizing away
    for (int k = 0; k < 10000; ++k) {
        for (int i = 0; i < size; ++i) {
            // Read-Modify-Write
            // If data[i] is Denormal, this is slow!
            data[i] = data[i] * mul_factor; 
            sum += data[i];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << label << ": " << elapsed.count() << "s" << std::endl;
}

int main() {
    float normal = 1.0f;
    // Create a denormal number (smallest float * 0.5)
    float denormal = std::numeric_limits<float>::min() * 0.5f;

    std::cout << "--- Performance Benchmark ---\n";
    std::cout << "Buffer Size: 16KB (L1 Cache)\n";
    std::cout << "Operations: Mul + Add\n\n";

    // 1. Normal Numbers
    run_benchmark("1. Normal Numbers", normal, 1.0000001f);

    // 2. Denormals (Can be 100x slower!)
    run_benchmark("2. Denormals     ", denormal, 1.0000001f);

    // 3. Enable FTZ (Flush to Zero)
    // This tells the CPU: "If you see a denormal, treat it as zero. Don't trap/microcode."
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
    
    std::cout << "\n[ Enabled FTZ (Flush-to-Zero) ]\n";
    
    // 4. Denormals with FTZ (Should be fast again)
    run_benchmark("3. Denormals + FTZ", denormal, 1.0000001f);

    return 0;
}
