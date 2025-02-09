---
title: The Linker
author: Your Name
date: 2024
theme: gaia
marp: true
---

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

# Linux linkage

🛠 **Fix:**  
- Use `--start-group ... --end-group`
- Reorder `.o` and `.a` 
```bash
g++ main.o -Wl,--start-group -lfoo -lbar -Wl,--end-group -o app
```
<!---
Windows :
```
cl /c file.cpp
link file.obj /OUT:file.exe
```


Linux :
```
g++ -c file.cpp
g++ file.o -o file.out
```
-->

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
✅ **Fix missing paths**

---

# ❌ Wrong shared object loaded at Runtime

### **Solution**

✅ **Check which DLL is loaded**  
* Windows : Debug -> Windows -> Modules
* Linux : gdb -> `info shared`
