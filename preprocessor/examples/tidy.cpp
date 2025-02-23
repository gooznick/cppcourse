#include <iostream>

void foo(int unusedParam) {
    int x = 42;
    int y = x;
    std::cout << "Value of y is " << y << std::endl;
}

int main() {
    int arr[3] = {1, 2, 3};
    
    // Old-style loop (modernize check)
    for (int i = 0; i < 3; i++) {
        std::cout << arr[i] << std::endl;
    }

    foo(0);  // unused parameter (misc-unused-parameters)
    
    return 0;
}
