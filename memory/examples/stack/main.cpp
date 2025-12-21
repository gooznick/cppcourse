#include <iostream>
#include <stdint.h>


void measure_stack(int depth, uint8_t* prev_addr)
{
    volatile std::uint8_t marker = 0;
    std::cout << prev_addr - &marker << " bytes" << std::endl;

    if (depth == 0) return;

    measure_stack(depth - 1, (uint8_t*)&marker);
}

int main()
{
    measure_stack(5, nullptr);
    return 0;
}
