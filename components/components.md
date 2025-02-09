---
title: components
author: eranbu
date: 6/2024
theme: gaia
marp: true
---

# C/C++ components

---

# Toolchain

<img src="images/toolchain.webp" alt="Local Image" width="500" height="400" />

* gcc : g++, objdump, readelf, strings, nm, c++filt, ar...

---

# Toolchains

* Windows:
  * MSVC
  * MinGW-w64
  * Intel C++ Compiler (icpc / icpx) [Classic/OneAPI]
* Linux:
  * GNU toolchain (GCC)
  * LLVM/Clang
  * Intel C++ Compiler (icpc / icpx) [Classic/OneAPI]

---

# Cuda

![Image](images/cuda.png)

* Compiler of `.cu` files for NVIDIA GPUs


---

# C++ Versions

<img src="images/cpp_vers.png" width="1000" style="display: flex;" />

* Rule of thumb: Use a C++ version that is at least 5 years old for stability in production systems.

---

# Build systems

![Image](images/buildsystems.png)

* Features : Flags, Files manipulation, Installation

---

# IDE - Integrated Development Environment 

* Features: Code Editor, Compilation, Debugging, Integration with Other Tools

<img src="images/ide_poll.png" width="700" style="display: flex;" />


---

# Additional components

* Additions :
  * **Standard** c library : <stdlib.h>, <malloc.h>
  * **Standard** c++ library : <vector>, <map> 
  * OS specific : <windows.h> <afxwin.h> / <unistd.h> <pthread.h>
  * Compiler specific : <x86intrin.h> / <intrin.h>

---

# Why can't I compile in windows and run in linux ?

* ABI - e.g. function call 
* Executable format 
* Runtime libraries / syscalls (e.g. : printf, malloc)
* OS specific libraries - pthreads / win32api


---

# Cross Compilation

![Image](images/cross.png)

* More the host and target are similar, the problems are bigger

<!---
 apt install mingw-w64
x86_64-w64-mingw32-g++ -o hello.exe test.cpp
-->

---

# 3rd party common libraries

![Image](images/Boost.png)

* STL
* Boost
* Eigen
* OpenMP
* OpenCV
* TBB
* gtest

---

# Compiler specific

* Flags...
* Optimizations
* Alignment
* Warnings, notes and hints
* OpenMp
* Sanitizers and Static Code Analysis
* AVX intrinsics
* Vectorization



