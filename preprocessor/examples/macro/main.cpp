#include <iostream>

#define SQUARE(x) x * x
#define ABS(x) (x < 0 ? -x : x)

int main()
{
    int a = 1;
    std::cout<<"a="<<a<<std::endl;
    std::cout<<"SQUARE(a)="<<SQUARE(a + 1)<<std::endl;
    std::cout<<"ABS(a)="<<ABS(a++)<<std::endl;
    std::cout<<"a="<<a<<std::endl;
    return 0;
}  