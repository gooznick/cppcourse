
#include <iostream>

bool& a()
{
    static bool b = [](){bool c; std::cin>>c;return c;}();
    return b;
}

int main()
{
    return a();
}