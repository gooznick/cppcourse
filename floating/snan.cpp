#include <iostream>
#include <cfenv>
#include <limits>

#pragma STDC FENV_ACCESS ON  // Ensures floating-point environment is accessible

int main() {
    feenableexcept(FE_INVALID);  // Enable invalid operation exception

    double sNaN = std::numeric_limits<double>::signaling_NaN();
    double result = sNaN + 1.0;  // This will now terminate the program

    std::cout << "Result: " << result << "\n";  // This line will never execute
}
