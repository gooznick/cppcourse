// g++ -std=c++17 main.cpp 
// __cplusplus: 201703

// g++ -std=c++11 main.cpp 
// __cplusplus: 201103

#include <iostream>

int main() {
    std::cout << "__cplusplus: " << __cplusplus << std::endl;
    return 0;
}