---
title: Toolchain and Build System Components
author: eranbu
date: 6/2024
theme: gaia
marp: true
---

![bg left](images/taiwan.jpg)

# Toolchain and Build System Components 🛠️


---

# Toolchain Overview 🔗

<br/><br/> 

<img src="images/definition.png" alt="Toolchain Definition" width="900" />

<!-- 
A software toolchain is a set of software development tools used together to compile, link, optimize, and debug software. A well-integrated toolchain ensures compatibility across components.
-->

---

# Toolchain Components 🏗️

<img src="images/components.jpg" alt="Toolchain Components" width="700" />

<!-- 
* GCC Toolchain: g++, objdump, readelf, strings, nm, c++filt, ar...
* MSVC: cl, link, dumpbin, editbin...
* LLVM Toolchain: clang++, lld, llvm-nm, llvm-objdump...
* Cross-compilation toolchains: aarch64-linux-gnu-gcc, arm-none-eabi-gcc...

the examples/simple.sh example
-->

---

# "...used together..."

<br/>

<img src="images/gear.jpg"  width="700" />

---

# Build Systems 🔨

<img src="images/buildsystems.png" alt="Build Systems" width="700" />

* **Features:** Dependency management, compiler flags, file manipulation, installation

<!-- 
Build systems automate compiling, linking, and installing software. Examples: Make, CMake, Ninja, Meson.

According to the definition, it's part of the toolchain.

My definition: Only the tools that depend one another are called "toolchain"
-->

---

# IDE 
### Integrated Development Environment 🖥️

* **Features:** Code Editor, Compilation, Debugging, Toolchain Integration

<img src="images/ide_poll.png" width="700" />

<!-- 
IDEs help developers by integrating all tools into a single interface. Examples: Visual Studio, CLion, VS Code.
-->

---

# Toolchains by Platform 💻

* **Windows:**
  * MSVC
  * MinGW-w64 / Clang
* **Linux/macOS:**
  * GNU toolchain (GCC)
  * Clang

<!-- 
LLVM
Different operating systems have different native toolchains. Some, like Intel's ICC, support multiple platforms.
-->

---

# Heterogeneous computing Toolchains

<img src="images/cuda.png" width="300" />
<img src="images/sycl.png" width="300" />
<img src="images/OpenCL.png" width="300" />

* **Compiler** : `nvcc` / `icpx`
* **Debugger** 
* **Profiler**
<!-- 
**More** : `nvcc`, `cuobjdump`, `nvdisasm`, `nvprune`
CUDA toolchain is specialized for GPU programming. Works with MSVC, GCC, and Clang but requires specific linker configurations.
-->

---


![bg](images/elf.jpg)

<!-- 
In the rest of the lecture, we will answer a (simple?) question :

I Compiled simple program that adds two numbers. 
Which platform can run it ?

And when the program is more complicated ?

I'll begin with both linux/windows but then I'll speak only about linux.
-->

---

# Static Executable Portability

* ISA - Instruction Set Architecture
   * X86 / X86_64 / Arm
* Instruction sets
   * MMX / SSE / AVX / FMA / AES
* Executable format :
   * PE (exe) / ELF 
* Execution :
   * _start → mainCRTStartup → main /  _start → main
<!-- 

Executable is not portable across different ISA/IS/OS

Demo : AVX instruction set

PE portable executable
ELF - Executable and Linkable format

-->

---

# Static Executable Portability

* Application Binary Interface (ABI):
  * Function calling conventions, data alignment
* Using syscalls/win32api that has changed:
   * Windows : CreateFile2() (windows8+)
   * Linux : clone3() (Linux 5.3+) 
* Explicit or implicit calls !

<!-- 
Debugging Symbols Formats: DWARF (ELF), PDB (MSVC)

The compiler may add syscalls/win32api calls to the program (cout !!)

It may not be compatible with the running kernel/windows version.

Example : syscall.cpp

Most of the time - we'll be fine, because kernel and win32api have backward compatibility.

-->

---

# Additional Components 📦

* **Standard Libraries:** `stdlib.h`, `malloc.h` /`vector`, `map`
* **OS-Specific Headers:** `windows.h`, `afxwin.h`/ `unistd.h`, `pthread.h`
* **Compiler-Specific Headers:** `x86intrin.h`, `intrin.h`

<!-- 
By default some of those are implemented in shared objects.

Libraries and headers differ by OS and compiler. 
Some functions are available only in specific environments.

<math.h>, <complex.h> and <fenv.h> implemented in libm.so

demo - simple program ("simple") link without "-static"

```sh
g++ simple.cpp -osimple
ldd simple
nm -D simple | c++filt
```

| **Shared Library**              | **Common Functions Implemented** |
|---------------------------------|---------------------------------|
| **`linux-vdso.so.1`** (Virtual Dynamic Shared Object) | `gettimeofday()`, `clock_gettime()`, `time()`, `getcpu()` (Optimized system calls hout a syscall instruction) |
| **`libstdc++.so.6`** (GNU C++ Standard Library) | `std::vector`, `std::string`, `std::cout`, `std::map`, `std::sort`, `std::thread` (C++ ndard Library functions) |
| **`libc.so.6`** (GNU C Library - glibc) | `printf()`, `malloc()`, `free()`, `open()`, `read()`, `write()`, `fork()`, `execve()`, `exit()` re C standard and system calls) |
| **`libm.so.6`** (Math Library) | `sin()`, `cos()`, `sqrt()`, `log()`, `pow()`, `exp()`, `tan()`, `floor()`, `ceil()` (Mathematical ctions) |
| **`/lib64/ld-linux-x86-64.so.2`** (Dynamic Linker) | `_start()`, `dlopen()`, `dlsym()`, `dlclose()` (Loads shared libraries and resolves bols) |
| **`libgcc_s.so.1`** (GCC Support Library) | `__gcc_personality_v0()`, `__cxa_throw()`, `__cxa_begin_catch()`, `__cxa_end_catch()` (Exception handling, stack unwinding) |


 ## **🚀 Function Breakdown**
 | **Function** | **Purpose** | **Implemented In** |
 |-------------|------------|--------------------|
 | **`__cxa_atexit@GLIBC_2.2.5`** | Registers a function to run when `exit()` is called (used for global/static destructors). | `libc.so.6` libc) |
 | **`__cxa_finalize@GLIBC_2.2.5`** | Runs functions registered with `__cxa_atexit` before program termination. | `libc.so.6` (glibc) |
 | **`__gmon_start__`** | Used for profiling (GNU `gprof`). **Weak symbol**, sometimes auto-inserted. | `libc.so.6` (if profiling enabled) |
 | **`_ITM_deregisterTMCloneTable`** | Transactional memory support (used in GCC's **Thread-Level Speculation**). **Weak symbol**. | `libgcc_s..1` |
 | **`_ITM_registerTMCloneTable`** | Registers transactional memory clone tables for **GCC optimizations**. | `libgcc_s.so.1` |
 | **`__libc_start_main@GLIBC_2.34`** | Entry point for **glibc-based programs** (sets up argc, env, calls `main()`). | `libc.so.6` (glibc) |
 | **`std::basic_ostream<char, std::char_traits<char> >::operator<<(...)`** | Handles `std::cout <<` operations (formatted output). | `libstdc.so.6` (GNU C++ Standard Library) |
 | **`std::ios_base::Init::Init()@GLIBCXX_3.4`** | Initializes global `std::cout` and `std::cin`. Required for C++ iostreams. | `libstdc++.so. |
 | **`std::ios_base::Init::~Init()@GLIBCXX_3.4`** | Cleans up iostreams (`std::cout`, `std::cin`) at program exit. | `libstdc++.so.6` |
 | **`std::cout@GLIBCXX_3.4`** | Global standard output stream (`std::cout`). | `libstdc++.so.6` |
 | **`std::basic_ostream<char, std::char_traits<char> >& std::endl(...)`** | Handles `std::endl`, which adds a newline (`\n`) and flushes tput. | `libstdc++.so.6` |
 | **`std::operator<< <std::char_traits<char> >(...)`** | Overloaded `operator<<` for C++ streams (`std::cout <<`). | `libstdc++.so.6` |


-->

---

# Dynamic linking

* `libstdc++.so.6` : c++ implementations.
* `libgcc_s.so.1` : gcc internal functions implementations.
* `ld-linux-x86-64.so.2` 
* `libc.so.6`

<!-- 
Mostly **backward** compatible.
Can install some older versions on same machine

Glibc must be compiled with the current kernel.

-->

---


# glibc - The GNU C Library

The project provides the **core libraries for the GNU system** and GNU/Linux systems. 

These APIs include such foundational facilities as **open, read, write, malloc, printf, getaddrinfo, dlopen, pthread_create, crypt, login, exit** and more.

The GNU C Library is designed to be a **backwards compatible**, portable, and high performance ISO C library.

<!-- 
Mostly **backward** compatible.
Can install some older versions on same machine

Glibc must be compiled with the current kernel.

example: running simple dynamic example on two ubuntu's : newer and older.

./simple: /lib/x86_64-linux-gnu/libc.so.6: version `GLIBC_2.34' not found (required by ./simple)
-->

---

![bg left width:600px](images/glibc.png)

* `ld-linux-x86-64.so.2` 
* `libc.so.6`
* `libm.so`
* `libdl.so`
* `ldd`

<!-- 
versionning @GLOBC... 
ldd --version
-->

---

# Windows equivalents 

`General` -> `Windows SDK Version`

```cpp
#define WINVER 0x0601
#define _WIN32_WINNT 0x0601
```

```bash
echo %INCLUDE%
```

```bash
dumpbin /imports simple.exe
Dependencies -imports simple.exe
```

<!-- 
C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\include;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\ATLMFC\include;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\VS\include;C:\Program Files (x86)\Windows Kits\10\include\10.0.22000.0\ucrt;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\um;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\shared;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\winrt;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\cppwinrt


:\Dependencies_x64_Release>Dependencies.exe -imports C:\Users\admin\source\repos\Project1\x64\Debug\Project1.exe
[-] Import listing for file : C:\Users\admin\source\repos\Project1\x64\Debug\Project1.exe
Import from module MSVCP140D.dll :
         Function ?good@ios_base@std@@QEBA_NXZ
         Function ?flags@ios_base@std@@QEBAHXZ
         Function ?width@ios_base@std@@QEBA_JXZ
         Function ?width@ios_base@std@@QEAA_J_J@Z
         Function ?sputc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QEAAHD@Z
         Function ?sputn@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QEAA_JPEBD_J@Z
         Function ?setstate@?$basic_ios@DU?$char_traits@D@std@@@std@@QEAAXH_N@Z
         Function ?_Osfx@?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAXXZ
         Function ?tie@?$basic_ios@DU?$char_traits@D@std@@@std@@QEBAPEAV?$basic_ostream@DU?$char_traits@D@std@@@2@XZ
         Function ?cout@std@@3V?$basic_ostream@DU?$char_traits@D@std@@@1@A
         Function ?flush@?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAAEAV12@XZ
         Function ?put@?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAAEAV12@D@Z
         Function ??6?$basic_ostream@DU?$char_traits@D@std@@@std@@QEAAAEAV01@P6AAEAV01@AEAV01@@Z@Z
         Function ?uncaught_exception@std@@YA_NXZ
         Function ?widen@?$basic_ios@DU?$char_traits@D@std@@@std@@QEBADD@Z
         Function ?fill@?$basic_ios@DU?$char_traits@D@std@@@std@@QEBADXZ
         Function ?rdbuf@?$basic_ios@DU?$char_traits@D@std@@@std@@QEBAPEAV?$basic_streambuf@DU?$char_traits@D@std@@@2@XZ
Import from module VCRUNTIME140_1D.dll :
         Function __CxxFrameHandler4
Import from module VCRUNTIME140D.dll :
         Function __C_specific_handler
         Function memcpy
         Function __C_specific_handler_noexcept
         Function __std_type_info_destroy_list
         Function __current_exception
         Function __current_exception_context
         Function __vcrt_GetModuleFileNameW
         Function __vcrt_GetModuleHandleW
         Function __vcrt_LoadLibraryExW
Import from module ucrtbased.dll :
         Function _execute_onexit_table
         Function _crt_atexit
         Function _crt_at_quick_exit
         Function terminate
         Function _wmakepath_s
         Function _wsplitpath_s
         Function wcscpy_s
         Function _register_onexit_function
         Function __stdio_common_vsprintf_s
         Function strcat_s
         Function strcpy_s
         Function __p__commode
         Function _set_new_mode
         Function _configthreadlocale
         Function _register_thread_local_exe_atexit_callback
         Function _initialize_onexit_table
         Function __p___argv
         Function __p___argc
         Function _set_fmode
         Function _exit
         Function exit
         Function _initterm_e
         Function _initterm
         Function _get_initial_narrow_environment
         Function _initialize_narrow_environment
         Function _configure_narrow_argv
         Function __setusermatherr
         Function _set_app_type
         Function _seh_filter_exe
         Function _CrtDbgReportW
         Function _CrtDbgReport
         Function strlen
         Function _cexit
         Function _seh_filter_dll
         Function _c_exit
Import from module KERNEL32.dll :
         Function GetProcAddress
         Function IsDebuggerPresent
         Function RaiseException
         Function MultiByteToWideChar
         Function WideCharToMultiByte
         Function RtlCaptureContext
         Function RtlLookupFunctionEntry
         Function RtlVirtualUnwind
         Function UnhandledExceptionFilter
         Function SetUnhandledExceptionFilter
         Function GetCurrentProcess
         Function TerminateProcess
         Function IsProcessorFeaturePresent
         Function QueryPerformanceCounter
         Function GetCurrentProcessId
         Function GetSystemTimeAsFileTime
         Function InitializeSListHead
         Function GetStartupInfoW
         Function GetModuleHandleW
         Function GetLastError
         Function HeapAlloc
         Function HeapFree
         Function GetProcessHeap
         Function VirtualQuery
         Function FreeLibrary
         Function GetCurrentThreadId
[-] Import listing done
-->
---


# Some Thoughts 

* It's good to compile on `old` glibc (ipp!)
* The gcc uses glibc which uses the kernel of **my machine**.
* If I'm using static linked code, It's (mostly) fine.
* When using dynamic linked code, it's tight with my glibc and kernel.
* When compiling to other target, I have to **cross compile**.


---

# When I cannot use static link ?

* Missing static libraries (3rd, gcc's c++)
* I want to use dynamic libraries.
* Some networking functions (gethostbyname)
* Using cuda or other library that supports only dynamic link.

---


# Cross-Compilation 🌍

<img src="images/cross.png" width="800" />

<!-- 
Cross-compilation allows compiling software for a different architecture or OS than the host system.
-->

---

# Cross-Compilation Challenges 🛠️

* Cross toolchain dependencies
   * chroot
   * docker
   * virtual machine
* Missing target's C/C++ libraries

* **More similarity between host & target = Bigger problems**

<!-- 
Cross-compilation requires careful dependency management. Example: ARM cross-compilers for embedded systems.

Example: 
   Cross compiling for aarch64
   Running ldd on the output will fail (!)
-->

---

# CMake Cross-Compilation 🛠️

```cmake
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Set cross-compiler
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Set sysroot if needed (optional)
set(CMAKE_SYSROOT /usr/bin/aarch64-linux-gnu)

# Set find root path mode (prevents using host libraries)
set(CMAKE_FIND_ROOT_PATH /usr/bin/aarch64-linux-gnu)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

```

---


# `strace` - System Call Tracing Tool 🛠️

- A powerful debugging tool for **Linux** 🐧
- Traces **system calls** and **signals**
- Useful for debugging and profiling

🔹 **Example Usage:**
```sh
strace ls
```
📌 Shows all system calls used by `ls`

---

## Common `strace` Flags ⚙️

| Flag | Description |
|------|-------------|
| `-e trace=` | Filter system calls (e.g., `-e trace=open,read`) |
| `-p PID` | Attach to a running process |
| `-f` | Follow child processes |
| `-c` | Count system calls (summary) |
| `-o file` | Save output to a file |

<!-- 
docker run --rm -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined ubuntu bash

apt update && apt install -y strace

strace ls
-->


---


## Windows Equivalent? 🖥️

🔹 Windows has no direct `strace`, but similar tool exist:
- **`Process Monitor (ProcMon)`** – GUI-based syscall tracing (Sysinternals)


---

## Bad coding practice

---

# Compiling with non-isolated toolchain

* **Libraries** - Using host `glibc`, `libstdc++`, etc. 
* **Kernel Headers** - Using host kernel headers
* **Sysroot Path** - Linkage to `/usr/lib` from the host

* `gcc -print-search-dirs`
* `echo | gcc -E -Wp,-v -`

<!-- 
isolated build machine != isolated running machine !!!!

3rd party - ipp, onnx runtime etc...
-->

---


# Virtualization 🖥️

<img src="images/containers.png" width="800" />

---

## Virtualization for Isolation 🌍

* Run some python versions
* Run separated toolchain
* Microservices
* Security

---

## Virtualization Technologies ⚙️

* **VT-x / AMD-V**: CPU hardware support for virtualization
* **Namespaces & Cgroups**: Linux OS isolation features (used in containers)
* **Hypervisor**: Software layer to manage VMs (e.g., KVM, Hyper-V)

---

## Docker 🐳
- **Container-based** virtualization
- **Uses OS namespaces & cgroups** (Linux kernel features)
- **Lightweight & fast** compared to VMs
- **No full OS kernel emulation**

<br/>
- Isolated Toolchains

---

## QEMU 🖥️
- **Full-system emulator or user-mode emulator**
- **Can emulate different CPU architectures**
- **Supports KVM for acceleration**

<br/>
- Embedded development


---

## WSL1 🏗️
- **Windows Subsystem for Linux (WSL1)**
- **Translates Linux syscalls into Windows syscalls**
- **Does not use virtualization**
- **Lower performance for native Linux features**

---

## WSL2 🚀
- **Uses a lightweight VM with a real Linux kernel**
- **Better performance than WSL1**
- **Provides full Linux compatibility**
- **Uses Hyper-V for virtualization**

<br/>
- Ideal for windows lovers
- Under Security testing
- Can work with Qemu

---

## VirtualBox 📦
- **Type-2 hypervisor (runs on top of OS)**
- **Slower than Type-1 hypervisors**

<br/>
- Desktop virtualization

<!-- 

## VMware 🏢
- **Enterprise-grade virtualization**
- **Offers both Type-1 and Type-2 hypervisors**
- **Optimized for performance and enterprise workloads**
-->
---


# Summary ✅

✔ Toolchains provide compilers, linkers, and libraries
✔ Build systems automate compilation
✔ Cross-compilation enables targeting different architectures
✔ Compatibility between components is key

<!-- 
This presentation covered toolchains, build systems, and challenges in cross-compilation.
-->

---


---

# Quiz Questions 🎓

---

Which tool can show imported functions from a Windows executable?

1. Process Explorer
2. dumpbin
3. Task Manager
4. Regedit

<!-- Answer: 2 
 (+Dependencies/Depends)
-->

---

WSL1 primarily operates by:

1. Hardware virtualization
2. Hyper-V integration
3. Syscall translation
4. Containerization

<!-- Answer: 3 
That what makes it pretty slow
-->

---

Which feature explicitly requires VT-x/AMD-V?

1. Docker containers
2. QEMU software emulation
3. VirtualBox (hardware virtualization mode)
4. WSL1

<!-- Answer: 3 
all others are software implementations
-->

---

Which syscall may break backward compatibility on Linux if compiled on newer kernels?

1. read()
2. clone3()
3. open()
4. write()

<!-- Answer: 2
clone3 is a new syscall
 -->

---

What is a typical consequence of linking against newer glibc?

1. Smaller executable size
2. Improved compatibility with older kernels
3. Potentially breaking compatibility with older glibc versions
4. Faster compilation times

<!-- Answer: 3 -->

---

Which of the following tools can trace system calls inside a Docker container?

1. strace (with SYS_PTRACE capability)
2. nm
3. objdump
4. readelf

<!-- Answer: 1 -->

---

Which ABI aspect affects portability of statically compiled executables?

1. Operating system GUI
2. Function calling conventions and data alignment
3. File system structure
4. Network latency

<!-- Answer: 2 -->

---

Which is true about VirtualBox virtualization?

1. Always uses pure software emulation
2. Primarily uses hardware virtualization but can fall back to software emulation
3. Does not support hardware virtualization
4. Runs only on Linux

<!-- Answer: 2 -->

---

Which toolchain component translates Linux syscalls into Windows syscalls?

1. Docker
2. WSL2
3. WSL1
4. QEMU

<!-- Answer: 3 -->

---

If cross-compiling for ARM64, which tool helps verify shared library dependencies without executing the binary?

1. ldd
2. readelf
3. strace
4. gdb

<!-- Answer: 1/2 
mostly ldd is not supplied in the toolchain -->

---


# Thank You! 🎯
