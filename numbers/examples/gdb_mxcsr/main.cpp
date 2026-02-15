#include <iostream>
#include <cfenv>
#include <cmath>

int main() {
    std::cout << "1. Default State (Nearest)" << std::endl;
    // Inspect here: 'info float' in GDB
    
    std::fesetround(FE_DOWNWARD);
    std::cout << "2. Changed to Downward Rounding" << std::endl;
    // Inspect here: 'info float' should show RC (Rounding Control) change
    
    float a = 1.0f;
    float b = 0.0f;
    // Uncommenting this would raise DIVBYZERO exception flag
    // float c = a / b; 
    
    std::cout << "Done." << std::endl;
    return 0;
}
