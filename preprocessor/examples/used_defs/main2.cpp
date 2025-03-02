#include <string>
#include <iostream>

std::string is_unix(bool unix){return unix?"yes":"no";}

int main()
{

    std::cout<<"Is Unix: "<<is_unix(true)<<std::endl;
    
    return 0;
}