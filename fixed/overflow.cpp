#include <iostream>
#include <climits>

int main() {
    unsigned int u = 0;
    std::cout << "Unsigned underflow: " << (u - 1) << "\n";
    
    int s = INT_MAX;
    std::cout << "Signed overflow: " << (s + 1) << "\n";
}