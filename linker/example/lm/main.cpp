// gcc main.cpp 
//
// /usr/bin/ld: /tmp/ccOqNGeM.o: in function `main':
// main.cpp:(.text+0x23): undefined reference to `exp'
// collect2: error: ld returned 1 exit status

// with g++ in links fine.

#include <math.h>  // C math header
#include <stdio.h>

int main() {
    double x = 1.23;
    double y = exp(x);  // <- C math function, no std::exp
    printf("exp(%f) = %f\n", x, y);
    return 0;
}
