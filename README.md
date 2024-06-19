# cppcourse
Cpp and some more presentations and demonstrations 

## Content

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

