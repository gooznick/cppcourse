// g++ main.cpp -g -omain_g
// g++ main.cpp -O3 -g -omain_O3_g
// g++ main.cpp -O3 -omain_O3
// g++ main.cpp -omain
#include <iostream>

void hello() {
    int x = 42;
    std::cout << "Hello! " << x << std::endl;
}

int main() {
    hello();
    return 0;
}
