
// time g++ -std=c++17 -O2 slow.cpp -o slow_compile

// real    0m2.700s
// user    0m2.528s
// sys     0m0.170s

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iostream>

// A recursive Fibonacci calculation using Boost multiprecision types
template <typename T>
T fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci<T>(n - 1) + fibonacci<T>(n - 2);
}

int main() {
    using namespace boost::multiprecision;

    cpp_dec_float_100 big_float = fibonacci<cpp_dec_float_100>(10);
    std::cout << "Fibonacci(10) as decimal: " << big_float << '\n';

    return 0;
}
