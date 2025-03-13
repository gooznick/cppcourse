// g++ main.cpp -fsanitize=undefined

#include <iostream>
#include <climits>

int ub_function() { }

int main() {
    int x; 
    std::cout << x << "\n"; 
    int arr[5];
    arr[10] = 42; 
    int* p = nullptr;
    *p = 5; 

    int max = INT_MAX;
    max++; 

    int* q = new int(42);
    delete q;
    std::cout << *q << "\n";

    return ub_function();
}
