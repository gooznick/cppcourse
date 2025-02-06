#include <iostream>

#define xstr(s) str(s)
#define str(s) #s
#define foo 4

int main()
{
     std:: cout<< str(foo) << std::endl;
     std:: cout<< xstr(foo) << std::endl;
}