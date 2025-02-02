#include <iostream>
#include <cstddef>
int main() {
    int arr[10];
    size_t sz = sizeof(arr);
    ptrdiff_t diff = &arr[5] - &arr[0];
    std::cout << "Array size: " << sz << " bytes\n";
    std::cout << "Pointer difference: " << diff << " elements\n";
}