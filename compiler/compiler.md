---
title: compiler
author: eranbu
date: 3/2024
marp: true
theme: gaia

---

![bg left width:500px](images/compiler.png)

# Compiler 🧙‍♂️

A **compiler** is a grumpy wizard that grudgingly transforms your poetic code into **machine gibberish**, while delighting in pointing out every tiny mistake you make.

---

# Optimization

<img src="images/dragons.webp" alt="Local Image" width="800"  />

---

# 🔧 Optimization Tricks

✨ **Magical Optimizations:**

- Code Reordering 🌀
- Function Inlining ⚡
- Dead Code Elimination ☠️
- Register Allocation 📦
- Loop Unrolling 🔄
- Debug Code Removal 🚫
- Constant Folding 🧮
- Code Merging 🏗️

---

# 📝 Example Code

```cpp
#include <stdint.h>

uint64_t foo()
{
    const int N(1024);
    uint64_t total(0);
    for (int i=0;i<N;++i)
        total+=i*i;
  return total;
}
```

<!--
sum of squares from 0 to 1023.
-->

---

# 🐌 No Optimization

```asm
_Z3foov:
 push   rbp
 mov    rbp,rsp
 mov    DWORD PTR [rbp-0x4],0x400
 mov    QWORD PTR [rbp-0x10],0x0
 mov    DWORD PTR [rbp-0x14],0x0
 cmp    DWORD PTR [rbp-0x14],0x400
 jge    46 <_Z3foov+0x46>
 mov    eax,DWORD PTR [rbp-0x14]
 imul   eax,DWORD PTR [rbp-0x14]
 cdqe
 add    rax,QWORD PTR [rbp-0x10]
 mov    QWORD PTR [rbp-0x10],rax
 mov    eax,DWORD PTR [rbp-0x14]
 add    eax,0x1
 mov    DWORD PTR [rbp-0x14],eax
 jmp    1a <_Z3foov+0x1a>
 mov    rax,QWORD PTR [rbp-0x10]
 pop    rbp
 ret
```

---

# 🚀 Max Optimization


```asm
_Z3foov:
 mov    eax,0x154d5600
 ret
```

---

# 🛠️ Optimization Notes

- Debug symbols can **coexist** with optimized code 🧐
- Partial optimization is possible (e.g., per file)

<!--
show how to remove optimization from a single file
visual studio
-->


---

# 🌍 Language Evolution

![Image](images/versions.png)

---

# ⚡ Modern C++ Features


```cpp
#include <iostream>
#include <ranges>

int main() {
    auto squares = std::views::iota(1, 11) | std::views::transform([](int n) { return n * n; });
    for (int n : squares | std::views::filter([](int n) { return n % 2 == 0; })) 
        std::cout << n << " "; 
    return 0;
}
```

`g++ --std=c++20`


<!---
4 16 36 64 100
-->

---

# 📜 The C++ Standard

<img src="images/bible.png" alt="Local Image" width="400"  />

---


# Undefined Behavior (UB)

<img src="images/ub.jpeg" alt="Local Image" width="200"  />

- **Behavior not defined by C++** 🚨
- Compiler **can do anything** 🤯
- UB may cause:
  - Crashes 💥
  - Silent data corruption 🕵️‍♂️

💀 **Avoid UB at all costs!**

---

# Common Types of Undefined Behavior

```cpp
// ❌ 1. Division by zero (UB: Crash or unexpected result)
int x = 42;
int y = x / 0; 

// ❌ 2. Out-of-bounds array access (UB: Overwriting random memory)
int arr[5];
arr[10] = 7; 

// ❌ 3. Dereferencing null or invalid pointer (UB: Segmentation fault)
int* p = nullptr;
*p = 5; 

// ❌ 4. Using an uninitialized variable (UB: Garbage value or crash)
int a;
int b = a + 1; 
```

---

```cpp
// ❌ 5. Use-after-free (UB: Accessing freed memory)
int* ptr = new int(10);
delete ptr;
std::cout << *ptr; 

// ❌ 6. Signed integer overflow (UB: Compilers assume it NEVER happens!)
int max = INT_MAX;
int result = max + 1; // UB: Wraparound not guaranteed in signed integers

// ❌ 7. Non-void function missing a return statement (UB: Can cause random behavior)
int brokenFunction() {
    // No return statement! UB if this function is called.
}

// ❌ 8. Type punning (UB: Violates strict aliasing rules)
float f = 1.5f;
int* ip = (int*)&f; 
```

<!---
ub_return

-->

---

# 🛠️ Optimization Flags  

| **Flag** | **MSVC** | **GCC** | **Effect** |
|---------|-----------------|-----------------|------------|
| **None (Debug Mode)** | `/Od` | `-O0` | No optimizations |
| **Basic Optimizations** | `/O1` | `-O1` | Optimize for size & speed |
| **Full Optimizations** | `/O2` | `-O2` | Optimize aggressively |
| **Max Optimizations** | `/Ox` | `-O3` | Highest optimization level |
| **Optimize for Size** | `/Os` | `-Os` | Optimize for small binaries |

---

# 🚀 Debugging optimized code

✔ **Add debug information** (`/Zo /Zi`, `-g`) 
✔ **Don't touch fp !** (`/Oy`,`-fno-omit-frame-pointer`) 
✔ **No inlining** (`-fno-inline`) 

<!---
The -fno-omit-frame-pointer flag preserves the frame pointer (rbp) in function calls

-->

---


# ⚠️ Warning & Debugging Flags  

| **Flag** | **MSVC** | **GCC** | **Effect** |
|---------|-----------------|-----------------|------------|
| **Enable All** | `/W4` | `-Wall` | Show most warnings |
| **Extra Warnings** | `/Wall` | `-Wextra` | More strict checking |
| **Treat as Errors** | `/WX` | `-Werror` | Warnings become errors |
| **Debug Info** | `/Zi` | `-g` | Enable debugging symbols |

---

# 🚀 More Flags?  

✔ **Standard Selection** (`/std:c++17`, `-std=c++20`)  
✔ **Multithreading Flags** (`/openmp`, `-fopenmp`)  
✔ **Allows larger object files** (`/bigobj`)  
✔ **Position-independent code** (`-fPIC`) 


---

# Single file flags

```cmake
add_executable(my_program main.cpp special.cpp other.cpp)

set_source_files_properties(special.cpp PROPERTIES COMPILE_FLAGS "-O3")
```

<!---
single

-->


---

# Godbolt


<img src="images/godbolt.png" alt="Local Image" width="1000"  />

---

# 🛠️ Debugging Compilation Errors

* Compile **one file at a time** 📝
  * `make VERBOSE=1` / `cmake --build . -- VERBOSE=1`
  * `compile_commands.json`

* Start with the **first error** 🚨

* Try a **different compiler**.


---

```cpp
#include <iostream>

void printHello() {
    std::cout << "Hello, world!" << std::endl;
}

int main() {
    printHe1lo(); 
    return 0;
}
```

🔹 **Error: `undefined reference`**

<!---
undefined reference 
-->

---

```cpp
#include <iostream>

int main() {
    char hello[5] = "hello";
    std::cout<<hello;
}
```

<!---
 error: initializer-string for char array is too long
 -->


---

```cpp
#include <algorithm>

uint64_t square(uint32_t i) {
    return i*i;
}
```

<!---
'uint64_t' does not name a type
 -->

---

```cpp
class A
{
    A(){};
}

A a;
```

```
expected initializer before 'a'
```

<!---
A::A()' is private within this context```
-->


---

```cpp
 struct A
    {
        int a() {return 1;}
    };

void foo(const A& a){
    a.a();
}
```

```
error C2662: 'int A::a(void)': cannot convert 'this' pointer from 'const A' to 'A &'
note: Conversion loses qualifiers
```

<!---
A::a()' has single parameter - this. "this" in foo is const, it cannot change to non const.
-->

---

```cpp
int main()
{
    return 1;
}
```

```
fatal error C1010: unexpected end of file while looking for precompiled header. Did you forget to add '#include ""' to your source?
```
<!---
/Yu
-->

---

```cpp
#include <stdio.h>

void foo(int answer)
{
    printf( “The answer is %d\n”,answer );
}
```

```
5:13: error: stray '\342' in program
     printf( ���The answer is %d\n”,answer );
             ^
5:14: error: stray '\200' in program
     printf( ���The answer is %d\n”,answer );
              ^
5:15: error: stray '\234' in program
     printf( ��The answer is %d\n”,answer );
               ^
5:32: error: stray '\' in program
     printf( “The answer is %d\n”,answer );
                                ^
5:34: error: stray '\342' in program
     printf( “The answer is %d\n���,answer );
                                  ^
5:35: error: stray '\200' in program
     printf( “The answer is %d\n���,answer );
                                   ^
5:36: error: stray '\235' in program
     printf( “The answer is %d\n��,answer );
                                    ^
 In function 'void foo(int)':
5:16: error: 'The' was not declared in this scope
     printf( “The answer is %d\n”,answer );
                ^~~
```

---

```cpp
enum BoolType
{
  FALSE = 0,
  TRUE = 1
};
```
---

# Tool

<img src="../images/multitool.png" width="300" />

--- 


# 🔍 Disassembly Tools for Debugging

Disassembling compiled files helps in:

- **Understanding compiler optimizations** 🛠️
- **Debugging crashes & undefined behavior** 🚨
- **Reverse engineering unknown binaries** 🕵️‍♂️
- **Analyzing performance bottlenecks** 🚀

**Common File Types:**

- **Windows:** `.obj`, `.exe`
- **Linux:** `.o`, `elf file`

---

# 🛠️ Objdump & Dumpbin

| **Tool**     | **Platform** | **Purpose** |
|-------------|-------------|------------|
| `objdump`  | Linux    | Disassemble ELF `.o`, `ELF` files 🐧 |
| `dumpbin`  | Windows     | Disassemble `.obj`, `.exe` files 🏁 |

---

# 📌 Example Usage for Debugging

```bash
objdump -d my_program    # Disassemble object file
objdump -t my_program    # Show symbol table
```

```cmd
dumpbin /DISASM my_program.obj   # Disassemble object file
dumpbin /SYMBOLS my_program.exe  # View symbol table
dumpbin /HEADERS my_program.exe  # Inspect binary headers
```

---

**Use Case:**
- Check if compiler optimized out variables or functions.
- Inspect function call ordering in the binary.
- Verify symbol visibility & linkage issues.

---


![bg](images/dont.webp)

<!-- 
Bad practice
-->

---

# Prevent UB in your code

<img src="images/compiler_complaint.png"  width="1200"  />


---
# 🚨 Sanitizers

💡 **Sanitizers (UBSan, ASan, MSan)** detect dangerous runtime issues like:
- 🔥 **UB: Signed overflows, invalid pointer derefs**
- 💾 **Memory errors: Out-of-bounds, use-after-free**
- ⚡ **Thread & race conditions (TSan)**

---

# 🛠️ How Do They Work?

🔍 **Compile-time instrumentation** adds checks
🐞 **Runtime validation** detects UB
⚠️ **Crashes or warnings** when UB happens

```bash
# G++/Clang
-fsanitize=undefined,address,thread,memory

# MSVC
/fsanitize=address
```

---

# 🛠️ How Do They Work?


```cpp
#include <climits>

int main() {
   int max = INT_MAX;
    max++;  //  UB: Signed integer overflow

    return 0;
}
```

---

# 🛠️ How Do They Work?

<img src="images/godbolt_ubsan.png"  width="1000"  />

---

