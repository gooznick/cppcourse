#include <iostream>

#define SQUARE(x) x * x
#define ABS(x) (x < 0 ? -x : x)

void foo(int a){
  int b = SQUARE(a + 1); 
  int c = ABS(a++);
}

int main()
{
    std::cout<< "SQUARE(2 + 1) = " << SQUARE(2 + 1);
    return 0;
}