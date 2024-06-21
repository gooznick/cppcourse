---
title: components
author: eranbu
date: 6/2024
marp: true
---

# C/C++ components

---

# Toolchain

<img src="images/toolchain.webp" alt="Local Image" width="500" height="400" />

* gcc : g++, objdump, readelf, string, nm, c++filt, ar...

---

# Toolchains

* Windows :
  * msvc
  * mingw
  * icpc / icpx [Classic/OneAPI]
* Linux :
  * GNU toolchain - gcc
  * llvm-clang
  * icpc / icpx [Classic/OneAPI]

---

# C++ Versions

![Image](images/cpp_vers.png)

* Rule of thumb - use version of 5 years ago.

---

# Build systems

![Image](images/buildsystems.png)

* Features : Flags, Files manipulation, Installation

---

# IDE - Integrated Development Environment 

![Image](images/ide_poll.png)

* Features : Code Editor, Compile, Debugger,  Integration with other tools

---

# Architecture vs Operation system 

* ABI - e.g. function call 
* Executable format 
* Runtime libraries / syscalls (e.g. : printf, malloc)
* OS specific libraries - pthreads/win32api


---

# Cross Compilation

![Image](images/cross.png)

* More the host and target are similar, the problems are bigger
---

# Common libraries

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



