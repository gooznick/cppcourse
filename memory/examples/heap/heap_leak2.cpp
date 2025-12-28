#include <vector>
#include <iostream>
#include <memory>

struct Base
{
};

struct Derived : Base
{
    std::vector<int> v;
    Derived() : v(1'000'000, 42) {}
};

int main()
{
    std::unique_ptr<Base> p(new Derived());
}