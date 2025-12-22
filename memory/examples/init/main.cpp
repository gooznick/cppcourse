#include <iostream>
#include <new>      // placement new (optional)
#include <cstdlib>  // malloc/free
#include <cstring>  // memset

struct Foo {
    int x;          // intentionally uninitialized
    Foo() {}        // does NOT initialize x
};

// Global/static storage duration: zero-initialized before main()
Foo g_foo;

int main() {
    Foo s_foo;                // stack: indeterminate (often "garbage")
    Foo* h_foo = new Foo;     // heap: indeterminate, unless allocator/debug fills memory

    std::cout << "global g_foo.x = " << g_foo.x << "\n";
    std::cout << "stack  s_foo.x = " << s_foo.x << "\n";
    std::cout << "heap   h_foo->x = " << h_foo->x << "\n";

    delete h_foo;
}
