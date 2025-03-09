// g++ main.cpp -fsanitize=undefined

#include <iostream>
#include <climits>
int ub_function() { } // ❌ UB: No return in non-void function

int main() {
    int x; 
    std::cout << x << "\n";  // ❌ UB: Uninitialized variable

    int arr[5];
    arr[10] = 42;  // ❌ UB: Out-of-bounds access

    int* p = nullptr;
    *p = 5;  // ❌ UB: Dereferencing null pointer

    int max = INT_MAX;
    max++;  // ❌ UB: Signed integer overflow

    int* q = new int(42);
    delete q;
    std::cout << *q << "\n"; // ❌ UB: Use-after-free

    return ub_function(); // ❌ UB: No return value
}
