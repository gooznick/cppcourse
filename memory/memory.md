---
title: debug
author: eranbu
date: 12.2025
marp: true
theme: gaia
paginate: true

---

# Memory in C++ 🧠

### Three Fundamental Memory Regions

🧱 **Stack**  
Fast, automatic, scope-bound

📦 **Heap**  
Dynamic, flexible, explicitly managed

🏛️ **Static / Global**  
Program-lifetime, shared state

---

# Stack Memory 🧱

### What lives on the stack?

```cpp
void foo(int a, int b) 
{
    int x = 42;
    int y = a * x * 2;
    bar(x);
};
```

<!-- a,b ? x64 are registers
     x, y may be registers
    calling bar - putting stack pointer on the stack, and return address.
-->

---

# Stack Memory 🧱

### Key properties

⚡ **Very fast** - Allocation = pointer movement
⏱️ **Deterministic** 
📏 **Limited size** - Per thread
🧨 **Unsafe when misused** - Stack overflow, dangling references
🧩 **Compiler-managed** - Layout, alignment, calling convention

---

# Stack - fast 

### When does allocation happen?

```cpp
void f()
{
    int a[100];   // stack array
    use(a);
}
```

![bg right width:600px](images/asm1.png)


<!-- Allocation happen when entring the function. The compiler "knows" that 100*4 bytes are reserved.
     They won't be cleaned before/after usage
-->

---

# Stack management registers 🧠


* rsp → moving top of the stack

* rbp → fixed anchor for the function

<!--
-fno-omit-frame-pointer → “Keep the anchor, even if it costs a little”
-->

---

# How much stack does function call takes ?

```cpp
void measure_stack(int depth, uint8_t* prev_addr)
{
    volatile std::uint8_t marker = 0;
    std::ptrdiff_t bytes = prev_addr - &marker;
    std::cout << bytes << " bytes" << std::endl;

    if (depth == 0) return;

    measure_stack(depth - 1, (uint8_t*)&marker);
}
```

<!--
-140729952818439 bytes
64 bytes
64 bytes
64 bytes
64 bytes
64 bytes
-->

---
 
## Adding arguments

```cpp
#include <iostream>
#include <stdint.h>


void measure_stack(int depth, uint8_t* prev_addr,int a,int b, int c, int d, int e,int f,int g)
{
    volatile std::uint8_t marker = 0;
    std::ptrdiff_t bytes = prev_addr - &marker;
    std::cout << bytes << " bytes" << std::endl;

    if (depth == 0) return;

    measure_stack(depth - 1, (uint8_t*)&marker,a,b,c,d,e,f,g);
}

int main()
{
    measure_stack(5, nullptr,0,0,0,0,0,0,0);
    return 0;
}
```

<!--
-140729952818439 bytes
128 bytes
128 bytes
128 bytes
128 bytes
128 bytes

Try -O0 and -O3, you may be surprised.
(-O3 stack is bigger, because it stores more registers and calls directo iostream functions)
-->

---


# Linux Stack Size 🐧🧱

- **Process (main thread) limit**: `ulimit -s` (KiB)  


* **Per-thread stack** (pthread / Boost.Thread)

    ```cpp
    pthread_attr_setstacksize(&attr, 2*1024*1024); // 2 MiB
    ```

    ```cpp
    boost::thread::attributes attrs;
    attrs.set_stack_size(4096*10);
    ```

* **See current mapped stack in GDB**

  ```
  (gdb) info proc mappings 
  ```

---


# Windows Stack Size 🪟🧱

- **Process (main thread) stack**: linker default (`/STACK`)  


* **Per-thread stack** (Win32 / Boost.Thread)

    ```cpp
    CreateThread(nullptr, 2*1024*1024, thread_fn, nullptr, 0, nullptr);
    ```

    ```cpp
    boost::thread::attributes attrs;
    attrs.set_stack_size(4096*10);
    ```
* **See current stack in MSVC**

    ```
    ((NT_TIB*)@tib)->StackBase
    ((NT_TIB*)@tib)->StackLimit
    ```

---

# Stack bugs - I

```cpp
#include <cstdint>

std::uint64_t f(std::uint64_t n) {
    return (n == 0) ? 0 : 1 + f(n - 1);
}

int main() {
    return (int)f(100000000ULL);
}
```

---


# Stack bugs - II

```cpp

void g() {
    int big[10'000'000];
    big[0] = 1;
}

int main() {
    g();
}

```

---


# Stack bugs - III

```cpp

int& get_ref() {
    int x = 42;
    return x;  
}

int main() {
    int& r = get_ref();
    return r;           
}


```

---



# Stack bugs - IV

```cpp

#include <cstring>

void spai(const char* s) {
    char buf[8];
    std::strcpy(buf, s); 
}

int main() {
    spai("Lorem ipsum dolor sit amet, consectetur adipiscing elit");
}

```

---


# Heap Memory 📦

- **Dynamic allocation**  
  `new / delete`, `malloc / free`

- **Flexible lifetime**  
  Objects live **beyond scope**, until explicitly released

- **Larger but slower than stack**  
  Allocation involves allocator, bookkeeping, possible locks

---


# Linux `malloc` — Key Ideas I 📦

- **Optimistic allocation (overcommit)**  
  `malloc()` may succeed even if RAM is not available → OOM killer later

- **Two allocation paths**  
  - Small / medium → heap (`brk` / `sbrk`)
  - Large (≈ ≥128 KB) → `mmap`

---


# Linux `malloc` — Key Ideas II 📦

- **Multithreaded scalability**  
  Multiple **arenas** reduce lock contention  
  (trade-off: more memory usage)

- **Debug & tuning**  
  `mallopt()` / `MALLOC_*` env vars  
  (`ARENA_MAX`, `CHECK`, `PERTURB`, `MMAP_THRESHOLD`)

---

# Linux `brk` vs `mmap`

```
start: sbrk(0) = 0x555555559000
small malloc(1024) = 0x5555555596b0
after small: sbrk(0) = 0x55555557a000
ptr 0x5555555596b0 is in mapping: 555555559000-55555557a000 rw-p 00000000 00:00 0                          [heap]

big   malloc(4194304) = 0x7ffff77ff010
after big: sbrk(0) = 0x55555557a000
ptr 0x7ffff77ff010 is in mapping: 7ffff77ff000-7ffff7c00000 rw-p 00000000 00:00 0 
```


---

# Tuning Heap Allocation on Linux 🛠️📦

- **Arenas (multithreaded)**   `MALLOC_ARENA_MAX` 

- **`mmap` vs heap (`brk`)**   `MALLOC_MMAP_THRESHOLD_`  

- **Returning memory to OS**   `MALLOC_TRIM_THRESHOLD_`, `MALLOC_TOP_PAD_`  

- **Debug & safety (dev only)**    `MALLOC_CHECK_=3`, `MALLOC_PERTURB_≠0`

- **When tuning fails**  `jemalloc` / `tcmalloc` / pools

---

# Debugging Heap Problems on Windows 🪟🧠

- **Debug Heap (MSVC)**  
  Enable in Debug builds:  
  CRT: `_CrtDumpMemoryLeaks()`, `_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)`
  Address Sanitizer (Clang-cl)
  WinDbg: `!heap`, `!analyze -v`, `RtlAllocateHeap` breakpoints

- **Page Heap (Application Verifier)**  
  Detects overruns, use-after-free, double free  
  → `gflags /p /enable app.exe /full`

---

# Heap related bugs - I

```cpp
void lead() {
    int* p = new int[100];

    if (!OpenFileControl())
        return;

    std::cout<< "Finished leading" << std::endl;

    delete[] p;
}

```

---

# Heap related bugs - II

```cpp
void leaf() {
    int* p = new int[100];

    delete[] p;
    delete[] p;
}

```

---

# Heap related bugs - III

```cpp
void lean() {
    int* p = new int[100];

    delete p;
}

```

---

# Heap related bugs - IV

```cpp
void lean() {
    int* p = new int[100];

    delete[] p;
    p[42] = 5;
}

```


---

# Heap related bugs - V

```cpp
#include <vector>

int main() {
    std::vector<int> v;
    v.push_back(1);

    int* p = &v[0];   // pointer into vector storage

    v.push_back(2);   // may reallocate
    *p = 99;          // ❌ dangling pointer
}

```

---

# Heap related bugs - VI

```cpp
#include <cstring>

int main() {
    char* buf = new char[8];
    std::strcpy(buf, "this is too long");
    delete[] buf;
}

```

---


# Heap Allocation in Modern C++ 📦✨

- **Avoid raw `new` / `delete`**  
  Prefer **RAII** — lifetime bound to objects, not code paths

- **Smart pointers**  
  - `std::unique_ptr` → sole ownership (default choice)  
  - `std::shared_ptr` → shared ownership (use sparingly)

- **Containers manage memory**  
  `std::vector`, `std::string`, `std::map` allocate on the heap safely  
  Use `reserve()` to avoid reallocations

---
# Global/Static memory

```cpp
// 1) static global
static int g_counter = 0;

// 2) static class member
struct Stats {
    static int instances;
    Stats() { ++instances; }
};
int Stats::instances = 0;

// 3) static local variable
int& next_id() {
    static int id = 0;   // initialized once, first call
    return ++id;
}
```

---

```cpp
struct Foo {
    int x;         
    Foo() {}      
};
Foo g_foo;

int main() {
    Foo s_foo;              
    Foo* h_foo = new Foo;  

    std::cout << "global g_foo.x = " << g_foo.x << "\n";
    std::cout << "stack  s_foo.x = " << s_foo.x << "\n";
    std::cout << "heap   h_foo->x = " << h_foo->x << "\n";

    delete h_foo;
}
```

<!--
- global must be 0
- stack - ?
- heap -?  , MALLOC_PERTURB_=165 - garbage
-->

---

| Kind | Zero-initialized? | Constructor called | Destructor called | Typical storage |
|-----|-------------------|--------------------|-------------------|-----------------|
| **Global variable** | ✔️ always (first phase) | Before `main()` | After `main()` (program exit) | `.bss` (if no init)<br>`.data` (if init)<br>`.rodata` (if `const`) |
| **`static` global** | ✔️ always | Before `main()` | After `main()` | `.bss` / `.data` / `.rodata` |
| **Static class member** | ✔️ always | Before `main()` | After `main()` | `.bss` / `.data` / `.rodata` |
| **Static local variable** | ✔️ always | **On first execution** of declaration | At program exit | `.bss` / `.data` / `.rodata` |

---

# Initialization & Layout ⚙️

- **Zero-initialized by default**  
  (`.bss` segment)

- **Explicitly initialized data**  
  Stored in `.data` segment

- **Const data**  
  Stored in `.rodata` segment

- **Initialization order**  
  - Same translation unit → top to bottom  
  - Across translation units → **unspecified order**


---

# Static Locals (Modern Pattern) 🔒

```cpp
int& counter() {
    static int value = 0;  // initialized once
    return value;
}
```

---

# Note : Once pattern

```cpp
void  my_function() {
    for (auto& frame : frame){
        Process();
        static bool once = [](){log_i("Processed first frame !");}();
    }
}
```

---

# **Static Memory**

- Global and static variables are stored here.
- Lifetime: Entire duration of the program.
- Initialized at program startup, deallocated at exit.

**Example:**
```cpp
#include <iostream>
static int counter = 0;  // Stored in static memory

void increment() {
    static int calls = 0;  // Persists across function calls
    calls++;
    std::cout << "Function called " << calls << " times\n";
}

int main() {
    increment();
    increment();
    return 0;
}
```

---

# **Shared Memory Across Processes**

- Shared memory allows **multiple processes** to access the **same memory region**, enabling fast communication.
- Used in **multi-process applications** like databases, real-time systems, and IPC (Inter-Process Communication).

## **Cross-Platform Shared Memory Methods**

| Method | Platform | Description |
|--------|----------|-------------|
| **POSIX Shared Memory (`shm_open`)** | Linux, macOS | Uses file-backed shared memory objects. |
| **System V Shared Memory (`shmget`)** | Linux, Unix | Traditional shared memory implementation. |
| **Boost.Interprocess Shared Memory** | Cross-Platform | Portable abstraction over shared memory APIs. |
| **Windows Memory-Mapped Files (`CreateFileMapping`)** | Windows | Uses file mapping for shared memory. |

---

# Boost.Interprocess Overview

- Boost.Interprocess provides a portable way to manage shared memory.
- It abstracts OS-specific APIs like:
  - `shm_open()` (POSIX)
  - `CreateFileMapping()` (Windows)
- Ensures safe memory management using **mutexes and semaphores**.

---

# Creating Shared Memory

```cpp
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <cstring>

using namespace boost::interprocess;

int main() {
    // Create a shared memory object
    shared_memory_object shm(create_only, "MySharedMemory", read_write);
    shm.truncate(1024);
    
    // Map the shared memory region
    mapped_region region(shm, read_write);
    
    // Write data into shared memory
    std::strcpy(static_cast<char*>(region.get_address()), "Hello from shared memory!");
    
    std::cout << "Shared memory created and initialized.\n";
    return 0;
}
```

✅ **Creates a shared memory segment** named `MySharedMemory`.
✅ **Maps it into the process address space**.
✅ **Writes a string into shared memory**.

---

# Accessing Shared Memory in Another Process

```cpp
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

using namespace boost::interprocess;

int main() {
    try {
        // Open existing shared memory
        shared_memory_object shm(open_only, "MySharedMemory", read_write);
        
        // Map the shared memory region
        mapped_region region(shm, read_write);
        
        // Read data from shared memory
        std::cout << "Read from shared memory: " << static_cast<char*>(region.get_address()) << "\n";
    } catch (const interprocess_exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
    return 0;
}
```
---