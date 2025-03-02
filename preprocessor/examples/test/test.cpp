#include <iostream>
#include <assert.h>

#include "tester.h"

class Algorithm
{
public :
    Algorithm(int v):_v(v) {}

    void Add(int n)
    {
        _v += n;
    }

    void Print()
    {
        std::cout << _v << std::endl;
    }

private:
    int _v;

};

void test1()
{
    Algorithm a(10);
    a.Add(5);
    assert(a._v == 15);
}


int main()
{
    test1();
}