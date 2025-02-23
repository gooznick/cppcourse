---
title: preprocessor
author: eranbu
date: 6/2024
marp: true
theme: gaia
---

# C/C++ Preprocessor

Some advanced usage tips

![bg left width:600px](images/semicolon.webp)

<!--
linux : 
Ctrl+Shift+U → 037E → Enter
-->

--- 

# `#include`

* Two kinds :
  * `#include <>` > `#include ""`
  * In most implementations `""` will look first in the local directory

<!--
echo | gcc -E -Wp,-v -
-->

--- 

⚠ **Common pitfalls:**
- Ensure headers are **self-contained**
- **Avoid contamination** - Do not define macros or `using namespace` in header file.
- **Don't use contaminated headers in headers** - `#include <windows.h>`
- **Avoid using common names** - `#include <common_defs.h>`
- Do not use **absolute paths** - `#include "c:\Users\davido\project\mordor.h"`
- Do not use **Uppercase**

<!--
echo | gcc -E -Wp,-v -
-->

--- 

⚠ **Some tools:**

* Compile header only (include only cpp)
* Include What You Use
* clang-tidy

```bash
sudo apt install iwyu
include-what-you-use --version
sudo apt install clang-9
iwyu iwyu.cpp 2> iwyu.txt---
cat iwyu.txt | fix_include
```

```bash
# create .clang-tidy
clang-tidy-10 tidy.cpp -checks=*
sudo apt install clang-tidy
```

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

<!--
Do not use many definitions !

Check each definition, it's a different code !
-->

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

```
... all identifiers that contain a double underscore __ in any position and each identifier that begins with an underscore followed by an uppercase letter is always reserved,
```

* Non-standard:
  * `_MSC_VER/__INTEL_COMPILER/__clang__/__INTEL_LLVM_COMPILER/__GNUC__`

* Show all definitions:
```bash
echo | g++ -dM -E -x c++ -
```

```
Properties -> C/C++ -> Preprocessor
```

---

# Tool: clang-tidy

Ignore : 
```cpp
badcode;  // NOLINT

// NOLINTNEXTLINE
badcode;

// NOLINTBEGIN
badcode;
badcode;
// NOLINTEND

badcode; // NOLINT(cert-err-58-cpp)
```

--- 

# ⚡ `#pragma` Directives  

* #pragma omp
* #pragma warning(disable: 4996) 
* #pragma warning
* #pragma pack()

* #pragma GCC optimize ("O3")  
* #pragma GCC poison free   

<!--
The ISO C++ language standard does not require the compilers to support any pragmas. However, several non-standard pragmas are supported by multiple implementations

From the standard : 
A preprocessing directive of the form

# pragma pp-tokensopt new-line
causes the implementation to behave in an implementation-defined manner.
-->

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