#include <iostream>
#include <stdint.h>


int main()
{
    const size_t SIZE(100);
    uint64_t total(0);
    while (true)
    {
        uint8_t* a = new uint8_t[SIZE];
        total+=SIZE;
        std::cout << "Leak of: " << total << " bytes" << std::endl;
    }
    return 0;
}
