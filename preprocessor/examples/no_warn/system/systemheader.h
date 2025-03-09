#pragma once 

#include <iostream>

inline void foo()
{
    int signed_value = -1;
    unsigned int unsigned_value = 1;

    if (signed_value < unsigned_value) {  
        std::cout<<"This should never happen!\n";
    }
}