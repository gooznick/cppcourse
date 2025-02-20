#!/bin/bash

# Create a simple C++ source file
cat > "simple.cpp" <<EOF
#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
EOF

# Compile to object file
# in msvc : cl /c simple.cpp /Fo:simple.obj
g++ -c simple.cpp -o simple.o

# in msvc : cl simple.obj /Fe:simple.exe
g++ simple.o -o simple


