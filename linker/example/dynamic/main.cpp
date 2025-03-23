#include <iostream>

int pow(int,int);

int main()
{
    int a = 2;
    int b = 10;
    int c = pow(a, b);
    std::cout<<"2 ^ 10 = "<<c<<std::endl;
    return 0;
}