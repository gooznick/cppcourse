---
title: The Linker
author: Your Name
date: 2024
theme: gaia
marp: true
---

<!---
Prepare  :
linux cmake 3.24

-->

<img src="images/errors.webp" width="400" style="display: flex;" />

---

# The Linker

* **Links object files into an executable or library**  
* Resolves **symbols** from different sources  
* Fixes **addresses** for functions & variables  
* Produces **executables, DLLs (Windows), SOs (Linux)**  

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
3. Statically links `.a` files  
4. Dynamically links `.so` files  

⚠ **Order matters in Linux!**  
Symbols must be **defined before use** in command:  
```bash
g++ main.o -lfoo -o app  # ❌ Undefined reference
g++ -lfoo main.o -o app  # ✅ Correct order
```


---

# Common Linking Problems

❌ **Undefined reference**  
```bash
g++ main.o -o app
# error: undefined reference to `foo()`
```
🔍 **Fix:** Link with missing library:  
```bash
g++ main.o -o app -lfoo
```
Or
```bash
g++ main.o -Wl,--start-group -lfoo -lbar -Wl,--end-group -o app
```

---

# **Undefined reference**  

* Find where it's defined (`readelf`, `grep -r`)
* Find link command (`link.txt` in cmake)
* Make sure the missing symbol is after the user
* Change cmake link order or add `LINK_GROUP`


<!---
order example

target_link_libraries(main PRIVATE "$<LINK_GROUP:RESCAN,add,mul>")

-->

---

# Windows DLLs

* Requires `.lib` for linking
* Linked at runtime (`.dll`)
* Search order:
  * Exe directory
  * System PATH

---

## **Linux SOs**

* Shared object (`.so`)
* Linked at runtime
* Search order:
  * `RPATH`
  * `LD_LIBRARY_PATH`
  * `/etc/ld.so.conf` (`man ldconfig`)


---

## **Advanced**

* Explore dependencies (`dependencies` / `ldd`)
* Load dynamically (boost::dll)
* Delay load in windows
* Versioning (`so.1.2.3`)

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
boost::function<void()> hello_func = lib.get<void()>("hello");
// Call the function
hello_func();

```

---

# Name Mangling 

🔹 **C++ compilers modify function names**   
🔹 **Compiler specifc** 
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

# 🔹 Function Visibility in Linux

✅ **Explicit export using `visibility("default")`**
```cpp
__attribute__((visibility("default"))) void myFunction();
```


---
# Version Script (Linux)

### `mylib.map`
```plaintext
MYLIB {
    global:
        my_*; // Exported
    local:
        *;  // Hide everything else
};
```

```bash
g++ -shared -o libmylib.so mylib.o -Wl,--version-script=mylib.map
```
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


# Why Shared Objects?

✅ **Memory savings** 
✅ **Easier updates** 
✅ **Plugins & modular design** 

⚠ **Risks:**  
❌ Version mismatches  
❌ Dependency hell  

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

# 🔍 Linker Debugging Tools


✅ **Linux:** `LD_DEBUG`, `LD_PRELOAD`
✅ **Windows:** `gflags`

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

# 🏗️ `LD_PRELOAD`

🔹 **Inject shared libraries** 📌  
🔹 **Override functions without rebuilding** 🔄  

```bash
LD_PRELOAD=/path/to/mylib.so ./my_program
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

# 🔒 How to Prevent

✅ **Use Default Visibility Hidden:**
```cmake
target_compile_options(my_lib PRIVATE -fvisibility=hidden)
```

---


✅ **Use `version scripts` in Linux (`.map` files):**
```bash
myLib.so {
    global:
        myPublicFunction;
    local:
        *;
};
```
```bash
gcc -Wl,--version-script=myLib.map -shared -o myLib.so myLib.o
```
```cmake
target_link_options(my_lib PRIVATE
    "LINKER:-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/myLib.map"
)
```

---

# 🔍 How to Verify Symbol Exports

✅ **Linux: Use `nm` or `objdump`**
```bash
nm -D myLib.so  # Lists exported symbols
objdump -T myLib.so  # Shows dynamic symbols
```

🔹 **Check exports before releasing shared libraries!** 🚀

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

