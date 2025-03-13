
// g++ main.cpp -O2

#include <iostream>

int main(){
    int c(0);
    int i = 0x10000000;
    do{
        i*=2;
        if ((c++) < 10) std::cout<<i<<std::endl;

    } while (i > 0);
    return 0;
}