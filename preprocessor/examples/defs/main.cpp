#include "add_one.h"

#include <iostream>

int main()
{
    Data data;
    data.value = 10;

    std::cout<<"data.value="<<data.value<<std::endl;
    add_one(data);
    std::cout<<"data.value="<<data.value<<std::endl;

    return 0;
}


