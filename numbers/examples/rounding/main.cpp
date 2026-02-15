#include <iostream>
#include <cmath>
#include <cfenv>
#include <iomanip>

// A function that changes rounding mode but forgets to restore it!
void bad_library_function() {
    std::cout << "[Bad Lib] Changing rounding to Downward..." << std::endl;
    std::fesetround(FE_DOWNWARD);
}

void print_rounding_mode() {
    int round = std::fegetround();
    std::cout << "Current Rounding: ";
    if (round == FE_TONEAREST) std::cout << "Nearest";
    else if (round == FE_DOWNWARD) std::cout << "Downward";
    else if (round == FE_UPWARD) std::cout << "Upward";
    else if (round == FE_TOWARDZERO) std::cout << "Toward Zero";
    else std::cout << "Unknown";
    std::cout << std::endl;
}

void print_calc(const char* label) {
    // 1.0 / 3.0 = 0.3333...
    // Nearest: 0.333333343 (float)
    // Down:    0.333333313 (float)
    
    // Use volatile to prevent constant folding
    volatile float a = 1.0f;
    volatile float b = 3.0f;
    volatile float val = a / b;
    
    std::cout << label << ": 1.0f/3.0f = " << std::setprecision(9) << val << std::endl;
    
    // Nearest integer rounding check
    // 2.9 -> 3 (Nearest), 2 (Down)
    std::cout << label << ": rint(2.9) = " << std::rint(2.9) << std::endl;
}

int main() {
    // Default is usually FE_TONEAREST
    print_rounding_mode();
    print_calc("Before");

    bad_library_function();

    // Now our own calculations are affected!
    print_rounding_mode();
    print_calc("After ");

    return 0;
}
