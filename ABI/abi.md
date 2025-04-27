---
title: abi
author: eranbu
date: 3.2025
marp: true
theme: gaia

---

![bg left width:500px](images/01.jpeg)


# 🔗 ABI & 🧱 DLLs

- ABI ➜ Alignment, Padding & Compatibility  
- DLL ➜ Exceptions, Memory, and Ownership

Our dll's should be "c compatible dll".

<!---
No mangling
Different compiliers.
Sizeof structs is same in windows/linux.
-->

---

## 🧠 What is ABI?

- ABI = **Application Binary Interface**
- Defines how binaries interact at runtime
- Must be consistent for:
  - 🔌 Communication
  - 📦 Serialization
  - 🧩 Shared libraries (`.dll`, `.so`)

---

![bg left width:500px](images/rules.png)

# **Rule #1**

# Use POD types

---

## What is POD?

- 🧱 **Plain Old Data** = simple structs
- 🔗 C-compatible memory layout
- 🚫 No ctors, dtors, virtuals, or private members
- ⚡ Safe for `memcpy`, binary serialization

---

# Is it a POD?

```cpp
struct Point {
    int x;
    int y;
};

struct Rectangle {
    Point top_left;
    Point bottom_right;
};
```

---

# Is it a POD?

```cpp
    tmpl_handle tmpl_Create(const std::string& ini_filename);
```

---


## `std::string` in MSVC

```cpp
struct std::string {
    char* _Ptr;         // Pointer to the string's data (heap or SSO buffer).
    size_t _Size;       // Length of the string (excluding null terminator).
    size_t _Capacity;   // Capacity of the buffer.
    char _SSO[16];      // Small string optimization buffer.
};
```


---

## `std::string` in GCC (<5.0)


```cpp
struct std::string {
    struct _Rep {
        size_t _Length;     // Length of the string.
        size_t _Capacity;   // Allocated capacity.
        size_t _RefCount;   // Reference count for COW.
        char _Data[1];      // Flexible array member holding the string data.
    };
    _Rep* _M_dataplus;      // Pointer to the _Rep structure.
};

```


---

## `std::string` in GCC (>=5.0)


```cpp
struct std::string {
    union {
        struct {
            char* _Ptr;     // Pointer to the dynamically allocated buffer.
            size_t _Size;   // Length of the string.
            size_t _Cap;    // Capacity of the buffer.
        };
        char _SSO[24];      // Inline buffer for small string optimization.
    };
};
```

---

![bg left width:500px](images/rules.png)

# **Rule #2**

# Always use fixed-width types.

---

## ❓ What is the output of this code?

```cpp
#include <iostream>

int main() {
    std::cout << sizeof(long) << " " << sizeof(int) << std::endl;
}
```
---
## 📋 Sizes of `int` and `long`

| Type        | 32-bit Windows | 32-bit Linux | 64-bit Windows | 64-bit Linux |
|:------------|:--------------:|:------------:|:--------------:|:------------:|
| `int`       | 4              | 4            | 4              | 4            |
| `long`      | 4              | 4            | 4              | 8            |

---

## ❓ What is the output of this code?

```cpp
int main() {
    char c = 200;
    if (c > 0) {
        std::cout << "Positive" << std::endl;
    } else {
        std::cout << "Negative" << std::endl;
    }
}
```

---

## 📋 Output of `char c = 200` Test

| Environment            | Default `char` | Output     |
|:------------------------|:--------------:|:----------:|
| Windows (MSVC)          | signed          | Negative   |
| Linux (GCC)             | signed          | Negative   |
| Linux (GCC `-funsigned-char`) | unsigned   | Positive   |
| ARM (GCC default)       | unsigned        | Positive   |

---


# Standard

| Type         | bytes | Standard              |
|:-------------|:-----:|:------------------|
| `bool`       | 1     | Only true/false guaranteed |
| `char`       | 1     | Signedness varies |
| `short`      | 2     | [−32767, +32767] at least |
| `int`        | 4     | [−32767, +32767] at least |
| `long long`  | 8     | [−9223372036854775807, +9223372036854775807] at least |


---

## 📋 Fixed-Width Types and Ranges

| Size   | Signed Type | Unsigned Type | Range                     |
|:------:|:------------|:-------------:|:-------------------------:|
| 8 bit  | `int8_t`    | `uint8_t`     | -128..127 / 0..255        |
| 16 bit | `int16_t`   | `uint16_t`    | -32k..32k / 0..65k         |
| 32 bit | `int32_t`   | `uint32_t`    | ~-2G..2G / 0..4G           |
| 64 bit | `int64_t`   | `uint64_t`    | ~-9E18..9E18 / 0..18E18    |
| 32 bit | `float`     | —             | ~±3.4×10³⁸                |
| 64 bit | `double`    | —             | ~±1.7×10³⁰⁸               |

---

## ⚡ Key Points

- Integer types: 
  - Exact number of bits
  - Defined in `<stdint.h>` / `<cstdint>`
- Floating point:
   - `float` / `double`: IEEE-754 standard
---


![bg left width:500px](images/rules.png)

# **Rule #3**

# Use packing of single byte

---


# 📏 Alignment & Padding

- ⚡ Ensures fast, aligned memory access
- 🧱 Structs may include hidden padding
- 🛠 Control with `#pragma pack`

---

# 📦 Unaligned Struct

```cpp
struct MyStruct {
    char a;    // 1 byte
    int  b;    // 4 bytes
    short c;   // 2 bytes
};
```

---

# 🧱 With Padding (implicitly)

```cpp
struct MyStruct {
    char a;        // 1 byte
    char pad1[3];  // alignment for int
    int  b;        // 4 bytes
    short c;       // 2 bytes
    char pad2[2];  // total = 12 bytes
};

```

---

## In code

```cpp
int main() {
    std::cout << sizeof(MyStruct) << "\n";  // 12
    std::cout << offsetof(MyStruct, a) << "\n";  // 0
    std::cout << offsetof(MyStruct, b) << "\n";  // 4
    std::cout << offsetof(MyStruct, c) << "\n";  // 8
    return 0;
}
```

---

## pahole (linux)

```bash
sudo apt install dwarves
gcc abi.cpp -g
pahole a.out 
```

```
struct MyStruct {
        char                       a;                    /*     0     1 */
        /* XXX 3 bytes hole, try to pack */
        int                        b;                    /*     4     4 */
        short int                  c;                    /*     8     2 */
        /* size: 12, cachelines: 1, members: 3 */
        /* sum members: 7, holes: 1, sum holes: 3 */
        /* padding: 2 */
        /* last cacheline: 12 bytes */
};
```
---

## Visual Studio (windows)

```bash
cl /d1reportSingleClassLayoutMyStruct abi.cpp
```

```
class MyStruct
   size: 12
   alignment: 4
   a: offset 0 (1 byte)
   <padding>: offset 1 (3 bytes)
   b: offset 4 (4 bytes)
   c: offset 8 (2 bytes)
   <padding>: offset 10 (2 bytes)
```

---

# 🚫 Disable Padding

```cpp
#pragma pack(push, 1) 
struct MyStruct {
    char a;    // 1 byte
    int b;     // 4 bytes
    short c;   // 2 bytes
};
#pragma pack(pop) 
```

⚠️ Use with care 
 - May impact performance
 - May cause misalignment


---

## ❓ Bitfield Question

```cpp
#include <iostream>

struct Flags {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
};

int main() {
    Flags f = {1, 2, 5};
    std::cout << sizeof(f) << std::endl;
}
```

What is the output?

<!---
8 bits (=1 byte)
+ additional = 4 bytes
-->
---

## 📖 Bitfield Explanation

✅ `a` needs 1 bit  
✅ `b` needs 2 bits  
✅ `c` needs 5 bits  
➡ Total = 8 bits = 1 byte

BUT most compilers align bitfields to **full int (4 bytes)** for speed!

| Compiler | Size     |
|:---------|:---------:|
| Windows (MSVC) | 4 bytes |
| Linux (GCC)    | 4 bytes |

Answer: **C) 4**

---


## ❓ Bitfield Question (2)

```cpp
#include <iostream>

#pragma pack(push, 1)
struct Flags {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
};
#pragma pack(pop)

int main() {
    Flags f = {1, 2, 5};
    std::cout << sizeof(f) << std::endl;
}
```

---

# Bitfield packing 

```cpp
#include <iostream>

#pragma pack(push, 1)
struct Flags {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
};
#pragma pack(pop)
```

| Compiler | Size     |
|:---------|:---------:|
| Windows (MSVC) | 4 bytes |
| Linux (GCC)    | 1 bytes |

---
# Bitfield packing 

```cpp
#include <iostream>

#pragma pack(push, 1)
struct Flags {
    unsigned char a : 1;
    unsigned char b : 2;
    unsigned char c : 5;
};
#pragma pack(pop)
```

| Compiler | Size     |
|:---------|:---------:|
| Windows (MSVC) | 1 byte |
| Linux (GCC)    | 1 byte |


---

## ❓ Union Question

```cpp
#include <iostream>

union U {
    int i;
    float f;
};

int main() {
    U u;
    u.i = 0x3F800000;
    std::cout << u.f << std::endl;
}
```

What is the output?

- A) 0
- B) 1
- C) -1
- D) Undefined

---

## 📖 Union Explanation

✅ In a `union`, all fields **share the same memory**.  
✅ 0x3F800000 is the IEEE-754 binary for **1.0f** in `float`.

- `u.i` sets the bits
- `u.f` reads the same bits as a float

Answer: **B) 1**

---

# Union of bitfield

```cpp
#pragma pack(push, 1)
struct Flags {
    unsigned char a : 1;
    unsigned char b : 1;
    unsigned char c : 1;
};

struct Telemetry
{
    union {
        int i;
        Flags f;
    };
};
#pragma pack(pop)

static_assert(sizeof(Telemetry) == 4);

```

---

![bg left width:500px](images/rules.png)

# **Rule #4**

# Export only needed functions

---

# CMake file for shared library export

```cmake
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
add_library(mylibrary SHARED mylibrary.cpp)
target_compile_definitions(mylibrary PRIVATE BUILDING_DLL)
```

---

# Macro for shared library export

```cpp
#if defined(_WIN32) 
  #ifdef BUILDING_DLL
    #define EXPORT __declspec(dllexport)
  #else
    #define EXPORT __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_DLL
    #define EXPORT __attribute__((visibility("default")))
  #else
    #define EXPORT
  #endif
#endif

EXPORT tmpl_handle tmpl_Create(const char* ini_filename);
EXPORT void tmpl_Destroy(tmpl_handle&);

```
---


![bg left width:500px](images/rules.png)

# **Rule #5**

# Use "extern c" linkage

---


## 📜 What is `extern "C"`?

✅ Special syntax in C++ to tell the compiler:

> "Use **C** function naming (no mangling)!"

**Syntax:**

```cpp
extern "C" 
{
    void my_function(int x);
}
```

---

## 🎯 Why use `extern "C"`?

✅ Prevents **name mangling** by C++ compiler

| Without `extern "C"` | With `extern "C"` |
|:---------------------|:-----------------:|
| `_Z12my_functioni`    | `my_function`     |

- Functions will have **plain C names** in compiled library
- Can be called from:
  - **C programs**
  - **Python (ctypes/FFI)**
  - **dynamic load**

---

## Will this compile ?

```cpp
#include <string>

extern "C" {

void print_int(int& i);         // 1
void print_str(std::string& s); // 2
void print(float x);            // 3
void print(double d);           // 4

}
```

<!---
<source>:8:6: error: conflicting types for 'print'
    8 | void print(double d);           // 4
      |      ^
<source>:7:6: note: previous declaration is here
    7 | void print(float x);            // 3
      |      ^
-->

---

![bg left width:500px](images/rules.png)

# **Rule #6**

# Use Create and Destroy

---


## ❌ Why Not Global Variables?

- Hard to control lifetime
- Impossible to reset safely
- Breaks multiple instances

<!-- 
Global variables cause:
- Lifetime issues: no way to free memory deterministically.
- Problems when loading/unloading shared libraries dynamically 
- Only one instance is possible; cannot create "sessions" or "contexts".
-->

---

## 🎯 Why Use Create / Destroy?

- Controlled construction and destruction
- Multiple independent handles
- Safer resource management
- Clean API for users

<!--
- Create allocates and initializes a context.
- Destroy cleans up memory, resources (files, sockets, internal states).
- Each handle is independent = you can create multiple sessions.
- Works well with any language that can call C functions
-->

---

## ⚡ Example Pattern

```cpp
typedef struct Tmpl_Handle Tmpl_Handle;

Tmpl_Handle* tmpl_Create(const char* ini_filename);
void tmpl_Process(Tmpl_Handle* handle, int param1, int param2...);
void tmpl_Destroy(Tmpl_Handle* handle);
```

✅ `tmpl_Create` allocates  
✅ `tmpl_Destroy` releases

<!--
Speaker Notes:
- Tmpl_Handle is an opaque type — users only know the pointer.
- `tmpl_Create()`  initializes
- `tmpl_Destroy()` ensures proper free
-->

---

![bg left width:500px](images/rules.png)

# **Rule #7**

# Use clear memory management idiom

---


## 🛡️ Two-Step Pattern

1. Ask for size:
```cpp
size_t size = tmpl_GetTelemetrySize(handle);
```

2. Allocate and fetch:
```cpp
std::vector<uint8_t> buf(size);
tmpl_GetTelemetry(handle, buf.data(), buf.size());
```

✅ No overflow  
✅ No assumptions

<!-- 
- Safe even for dynamic data that changes size at runtime (e.g., telemetry payloads, compressed frames).
-->

---

## ✏️ API Design

```cpp
TMPL_API size_t tmpl_GetTelemetrySize(Tmpl_Handle* h);
TMPL_API int tmpl_GetTelemetry(Tmpl_Handle* h, uint8_t* buffer, size_t buffer_size);
```

- `tmpl_GetTelemetrySize` ➔ tells caller how much to allocate
- `tmpl_GetTelemetry` ➔ writes into caller's buffer

<!-- 
- `tmpl_GetTelemetry` must check that buffer_size is enough.
- If buffer_size too small, either return an error or fill as much as possible (API decision).
- Future-proof: telemetry format can grow without breaking apps

-->

---

## 📜 Static Internal Pointer

```cpp
const char* err = tmpl_GetLastError(handle);
std::cout << "Error: " << err << std::endl;
```

✅ No free needed  
✅ Valid until next call

<!-- 
- Internal static or thread-local pointer inside the library.
- Returned string is owned by the library.
- Valid until the next API call (unless documented otherwise).
- Useful for small things: errors, status messages, version strings.
-->

---

## ✏️ API Design

```cpp
TMPL_API const char* tmpl_GetLastError(Tmpl_Handle* h);
```

- Fast and simple  
- **Readonly** — do not modify!

<!-- 
- Caller must never attempt to modify or free the pointer.
- Document clearly: when the pointer remains valid.
- If needed, user should copy the string immediately.
-->
```

---

## 🛡️ API Allocates Memory

```cpp
uint8_t* buf = nullptr;
size_t size = 0;
tmpl_AllocAndGetBlob(handle, &buf, &size);
// use buf...
tmpl_FreeBlob(buf);
```

✅ API owns allocation  
✅ User must free!

<!-- 
- API allocates dynamic memory 
- Caller gets pointer and size.
- Caller must call API-provided `tmpl_FreeBlob()` to release.
- Good for unknown-size data
-->

---

## ✏️ API Design

```cpp
TMPL_API int tmpl_AllocAndGetBlob(Tmpl_Handle* h, uint8_t** out_buffer, size_t* out_size);

TMPL_API void tmpl_FreeBlob(void* ptr);
```

- Caller **must** call `tmpl_free`
- Memory lifetime is controlled by caller

<!-- 
-->

---

![bg left width:500px](images/rules.png)

# **Rule #8**

# Error handling

---


## ❌ No Exceptions Across API

- No `throw` across shared object boundary
- No C++ exceptions in extern "C"

<!-- 
- Exceptions across shared object boundary cause ABI and runtime crashes.
- C cannot catch C++ exceptions.
- Always return error codes manually from functions.
-->

---

## ✅ Return Code Enum

```cpp
typedef enum {
    TMPL_OK = 0,
    TMPL_ERR_INVALID_ARGUMENT = 1,
    TMPL_ERR_INTERNAL = 2,
    TMPL_ERR_NO_MEMORY = 3,
} tmpl_result_t;
```

✅ `0` = OK  
✅ Positive values = errors

<!-- 
- Always define a clear enum for return values.
- `0` must mean "Success".
- Higher numbers represent different error types.
- This is readable, easy to document, and portable across languages.
-->

---

## 🧹 Get Last Error Message

```cpp
TMPL_API const char* tmpl_GetLastError(Tmpl_Handle* h);
```

- After error, call to get description
- Static or thread-local string

<!-- 
- Useful for human-readable explanations.
- The message pointer must not be freed or modified by the user.
- Valid until next API call (unless documented otherwise).
-->

---

# 🛠️ Other Patterns


* 📜 Optional Logging
```cpp
tmpl_SetLogger(const char* filename);
```

* 📜 Metry Counters

* 📜 Error Callback Registration
```cpp
typedef void (*tmpl_error_callback_t)(Tmpl_Handle* handle, const char* error_string);
tmpl_SetErrorCallback(Tmpl_handle, tmpl_error_callback_t);
```

<!-- 
- Some APIs allow setting a log callback: the library will log important events.
- Diagnostic counters (e.g., number of errors, retries) can be exposed.
- All these are optional helpers — **never replace clean return values**.
-->

---

![bg left width:500px](images/rules.png)

# **Rule #9**

# Versioning: Major.Minor.Patch

---

## 🔢 Versioning Concepts

- **Major.Minor.Patch** format
- Bump:
  - Major = breaking change
  - Minor = new features
  - Patch = bugfixes

✅ Clear evolution  
✅ ABI compatibility tracking

<!--
- Major: Breaking ABI change (structs change, function signatures change)
- Minor: New functions, optional features, no breakage
- Patch: Fixes only, no API or ABI change
-->

---

## 🧩 `GetVersion()` API

```cpp
TMPL_API void tmpl_GetVersion(int* major, int* minor, int* patch);
```

✅ Allow users to query library version at runtime.

<!--
- Never assume header and shared object are matching.
- Always provide a function to get the runtime version.
-->

---

## 🛠️ Linux `.so` Softlinks

- `libtmpl.so.1.2.3` — real file
- `libtmpl.so.1` — ABI version softlink
- `libtmpl.so` — linker-time softlink

✅ System controls which version is loaded.

<!--
- `libtmpl.so.1.2.3` is the actual file.
- `libtmpl.so.1` links to correct ABI version.
- `libtmpl.so` is used by the linker at build time.
- Lets you update bugfixes without breaking apps.
-->

---

## ⚙️ CMake Versioning

```cmake
set_target_properties(tmpl_shared PROPERTIES
    VERSION 1.2.3
    SOVERSION 1
)
```

✅ Creates correct `.so` links automatically.

<!--
- VERSION = Full version (major.minor.patch)
- SOVERSION = ABI version (major only usually)
- CMake auto-creates correct .so.1 and .so symlinks.
- Updating SOVERSION signals ABI change to system package managers.
-->


---

![bg left width:500px](images/rules.png)

# **Rule #10**

# Naming and Documentation


---

## 📛 Naming Convention

- Macros ➔ `TMPL_...`
- Structs ➔ `Tmpl_...`
- Functions ➔ `tmpl_...`

✅ Unique and collision-free

<!--
- Always prefix everything with project-specific name (e.g., TMPL).
- Prevents name clashes when multiple shared objects are loaded.
- Same rule for macros, structs, typedefs, enums, functions.
-->

---

## 🗂️ Header Organization

- Put headers under `tmpl/` **flat** directory
- Include headers using quotes:

```cpp
#include "tmpl/tmpl_api.h"
#include "tmpl/tmpl_types.h"
```

✅ Clean, avoids relative paths mess

<!--
- All public headers inside "tmpl/".
- Single flat structure, no nested subfolders unless necessary.
- Use `#include "tmpl/..."` always, not relative paths like `../tmpl.h`.
- Easy for users, easy for packaging.
-->

---

## 🛡️ Function Documentation Checklist

- Initialization requirements?
- Sync or async behavior?
- Who allocates/frees memory?
- Memory lifetime rules
- Thread safety rules
- Blocking behavior?


<!--
- Must clearly say if a function can be called anytime or only after create/init.
- Must state if function blocks (e.g., network wait) or is instant.
- Must define exactly who allocates memory and who frees it.
- Memory lifetime must be documented clearly (e.g., valid until next call, needs manual free, etc.)
- Say if function is thread-safe (can be called concurrently) or not.
- State if function may internally wait (e.g., mutex locks, IO wait).
-->

---


![bg left width:500px](images/rules.png)

# 📋 Shared Object Rules (Summary)

---


1. POD types (✅ `struct {...}` / ❌ `class with ctor`)
2. Fixed-width types (✅ `uint8_t` / ❌ `int`)
3. Packed layout (✅ `#pragma pack(1)`)
4. Export minimal API (✅ needed only)
5. `extern "C"` linkage (✅ clean names)
6. Create/Destroy handles (✅ no globals)
7. Clear memory ownership (✅ who allocates/frees)
8. Return error codes (✅ `0 = OK`)
9. Version API (✅ `tmpl_get_version()`)
10. Prefix names and document (✅ `TMPL_`, memory, threads)

---


# 🔄 Endianness

<img src="images/endianness.webp" width="700" style="display: flex;" />

---

# 🔍 What Is Endianness?

- 📦 Byte order in multi-byte types (e.g., `int`, `float`)

### 🧠 Two types:
- **Little-Endian** → LSB first (x86, ARM)
- **Big-Endian** → MSB first (network, PowerPC)

---

# 🧪 Detect Endianness 

```cpp
void PrintEndianness() {
    unsigned int x = 0x12345678;
    unsigned char *byte = (unsigned char *)&x;

    if (byte[0] == 0x78) {
        printf("Little-Endian\n");
    } else if (byte[0] == 0x12) {
        printf("Big-Endian\n");
    }
}

```
---

# 🧪 Detect Endianness (C++20)

```cpp
#include <bit>
#include <iostream>

int main() {
    if constexpr (std::endian::native == std::endian::little) {
        std::cout << "Little Endian\n";
    } else if constexpr (std::endian::native == std::endian::big) {
        std::cout << "Big Endian\n";
    } else {
        std::cout << "Mixed Endian (Rare CPU!)\n";
    }
}
```

---


# 🌍 Endianness in Practice

- Most modern CPUs are **little-endian**
- Many (like ARM, RISC-V) can switch endianness
- 🧪 Always check network/packed binary formats!


---


## Example : curl

```cpp
CURL_EXTERN CURL *curl_easy_init(void);
CURL_EXTERN CURLcode curl_easy_setopt(CURL *curl, CURLoption option, ...);
CURL_EXTERN CURLcode curl_easy_perform(CURL *curl);
CURL_EXTERN void curl_easy_cleanup(CURL *curl);

CURL_EXTERN const char *curl_easy_strerror(CURLcode);

```
--- 
