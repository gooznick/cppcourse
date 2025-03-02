---
title: preprocessor
author: eranbu
date: 6/2024
marp: true
theme: gaia
---

<!--
Preparations :
  vscode :     "C_Cpp.errorSquiggles": "disabled",
-->

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
Other default include directories :

g++:
echo | gcc -E -Wp,-v -

msvc:
echo %INCLUDE%
-->

--- 

# Hierarchy in `#include ""`

<br/>
<img src="images/hierarchy.png"  width="200" />


hierarchy.cpp : `#include <1/1.h>`
1.h : `#include <2/2.h>`
2.h : `#include "1b.h"`

<!--

msvc:
"" searches back in the #include hierarchy

g++:
does not 
-->
---

# Hierarchy in `#include ""`

<br/>
<img src="images/hierarchy_g++.png"  width="700" />
<img src="images/hierarchy_msvc.png"  width="700" />

---

# Include files names

```cpp
#include "string"

int main()
{
    std::string str("Hello World");
    return 0;

}
```

<!--
clash

has string file in directory.
use <>, dont use common names, use prefix or directory (proj/string)

-->


---

```cpp
#include "1\foo.h" // defines foo

int main()
{
    foo();
    return 0;
}
```
<br/>
<br/>

<img src="images/plat_g++.png"  width="700" />


<!--
Same with lowercase and uppercase (windows ignores)
-->

---

```cpp
#include "a.h" // defines a()


int main() {
    a();
    return 0;
}
```

```cpp
In file included from a.h:3,
                 from main.cpp:1:
b.h: In function ‘int sum_a_and_b()’:
b.h:13:16: error: ‘a’ was not declared in this scope
   13 |     return b()+a();
```
<!--
must show whole example...

Circular dependency.
-->

---

# Tracking API
<br/>

<img src="images/tracking.jpeg"  width="500" />


<!--

g++ main.cpp -Itracker -Itracker/manager -Itracker_manager

The api should be flat.
How to flatten ? 
"manager.h" : `#include "../../manager/manager.h"`
-->


---

# Warning source (msvc)


<br/>

<img src="images/endian_msvc.png"  width="800" />


---

# Slow compilation


```cpp
int main() {
    using namespace boost::multiprecision;

    cpp_dec_float_100 big_float = fibonacci<cpp_dec_float_100>(10);
    std::cout << "Fibonacci(10) as decimal: " << big_float << '\n';

    return 0;
}
```


---

⚠ **Common pitfalls:**
- Ensure headers are **self-contained**
- **Avoid contamination** - Do not define macros or `using namespace` in header file.
- **Don't use contaminating headers in headers** - `#include <windows.h>`
- **Avoid using common names** - `#include <common_defs.h>`
- Do not use **absolute paths** - `#include "c:\Users\davido\project\mordor.h"`
- Do not use **Uppercase** or **Slash**
- Don't **force include** or use **precompiled headers**.
<!--
-->

--- 

⚠ **Some flags:**

* Show includes:
  * `g++ -H` / `cl /showIncludes` / `C/C++ -> Advanced -> Show Includes`
* Include directories:
  * `-I` / `/I` / `C/C++ -> General -> Additional Include Directories`
* Internal include directories:
  * `echo | gcc -E -Wp,-v -`
  * `echo %INCLUDE%`

--- 

⚠ **Include what you use:**

```bash
mkdir build
cd build
cmake ..
iwyu_tool -p compile_commands.json
iwyu_tool -p compile_commands.json > iwyu.txt
cat iwyu.txt | fix_include 
```

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
<br/>

<img src="images/corrections.png"  width="300" />

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

[MSVC](images/msvc_macros.pdf)

<!--
echo | g++ -dM -E -x c++

https://learn.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-170
-->

---


# Debug Definitions

``` cpp
#ifdef DEBUG
std::cout << "The value of x is " << x << std::endl
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

<!--
not standard !
-->


---

# Stringify

```cpp
#define xstr(s) str(s)
#define str(s) #s
#define foo 4.0

int main()
{
     std:: cout << foo << std::endl;
     std:: cout << str(foo) << std::endl;
     std:: cout << xstr(foo) << std::endl;
     return 0;
}
```

<!--
4
foo
4.0
-->


---

# Macro

* Avoid:
```cpp
#define SQUARE(x) x * x
#define ABS(x) (x < 0 ? -x : x)

void main(){
    int a = 1;
    std::cout<<"a="<<a<<std::endl;
    std::cout<<"SQUARE(a)="<<SQUARE(a + 1)<<std::endl;
    std::cout<<"ABS(a)="<<ABS(a++)<<std::endl;
    std::cout<<"a="<<a<<std::endl;
}
```

* Exception :
```cpp
#define DEBUG_PRINT(x) std::cout << "DEBUG: " << #x " = " << x << "in line" << __LINE__ << std::endl

void foo(int x)
{
  DEBUG_PRINT(x);
}
```

<!--
macro/main.cpp
macro/good.cpp
-->

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


<!--
color/color.cpp
-->

---

# Predefined 

* Standard :
  * `__FILE__`  `__LINE__`  `__func__`
  * `__DATE__`  `__TIME__`
  * `__cplusplus` → **C++ standard version**

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

# 🛠️ Debugging

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


# 🛠️ Defined - 1


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

# 🛠️ Defined - 2


```cpp
#include <string>
#include <iostream>

std::string is_unix(bool unix){return unix?"yes":"no";}

int main()
{

    std::cout<<"Is Unix: "<<is_unix(true)<<std::endl;
    
    return 0;
}
```

```
main2.cpp:4:26: error: expected ‘,’ or ‘...’ before numeric constant
    4 | std::string is_unix(bool unix){return unix?"yes":"no";}
```

<!---
#define NOMINMAX 
windows defines min and max
(it defines IN and OUT too)
-->


---

# 🛠️  Words to Avoid 

| Category | Examples |
|----------|---------|
| **C Standard Macros** | `NULL`, `TRUE`, `FALSE`, `EOF` |
| **OS-Specific Macros** | `unix`, `linux`, `WIN32` |
| **Windows API Conflicts** | `IN`, `OUT`, `min`, `max` |
| **STL Conflicts** | `index`, `count`, `list`|
| **Reserved Names** | `find`, `erase`, `_MyVar`, `__my_var` |



---


# 🎯 Summary  

✔️ **Choose header names and directories wisely**  
✔️ **Know how to preprocesses a file**  
✔️ **Understand `#include` pitfalls**  
✔️ **Use macros wisely**  
✔️ **Use Boost Preprocessor**  


---

# 🎉 Questions?  