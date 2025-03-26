---
title: The Linker
author: eranbu
date: 3.2025
theme: gaia
marp: true
---

<!---
Prepare  :
linux cmake 3.24

-->

![bg left width:500px](images/errors.webp)

# The Linker


---

# The Linker

* **Links object files into an executable or library**  
* Resolves **symbols** from different sources  
* Fixes **addresses** for functions & variables  
* Produces **executables, DLLs (Windows), SOs (Linux)**  

---


![bg left width:500px](images/demo.jpeg)

# Static Libraries

* `+`
* `*`
* `^`

---

# 🔗 Full Source Compilation
### 🧮 `add → mul → pow → main`
<br/>

```bash
g++ add.cpp mul.cpp pow.cpp main.cpp
```

- 🛠️ All `.cpp` files compiled and linked in one step
- ✅ No dependency order needed

---

# 🧱 Object File Linking
### 📦 Build objects first, then link
<br/>

```bash
g++ add.cpp -c
...
g++ add.o mul.o pow.o main.cpp
```

- ⚙️ Compile each source separately
- ✅ Still no link order issues

---

# 📚 Static Library Linking
### 🗂️ Libraries need correct order!
<br/>

```bash
g++ add.cpp -c
ar rcs libadd.a add.o
...

g++ main.cpp libpow.a libmul.a libadd.a
```

- ⚠️ Linker reads **left to right**
- ❗ Put libraries **after** the code that needs them

---


# 📚 Inspecting with `nm`
<br/>

```bash
nm libmul.a -C
```

```
mul.o:
                 U _GLOBAL_OFFSET_TABLE_
                 U add(int, int)
0000000000000000 T mul(int, int)
```

<!--
T is Text (code)
U is undefined

`-C` is demangle 
-->

---

# Link Stages: Windows

1. Object files (`.obj`) compiled  
2. Linker resolves **all symbols**  
3. Statically links `.lib` files  
4. Produces `.exe` or `.dll`

---

# Link Stages: Linux

1. Object files (`.o`) compiled  
2. Linker resolves symbols **in order**  
3. Links `.a` and `.so` files  
3. Produces `executable`

⚠ **Order matters in Linux!**  
Symbols must be **defined before use** in command:  
```bash
g++ main.o -lfoo -o app  # ❌ Undefined reference
g++ -lfoo main.o -o app  # ✅ Correct order
```

---


# ⚠️ **Undefined Reference**

- 🔍 Use `nm`, `grep` to find where it's defined
- 🧱 Check link command (`link.txt` in CMake)
- ➕ Ensure dependency is **after** its user
- 🛠 Fix link order or use `LINK_GROUP`

```bash
find . -type f -name '*.a' -exec nm -C {} + 2>/dev/null
```

<!---
order example

target_link_libraries(main PRIVATE "$<LINK_GROUP:RESCAN,add,mul>")

-->


---

| 🔍 Undefined Symbol                              | 📚 Missing Library or Flag         |
|--------------------------------------------------|------------------------------------|
| `pthread_create`, `pthread_join`                 | `-lpthread`                        |
| `boost::filesystem::path`      | `-lboost_filesystem` |
| `cv::Mat`       | `-lopencv_core`  |
| `dlopen`, `dlsym`, `dlclose`                     | `-ldl`                             |
| `std::thread`, `std::mutex`      | `-pthread`    |
| `zlibVersion`, `inflate`, `deflate`              | `-lz`                              |
| `curl_easy_init`, `curl_easy_setopt`             | `-lcurl`                           |
| `glBegin`, `glVertex3f`, `glEnd`                 | `-lGL` (OpenGL)                    |
| `glutInit`, `glutCreateWindow`                   | `-lglut`                           |


---

# 🧱 Which libraries will be linked ?

---

## 🐧 Linux Flags

- `-L<dir>` — Add search path  
- `-l<name>` — Link `lib<name>.so` / `.a`  
- `-l:filename.a` — Exact archive

<!---
```sh
g++ -L. -lmylib
g++ -L. -l:libmylib.a
```
-->

---

## 🪟 Windows (MSVC)

- Linker -> Input -> Additional Dependencies
- `#pragma comment(lib, "mylib.lib")` in code

---

## 📦 CMake: Finding Libraries

```cmake
add_library(mylib STATIC file.cpp)
target_link_libraries(app PRIVATE mylib)
```

- `target_link_libraries()` connects targets

---


![bg left width:500px](images/library.jpg)

# Dynamic Libraries

* `+`
* `*`
* `^`

---


|                | 🧱 Static (`.a`, `.lib`)         | 🔗 Dynamic (`.so`, `.dll`)          |
|----------------|----------------------------------|-------------------------------------|
| Linked         | At **compile/link** time         | At **runtime**                      |
| Included in    | Final executable     | External file     |
| Size           | Larger executable                | Smaller executable                  |
| Flexibility    | Less (update=rebuild)         | More (swap `.dll`/`.so`)            |
| Dependencies   | None at runtime                  | `.so`/`.dll` must be present        |

💡 Use **static** if you can.

---

# 🪟 Create a DLL (Windows)

- 🛠 Change `CMakeLists.txt`:
```cmake
add_library(add SHARED add.cpp)
```

- 🧩 Mark functions for export:
```cpp
__declspec(dllexport)
int add(int a, int b) {
  return a + b;
}
```

---

# 📦 Portable Export Macro

```cpp
  #ifdef MYLIB_EXPORTS
    #define MYLIB_API __declspec(dllexport)
  #else
    #define MYLIB_API __declspec(dllimport)
  #endif


MYLIB_API int add(int a, int b);
```

```cmake
target_compile_definitions(mylib PRIVATE MYLIB_EXPORTS)
```
---

# 📚 Linking with a DLL (Windows)

- 🧱 Link against `.lib` (import library)
- 🔗 Loads `.dll` at runtime

### 🔍 DLL Search Order:
1. Executable directory  
2. Current directory  
3. System `PATH`  

<!---
No lib will be created if there are no __dllexport functions !
-->

---



# 🐧 Create a `.so` (Linux)

- 🛠 Change `CMakeLists.txt`:
```cmake
add_library(add SHARED add.cpp)
```

- 🧩 No need for `__declspec(dllexport)`

```cpp
int add(int a, int b) {
  return a + b;
}
```

---

# Link Stages: Linux

When linux creates an `so` file it does not link it!

Missing symbols will be raised only when linking the executable.

The `--no-undefined` flag changes that:
```cmake
add_library(pow SHARED pow.cpp)

set_target_properties(pow PROPERTIES
  LINK_FLAGS "-Wl,--no-undefined"
)
target_link_libraries(pow PRIVATE mul add)
```


---

# 📚 Linking with a `.so` (Linux)

- 🧱 Link with `-ladd`
- Provide `.so` at runtime

### 🔍 `.so` Search Order:
1. `DT_RPATH` (Deprecated)
1. `LD_LIBRARY_PATH`  
1. `DT_RUNPATH` 
2. `/etc/ld.so.cache`  


---

## CMake

By default :
* CMake **adds** run path to the executable
* CMake **removes** the run path when installing

Recommended :

```cmake
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

set_target_properties(main PROPERTIES
  INSTALL_RPATH "$ORIGIN"
  SKIP_BUILD_RPATH ON
)
```
---

# 🔧 CMake **Global** vs **Project**

- Global
```cmake
set(CMAKE_SKIP_BUILD_RPATH TRUE)
```

- More fine-grained control
```cmake
set_target_properties(main 
  PROPERTIES
  SKIP_BUILD_RPATH TRUE
)
```

---

# Tool

<img src="../images/multitool.png" width="300" />


--- 

# Dependency walker

* https://www.dependencywalker.com/
* https://github.com/lucasg/Dependencies

<!-- 
Some more tools :

procmon - windows error message will tell you the dll problem and not the dependency

-->

---

# 🔍 Runtime Linker Debugging Tools


✅ **Linux:** `LD_DEBUG`, `LD_PRELOAD`
✅ **Windows:** `procmon`

---

# 🛠️ `LD_DEBUG`

🔹 Debug **dynamic linker activity** 🏗️  
🔹 Show **symbol resolution, library loading** 🕵️‍♂️  

```bash
LD_DEBUG=all ./my_program  # Show everything 👀
LD_DEBUG=libs ./my_program  # Library loading 🔍
LD_DEBUG=symbols ./my_program  # Symbol lookup 🔡
```

---


![bg](images/dont.webp)

<!-- 
Bad practice
-->

---


# ⚠️ Exporting All Symbols

🔹 By default, all symbols may be exported 📤  
🔹 **Problem:** Exposing unnecessary functions may cause **symbol conflicts** 🛑  
🔹 **Example:** Exporting 3rd-party libraries like **Boost, IPP, OpenCV**

---

# 🚨 What Can Go Wrong?

❌ **Symbol Conflicts** – Runtime crashes, wrong ABI ⚡  
❌ **Linking Issues** – Different version might be used across libraries 📌  
❌ **Unintended ABI Exposure** – Internal functions can accidentally be used 🛠️  

---


# 🔍 How to Verify Symbol Exports

✅ **Linux: Use `nm` or `objdump`**
```bash
nm -D myLib.so  # Lists exported symbols
objdump -T myLib.so  # Shows dynamic symbols
```

🔹 **Check exports before releasing shared libraries!** 🚀

---

# Function Visibility 

✅ **Windows (DLLs)** → Uses `__declspec(dllexport)` & `__declspec(dllimport)`.  
✅ **Linux (SOs)** → Uses `__attribute__((visibility("default")))`.  

Notes :
* `__declspec(dllimport)` is optional, for functions.
* Linux - default is visible, Windows - default is invisible.
* Which functions are exported: `nm -D` , `objdump` / `dependencies`

---

# 🔹 Function Visibility in Windows

```cpp
#ifdef BUILD_DLL
#define API_EXPORT __declspec(dllexport)
#else
#define API_EXPORT __declspec(dllimport)
#endif

API_EXPORT void myFunction(); // Exported function
```
✅ **Importing (from EXE or another DLL)** → Use `__declspec(dllimport)`

---

# 📦 Function Visibility in Linux I

```cpp
#define MYLIB_API __attribute__((visibility("default")))

MYLIB_API int add(int a, int b);
```

```cmake
target_compile_options(mylib PRIVATE -fvisibility=hidden)
```

- ✅ Hides symbols by default
- ✅ Exports only tagged symbols (like `MYLIB_API`)


---

# 📦 Function Visibility in Linux II

- 🛠 Create a version script (`exports.map`):
```text
{
  global:
    add;
  local:
    *;
};
```

- 📦 Use it in CMake:
```cmake
set_target_properties(add PROPERTIES
  LINK_FLAGS "-Wl,--version-script=${CMAKE_SOURCE_DIR}/exports.map"
)
```


---

## **Advanced**

* Manual loading
* Name mangling
* Versioning (`so.1.2.3`)

---


# 🧩 Manually Loading DLLs

- ✅ Load at runtime (plugins, late binding)
- 🧠 Use `dlsym` / `GetProcAddress` / `boost::dll`
- 🔄 Flexible, optional dependencies
- ❗ Must know symbol names
- 🪧 `extern "C"` avoids name mangling
- 🕒 Manage global/static init

---

# ⚠️ Manual Loading: Pitfalls

- 🔍 Hard to debug missing or broken symbols
- 📛 Symbol names must match exactly
- 🔗 No linker error → runtime crash
- 🧪 Test for load failure (`nullptr`, try/catch)

--- 

## **Boost::dll Example**

```cpp
// Load the shared library
#ifdef _WIN32
boost::dll::shared_library lib("mylib.dll");
#else
boost::dll::shared_library lib("libmylib.so");
#endif
// Import the function dynamically
auto hello_func = lib.get<void()>("hello");
// Call the function
hello_func();

```

---

# Name Mangling 

🔹 **C++ compilers modify function names**   
🔹 **Compiler specific** 
🔹 **C does NOT mangle names**
```cpp
void foo();   // Regular function
void foo(int);  // Overloaded function
namespace A {
    void foo(); // Namespaced function
}
```

---

### **Name Mangling in GCC/Clang (`nm`)**
```bash
_Z3foov         # void foo()
_Z3fooRi        # void foo(int)
_N1A3fooEv      # void A::foo()
```

### **Name Mangling in MSVC (`dumpbin`)**
```bash
?foo@@YAXXZ        # void foo()
?foo@@YAXH@Z       # void foo(int)
?foo@A@@YAXXZ      # void A::foo()
```

<!---
c++filt

#include <dbghelp.h>
#include <cxxabi.h>

-->

--- 

### *extern "C"*
```cpp
extern "C" void foo();  // No name mangling applied
extern "C" {
    void bar();
    int baz(int);
}
```

✅ **Use `extern "C"` for**
- **C++ code that needs C-compatible APIs**
- **Dynamic libraries that must be callable from C**
- **Avoiding cross-compiler mangling issues**

---

# DLL Hell (Windows)

⚠️ **Version conflicts**  
* Different apps require different versions of the same DLL  
* **Example:** `gdal.dll` 
* **Fix:** 
  * Use local DLLs 
  * Rename 
  * [**SxS** (Side-by-Side assemblies)]

---


### **📌 SO Versioning in Linux**  

🔹 **Structure:**  
```
libmylib.so → libmylib.so.1 → libmylib.so.1.2.3
```
- **1** = Major (Breaking changes 🚨)  
- **2** = Minor (New features 🛠️)  
- **3** = Patch (Bug fixes 🐞)  

---

### **📌 SO Version in CMake**  

```cmake
add_library(mylib SHARED mylib.cpp)

set_target_properties(mylib PROPERTIES
    VERSION 1.2.3      # Full version
    SOVERSION 1        # Major version
)
```
🔹 **Creates:**  
```
libmylib.so → libmylib.so.1 → libmylib.so.1.2.3
```

---

# 🏗️ `LD_PRELOAD`

🔹 **Inject shared libraries** 📌  
🔹 **Override functions without rebuilding** 🔄  

```bash
LD_PRELOAD=/path/to/mylib.so ./my_program
```

---


## **Common linker problems**, its **cause**, how to **debug**, and **solutions**.

---


# ❌ `-fPIC`

### **Problem**
```bash
/usr/bin/ld: mylib.a(myfile.o): relocation R_X86_64_32S against `.text' 
can not be used when making a shared object; recompile with -fPIC
```

### **Solution**
✅ Compile with **`-fPIC`**

---

## ❌ `_ITERATOR_DEBUG_LEVEL` Mismatch (MSVC)

### **Problem**
```text
error LNK2038: mismatch detected for '_ITERATOR_DEBUG_LEVEL': 
value '0' doesn't match value '2'
```

### **Solution**
✅ Ensure **consistent build types**

<!---
Add here the dumpbin /?? command

-->

---

## ❌ `MDd_DynamicDebug` Mismatch (MSVC)


### **Problem**
```text
 error LNK2038: mismatch detected for 'RuntimeLibrary': 
 value 'MTd_StaticDebug' doesn't match value 'MDd_DynamicDebug' in program.obj
     C:\Data\ip_core.lib(ip_core.obj)    
```

---

## ❌ Cannot find ` mydll.lib` (MSVC)


### **Problem**
```text
LINK : fatal error LNK1104: cannot open file 'Debug\add.lib' 

```

### **Solution**
✅ In windows, a `.lib` file is created **Only if there are exported symbols**. Check `__declspec(dllexport)`

---

## ❌ Incorrect Shared Object (`.so`) Link Order

### **Problem**
```bash
/usr/bin/ld: undefined reference to `myFunction`
```

### **Solution**
✅ **Correct order**
✅ Use **grouping**: `g++ -Wl,--start-group -lfoo -lbar -Wl,--end-group -o myapp`

---

# ❌ Missing `.so` File at Runtime

### **Problem**
```bash
./myapp: error while loading shared libraries: libmylib.so: cannot open shared object file
```


### **Solution**
✅ **Check if the library is found**
✅ **Fix missing paths `LD_LIBRARY_PATH`**

---

# ❌ Wrong shared object loaded at Runtime

### **Solution**

✅ **Check which DLL is loaded**  
* Windows : Debug -> Windows -> Modules
* Linux : gdb -> `info shared`



---


# ❓ Q: What happens if a function exported by a DLL is missing in the `.lib` file?

- `①` The function cannot be used at all.  
- `②` The function can still be loaded using `LoadLibrary()` and `GetProcAddress()`.  
- `③` The application crashes at startup.  
- `④` The linker automatically resolves the missing function at runtime.  
<!---
✅ **Answer:** **The function can still be loaded using `LoadLibrary()` and `GetProcAddress()`.**  
🔍 **Explanation:** The `.lib` file is only needed for static linking. Dynamic loading (`LoadLibrary`) bypasses it.
-->

---

# ❓ Q: Which command shows if a program is missing an `.so` file in Linux?

- `①` `ls -l /usr/lib/ | grep mylib.so`  
- `②` `nm -D mybinary`  
- `③` `ldd mybinary`  
- `④` `objdump -d mybinary | grep mylib.so`  

<!---
✅ **Answer:** **`ldd mybinary`**  
🔍 **Explanation:** `ldd` prints all dynamically linked libraries and highlights missing `.so` files.
-->
---

# ❓ Q: What happens if a Windows DLL is compiled with `/MDd` and linked to a `/MD` executable?

- `①` The program runs fine with a performance hit.  
- `②` The program crashes due to runtime mismatch.  
- `③` The linker issues a warning but allows execution.  
- `④` The DLL is ignored, and the executable uses static linkage instead.  

<!---
✅ **Answer:** **The program crashes due to runtime mismatch.**  
🔍 **Explanation:** **Debug (`/MDd`) and Release (`/MD`) runtime libraries are incompatible** due to different heap implementations.
-->
---


# ❓ Q: What does the `-Wl,-rpath=/custom/path` flag do in GCC linking?

- `①` Sets the default path for `dlopen()`.  
- `②` Hardcodes `/custom/path` as the runtime search path for `.so` files.  
- `③` Makes the linker ignore all default system libraries.  
- `④` Embeds the `.so` directly into the executable.  

<!---
✅ **Answer:** **Hardcodes `/custom/path` as the runtime search path for `.so` files.**  
🔍 **Explanation:** This ensures that when the program runs, it **prefers** shared objects from `/custom/path` **without needing `LD_LIBRARY_PATH`**.
-->
---


# ❓ Q: What does `ldconfig` do in Linux?

- `①` Loads shared libraries into memory.  
- `②` Updates the dynamic linker’s cache of `.so` locations.  
- `③` Compiles `.so` files into `.o` files.  
- `④` Forces all running programs to reload their `.so` dependencies.  

<!---
✅ **Answer:** **Updates the dynamic linker’s cache of `.so` locations.**  
🔍 **Explanation:** Run `ldconfig -p | grep mylib.so` to see if a library is registered.
-->
---

# ❓ Q: How can you check if a Windows DLL is loaded into a process?

- `①` `tasklist /m mylib.dll`  
- `②` `Get-Process | Where-Object { $_.Modules.ModuleName -match "mylib.dll" }`  
- `③` Use the "Modules" window in Visual Studio Debugger.  
- `④` All of the above.  

<!---
✅ **Answer:** **All of the above.**  
🔍 **Explanation:** Each method works, but **Process Explorer (Sysinternals) is the best** visual tool for checking loaded DLLs.
-->
---

