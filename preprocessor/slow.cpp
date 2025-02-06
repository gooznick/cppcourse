#include <boost/multiprecision/cpp_dec_float.hpp>

template <typename T>
T fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci<T>(n - 1) + fibonacci<T>(n - 2);
}

int main() {
    using namespace boost::multiprecision;
    cpp_dec_float_100 big_float = fibonacci<cpp_dec_float_100>(10); // Use float precision
    return 0;
}