#include <iostream>
#include <immintrin.h>  // for __m128 and intrinsics
#include <vector>
int g_int = 42;

int foo()
{
    static int a = 5;
    a += 10;
    return a;
}
int main()
{
    foo();
    __m128 vec;                
    vec = _mm_set_ps(1.0f,2.0f,3.0f,4.0f);

    int sum(0);
    for (int i=0;i<100;i++)
    {
        sum+=i;
    }
    std::vector<uint8_t> v(800,1);

    std::cout<<"Hello world ! Sum is : "<<sum<< std::endl;
    return 0;
}
