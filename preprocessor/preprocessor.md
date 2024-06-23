---
title: preprocessor
author: eranbu
date: 6/2024
marp: true
---

# C/C++ Preprocessor

Some advanced usage tips

--- 

# `#include`

* Two kinds :
  * `#include <>` > `#include ""`
  * In most implementations `""` will look first in the local directory

* Pitfalls :
  * `#include <windows.h>`
  * `#include` order and independency
  * `#define` before `#include`
  * `#include <common_defs.h>`
  * `#include "c:\Users\davido\project\mordor.h`

---

# Definitions

```bash
g++ -DUSE_EIGEN  my_program.cpp
g++ -DUSE_EIGEN=1  my_program.cpp
```

```bash
cl /DUSE_EIGEN my_program.cpp 
cl /DUSE_EIGEN=1 my_program.cpp /
```

```cmake
target_compile_definitions(my_program PRIVATE USE_EIGEN)
target_compile_definitions(my_program PRIVATE USE_EIGEN=1)
```

---

# Debug Definitions

``` cpp
#ifdef DEBUG
#define LOG(x) std::cout << x << std::endl
#else
#define LOG(x)
#endif
```

* Debug definitions :
  * "NDEBUG" - standard for <assert.h>
  * "_DEBUG" - used in MSVC (using debug runtime)
  * "DEBUG" - commonly used

---

# Include guard

Old :
```cpp
#ifndef _MY_HEADER
#define _MY_HEADER

// bla bla
#endif
```

New :
```cpp
#pragma once
```

* Don't use within a cpp file

---

# Stringify

```cpp
#include <iostream>

#define xstr(s) str(s)
#define str(s) #s
#define foo 4

int main()
{
     std:: cout<< str(foo) << std::endl;
     std:: cout<< xstr(foo) << std::endl;
}
```

---

# Macro

* Avoid:
```cpp
#define SQUARE(x) x * x
#define ABS(x) (x < 0 ? -x : x)

void foo(int a){
  int b = SQUARE(a + 1); 
  int c = ABS(a++);
}
```

* Exception :
```cpp
#define DEBUG_PRINT(x) std::cout << "DEBUG: " << #x " = " << x << std::endl

void foo(int x)
{
  DEBUG_PRINT(x);
}
```
---

# Boost preprocess library

## Example : to_string

```cpp

enum Color{RED,GREEN,BLUE};

int main() {
    Color color = RED;
    std::cout << "Color: " << to_string(color) << std::endl;

    color = BLUE;
    std::cout << "Color: " << to_string(color) << std::endl;

    return 0;
}
```

---

# Boost preprocess library

```cpp
#include <boost/preprocessor.hpp>
#include <iostream>

// Define the enum entries
#define COLOR_ENUM_SEQ (RED)(GREEN)(BLUE)(YELLOW)

// Generate the enum definition
#define GENERATE_ENUM(r, data, elem) elem,

enum Color {
    BOOST_PP_SEQ_FOR_EACH(GENERATE_ENUM, _, COLOR_ENUM_SEQ)
};

// Generate the to_string function
#define GENERATE_STRING_CASE(r, data, elem) case elem: return BOOST_PP_STRINGIZE(elem);

const char* to_string(Color color) {
    switch (color) {
        BOOST_PP_SEQ_FOR_EACH(GENERATE_STRING_CASE, _, COLOR_ENUM_SEQ)
        default: return "Unknown";
    }
}
```

---

# Predefined 

* Standard :
  * __FILE__
  * __LINE__
  * __DATE__
  * __TIME__
  * __func__
  * __cplusplus__

* Non-standard:
  * _MSC_VER/__INTEL_COMPILER/__clang__/__INTEL_LLVM_COMPILER/__GNUC__

* Show all definitions:
```bash
echo | g++ -dM -E -x c++ -
```

--- 

# Some more `#pragma`s 

* #pragma omp
* #pragma warning
* #pragma pack()

---

# Debug

* Preprocess (no main!):
  * `g++ -E myfile.cpp`
  * `cl /P myfile.cpp`

* Look carefully at the definitions in the commandline.

* Add `#ifdef` - `#error` - `#endif`
