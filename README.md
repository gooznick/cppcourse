# cppcourse
Cpp and some more presentations and demonstrations 

## Content

| Lecture  | Subjects|  Presentation                                    | Comments   | 
|----|---------------|--------------------------------------------------|------------|
|  1 | Preface       |                                                  |            |  
|    | Git           |  [git.md](git/git.md)                            |            |  
|    | Components    |  [components.md](components/components.md)       |            |  
|  2 | Preprocessor  |  [preprocessor.md](preprocessor/preprocessor.md) |            |  
|    | Compiler      |  [compiler.md](compiler/compiler.md)             |            |  
|  3 | Linker        |  [linker.md](linker/linker.md)                   |  +dll/so   |  
|  4 | ABI           |  [abi.md](abi/abi.md)                            |            |  
 
## Urgent Todos :

* Add to compiler two slides:
   * The bible (standard)
   * Undefined behavior : types, how to avoid, memcopy, alignment
* In memory : Add the linux command to allocate page immediately.


### **C++ Lecture Schedule **  

1. **Version Control with Git and GitLab**  
   - Best practices for using Git 
   - Submodules and large files
   - Understanding "GitLab Flow" and recommended workflows  

2. **C++ Build System and Toolchain**  
   - Overview of the C++ toolchain (compiler, linker, debugger)  

3. **The C++ Preprocessor**  
   - Role and importance in C++ development  
   - Debugging preprocessor issues and common pitfalls  

4. **The C++ Compiler**  
   - Compilation stages and optimizations  
   - Tools for analyzing compilation behavior  
   - Debugging compilation errors  

5. **The Linker in C++**  
   - How linking works in C++  
   - Static vs. dynamic linking  
   - Common linking issues and debugging strategies  

6. **Application Binary Interface (ABI) in C++**  
   - ABI stability and compatibility  
   - Best practices for designing shared libraries and exporting APIs  

7. **Debugging C++ Applications**  
   - Understanding debugging information
   - Advanced debugging tools and methodologies

8. **Memory Management in C++**  
   - Stack vs. heap memory allocation  
   - Runtime memory allocation strategies
   - Identifying and resolving memory-related issues

9. **Software Testing in C++**  
   - Unit testing, integration testing, and system testing
   - Automating tests with Continuous Integration (CI)  

10. **Multithreading in C++**  
   - C++ threading model and best practices  
   - Understanding threads and task-based concurrency
   - Debugging multithreaded applications  

11. **Numerical Computation in C++**  
   - Fixed-point and floating-point representation in C++  
   - Precision considerations and floating-point standards  

12. **Error Handling in Modern C++**  
   - Exception handling and best practices  
   - Alternative approaches

13. **Integrating C++ with Python**  
   - Running Python code from C++  
   - Running C++ code from python
   - Using **pybind11** for seamless C++-Python interoperability

14. **Advanced C++ Features**  
   - Effective use of design patterns like **Singletons**  
   - When to use classes and templates

15. **Working with Third-Party C++ Libraries**  
   - Best practices for using external libraries
   - Using **OpenCV, Boost, and GDAL**


## Agenda

* Git and gitlab
   * git usage:
     * basics - .git directory, packing, garbage collection, non-centralized
     * rebase, merge, stash, push, pull, amend
     * history and confidential items
     * stash everything
     * submodules pros/cons
   * precommit hooks 
   * Gitlab as a whole (issues-jira, gitlab-ci vs jenkins)
   * gitlab flow
   * branches 
     * lightweight - working in parallel
     * dev branch - stash
     * CI approved
     * master branch
   * Large files/3rd party(os)/3rd party (internal)

* About c/cpp components
  * IDE
  * cross compiling
  * toolchain
  * compiler/linker
  * cmake
* About c/cpp components
  * Compiler
    * Is not an IDE
    * C++ versions (11/14/17/20). Thumb rule - 5 years back
    * Compiler goodies:
      * Supplementals (__has_include, #pragmas)
      * warnings (non standard)
      * compilation flags for sanitaizing, coverage and more
      * gcc and clang
      * intel compiler (gcc complaint)
  * IDE
    * Editor, copilot, commenting
    * debugging: stack, memory, breakpoints
  * Build system
    * CMake (ninja/more)
    * Code !
  * OS
    * Visual studio/CLion/vscode
    * OS dependent code (threading, etc)
  * Boost
    * OS independent (threads)
    * cpp version implementation
    * Example : reflection
 

 
* Preprocessor
  * Simple directives (#ifdef, header guard)
  * Stringify, macros, variadic macros
  * #ifdef and it's problems
  * macros - when to use and when not
  * Name collisions
    * of names (macros...)
    * of filenames (directories)
  * Some more directives (#pack, #warn)
  * boost preprocess
  * Macro and definition
  * `#include` <> and ""
  * File naming (case, "windows.h", directories)
  * preprocess only (-i) (-E -C -P)
  * The flow - cmake - cl commandline
  * Showing compilation commandline
  * one line
  * name clashes (windows...)
  * token pasting (## and stringify)

* Compiler
  * C++ versions (c++98,11,14,17,20,23)
  * debug and release builds
  * symbols
  * flags (optimizations and more)
  * Warnings and errors
  * Enhanced tools (sanitizers, coverage)
  * libclang
  * Compilation time (headers locations, enhanced preprocess)

* Linker
  * static and shared
  * name mangling and extern c
  * shared objects:
    * binary API
    * exceptions
    * memory model
    * version scripts / visibility
  * dynamic(programmatic) loading (so_loader)
  * cross compilation (linux, sysroot)
  * windows:
    * lib files
  * linux:
    * single pass multipass
      
 * CMake
   * versions
   * generator expressions
   * "generation/compilation/linkage time (example - copy 3rd from network, create git file)
   * installation
   * toolchain file
   * Cache and variables concept (cache force, usage examples)
   * Generation, build and install
   * Do and don't
   * target_* commands
   * externals 

 * Debugging :
    * Stack and symbols
    * debugging memory issues
    * visual studio / gdb / gdbserver
    * core dump, post mortem
    * optimization - call stack and variables
    * breakpoints:
      * code
      * memory - r/w
    
 * Testing :
    * boost test (google test)
    * unit test / regression test
    * Writing testable code (examples : HeightConverter, TrackerBraker)
    * Refactoring code 
    * Debugging memory issues

 * Multithreading :
    * Primitives
    * Building blocks
    * spurious wakeups

 * CI :
   *  CI machines
   *  docker
   
## TODO 

* integers casting and overflow
* exceptions
* floating point accuracy and exceptions
* boost libraries
* simd, sse, neon, ipp

