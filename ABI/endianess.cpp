#include <iostream>

int main() {
    unsigned int x = 0x12345678;
    unsigned char *byte = (unsigned char *)&x;

    if (byte[0] == 0x78) {
        std::cout<<"Little-Endian\n";
    } else if (byte[0] == 0x12) {
        std::cout<<"Big-Endian\n";
    }
}