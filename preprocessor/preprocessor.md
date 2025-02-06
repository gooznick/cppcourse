---
title: preprocessor
author: eranbu
date: 6/2024
marp: true
theme: gaia
---

# C/C++ Preprocessor

Some advanced usage tips

--- 

# `#include`

* Two kinds :
  * `#include <>` > `#include ""`
  * In most implementations `""` will look first in the local directory

--- 

⚠ **Common pitfalls:**
- Ensure headers are **self-contained**
- **Avoid contamination** - Do not define macros or `using namespace` in header file.
- **Beware of contamination** - `#include <windows.h>`
- **Avoid using common names** - `#include <common_defs.h>`
- Do not use **absolute paths** - `#include "c:\Users\davido\project\mordor.h"`

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
  * "NDEBUG" - standard (for <assert.h>)
  * "_DEBUG" - used in MSVC (using debug runtime)
  * "DEBUG" - commonly used

---

# Include guard

Old :
```cpp
#ifndef _MY_HEADER
#define _MY_HEADER
#endif
```

New :
```cpp
#pragma once
```

* Don't use within a cpp file
* If file can be #included more than once, use `.inc` extension

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

# Predefined 

* Standard :
  * `__FILE__`  `__LINE__`  `__func__`
  * `__DATE__`  `__TIME__`
  * `__cplusplus` → **C++ standard version**

---

# Predefined 

* Non-standard:
  * `_MSC_VER/__INTEL_COMPILER/__clang__/__INTEL_LLVM_COMPILER/__GNUC__`

* Show all definitions:
```bash
echo | g++ -dM -E -x c++ -
```

--- 

# ⚡ More `#pragma` Directives  

* #pragma omp
* #pragma warning(disable: 4996) 
* #pragma warning
* #pragma pack()

* #pragma GCC optimize ("O3")  
* #pragma GCC poison free   

---

# 🛠️ Debugging Preprocessor Output  

**Preprocess the code (without compiling)**:
gcc:
```bash
g++ -E myfile.cpp
```

msvc:
```bash
cl /P myfile.cpp
```

---

# 🛠️ Debugging Macros  


**Check command-line definitions:**
- Use `#ifdef` + `#error` to debug:
```cpp
#ifdef MY_MACRO
#error "MY_MACRO is defined!"
#endif
```
🔹 **Compilation will stop, and the error will be shown.**

---

# 🛠️ Debugging - 1

```cpp
#include <gdal.h>

int main()
{
  return 1;
}
```

`undefined reference to 'main'`

<!---
#define main Main
-->

---

# 🛠️ Debugging - 2

```cpp
#include <Slike.h>

int main()
{
  return 1;
}
```

Fails only in linux :
`fatal error: Slike.h: No such file or directory`

<!---
upper/lower case
-->

---

# 🛠️ Debugging - 3

```cpp
#include <algorithm>
#include <Windows.h>

bool is_odd(int IN){return IN % 2 == 1;}

int main()
{

    int k = std::min(3, 4);
    return 0;
}
```

```
error C2589: '(' : illegal token on right side of '::'
error C2059: syntax error : '::'
```

<!---
#define NOMINMAX 
windows defines min and max
(it defines IN and OUT too)
-->

---

# 🛠️ Debugging - 4

Compiling :
```cpp
#include <a.h>
#include <b.h>
```

Not compiling :
```cpp
#include <b.h>
#include <a.h>
```

<!---
Check contamination.
Fwd declaration.
-->

---

# 🛠️ Debugging - 5

```cpp
// A.h
#pragma once
#include "B.h"  
struct A {
    B* obj;  
};
```

```cpp
// B.h
#pragma once
#include "A.h"  
struct B {
    A* obj;  
};
```

<!---
error: ‘A’ does not name a type
fwd declaration.
-->

---


# 🎯 Summary  

✔️ **Choose header names and directories wisely**  
✔️ **Know how to preprocesses a file**  
✔️ **Understand `#include` pitfalls**  
✔️ **Use macros wisely**  
✔️ **Use Boost Preprocessor**  


---

# 🎉 Questions?  