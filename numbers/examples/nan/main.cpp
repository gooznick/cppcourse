#include <iostream>
#include <limits>
#include <cmath>
#include <cfenv>
#include <iomanip>

#ifdef _WIN32
#include <float.h> // For _controlfp
#endif

// Helper to enable exceptions (Make it Crash!)
void enable_floating_point_exceptions() {
#ifdef _WIN32
    // Windows: Clear the mask bits to ENABLE exceptions.
    // _EM_INVALID is the bit for Invalid Operation (NaN usage)
    unsigned int current_word;
    _controlfp_s(&current_word, 0, 0); // Get current
    _controlfp_s(&current_word, current_word & ~_EM_INVALID, _MCW_EM);
    std::cout << "[Windows] FP Exceptions Enabled (_EM_INVALID unmasked)" << std::endl;
#elif defined(__linux__) || defined(__APPLE__)
    // Linux/macOS (glibc): generic feenableexcept
    // FE_INVALID handles 0/0 and sNaN
    // Note: Standard C++ doesn't have feenableexcept, it's a GNU extension.
    #pragma STDC FENV_ACCESS ON
    feenableexcept(FE_INVALID); 
    std::cout << "[Linux] FP Exceptions Enabled (feenableexcept FE_INVALID)" << std::endl;
#endif
}

void print_fe_exceptions() {
    std::cout << "Exceptions raised: ";
    if (std::fetestexcept(FE_INVALID)) std::cout << "FE_INVALID ";
    if (std::fetestexcept(FE_DIVBYZERO)) std::cout << "FE_DIVBYZERO ";
    if (std::fetestexcept(FE_OVERFLOW)) std::cout << "FE_OVERFLOW ";
    if (std::fetestexcept(FE_UNDERFLOW)) std::cout << "FE_UNDERFLOW ";
    if (std::fetestexcept(FE_INEXACT)) std::cout << "FE_INEXACT ";
    std::cout << std::endl;
}

int main() {
    // Clear exceptions
    std::feclearexcept(FE_ALL_EXCEPT);

    std::cout << "--- Quiet NaN (Default) ---" << std::endl;
    // Standard operations produce Quiet NaN
    double qnan = 0.0 / 0.0; 
    std::cout << "0.0 / 0.0 = " << qnan << std::endl;
    print_fe_exceptions(); // Should show FE_INVALID 
    std::feclearexcept(FE_ALL_EXCEPT);

    double r1 = qnan + 1.0; 
    std::cout << "qNaN + 1.0 = " << r1 << std::endl;
    print_fe_exceptions(); // Should show FE_INVALID 

    std::cout << "\n--- Signaling NaN ---" << std::endl;
    std::feclearexcept(FE_ALL_EXCEPT);
    // sNaNs are NOT produced by standard math. You must create them manually!
    double snan = std::numeric_limits<double>::signaling_NaN();
    
    // Theoretical behavior: Accessing snan triggers FE_INVALID.
    // Note: Default C++ behavior often ignores this unless FPU exceptions are enabled.
    // To see a crash/trap, you often need compiler flags or feenableexcept (GLIBC).
    double r2 = snan + 1.0; 
    std::cout << "sNaN + 1.0 = " << r2 << std::endl;
    print_fe_exceptions(); // Should show FE_INVALID

    // 3. Enabling Traps (Crash)
    std::cout << "\n--- Trapping (Crash) ---" << std::endl;
    std::cout << "Enabling hardware exceptions..." << std::endl;
    enable_floating_point_exceptions();

    std::cout << "Triggering sNaN use (Should CRASH/Signal now)..." << std::endl;
    double r3 = snan + 1.0; 
    std::cout << "Result: " << r3 << " (If you see this, it didn't crash!)" << std::endl;

    return 0;
}
