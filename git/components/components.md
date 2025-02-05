---
title: C/C++ Components
author: Your Name
date: 6/2024
marp: true
theme: gaia
paginate: true
---

# 🚀 C/C++ Components Overview

Understanding the **toolchain, compiler, linker, build systems, IDEs**, and more!

---

# 🛠️ What is a Toolchain?

A **toolchain** is a collection of **tools** used to compile, link, and debug C/C++ programs.

- **Compiler** – Converts source code to machine code.  
- **Linker** – Combines multiple object files into an executable.  
- **Debugger** – Helps debug the application.  
- **Build System** – Automates the compilation process.  

---

# 🔧 The C/C++ Compiler

A compiler **translates C/C++ code into machine code**.

- **Preprocessing** (`cpp`)  
   - Expands macros, includes header files.  

- **Compilation / Assembly** (`gcc -S`)  
   - Converts preprocessed code into assembly language.  
   - Translates assembly code into machine code (`.o` file).  

- **Linking** (`ld`)  
   - Combines multiple object files into an executable.  


---

# 🔗 The Linker

🔹 The **linker** combines compiled `.o` files into a single executable.  
🔹 It resolves **function calls** across multiple files.

- **Static Linking** (`.a` archives)  
   - Copies libraries into the final executable.  
   - Bigger executable but faster runtime.  

- **Dynamic Linking** (`.so` shared libraries)  
   - Libraries are loaded at runtime.  
   - Smaller executable but requires shared libraries at runtime.  


---

# 🖥️ IDE 

🔹 **IDE (Integrated Development Environment)** provides:  
✔️ Code editor  
✔️ Build system integration  
✔️ Debugging tools  

- **Visual Studio** 
- **VS Code** (w/ extensions)  
- **CLion** (JetBrains)  
- **Eclipse CDT**  

---

# 🏗️ Cross-Compiling

🔹 **Cross-compiling** means compiling code for a **different architecture**.
🔹 Used for **embedded systems, Raspberry Pi, Android, and IoT devices**.

---

# 🛠️ CMake 

🔹 A **build system** automates compilation and linking.

✔️ Cross-platform  
✔️ Works with multiple compilers  
✔️ Generates Makefiles, Ninja, VS project files  
✔️ Supports toolchains
✔️ CMake is the industry standard for large projects!

<!-- Easy way to install on linux `cmake-3.31.5-linux-x86_64.sh` -->

---

# 🔌 C/C++ and the OS

💡 How does C/C++ interact with the OS?
- **System Calls** → `read()`, `write()`, `open()`
- **Memory Management** → `malloc()`, `free()`
- **Threads & Processes** → `pthread_create()`, `fork()`
- **File Handling** → `fopen()`, `fclose()`


---

# 🎯 Summary

| Component | Purpose |
|-----------|---------|
| **Compiler** | Translates source code to machine code |
| **Linker** | Combines object files into an executable |
| **IDE** | Provides code editing, debugging, and build tools |
| **Cross-Compiler** | Builds for different architectures |
| **Build System** | Automates compilation (CMake, Make) |
| **OS Integration** | System calls, memory, processes |

---

Questions ❓
