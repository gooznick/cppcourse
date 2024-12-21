struct MyStruct {
    char a;    // 1 byte
    int b;     // 4 bytes
    short c;   // 2 bytes
};

#include <iostream>

int main() {
    std::string a;
    std::cout<<"Size of struct:" << sizeof( MyStruct) << std::endl;
    std::cout<<"Offset of a   :" << offsetof( MyStruct, a) << std::endl;
    std::cout<<"Offset of b   :" << offsetof( MyStruct, b) << std::endl;
    std::cout<<"Offset of c   :" << offsetof( MyStruct, c) << std::endl;
    std::cout<<"Size of long double :" << sizeof( long double ) << std::endl;
    std::cout<<"Size of double:" << sizeof(  double ) << std::endl;

    
    return 0;
}