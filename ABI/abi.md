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

---

## 🧠 What is ABI?

- ABI = **Application Binary Interface**
- Defines how binaries interact at runtime
- Must be consistent for:
  - 🔌 Communication
  - 📦 Serialization
  - 🧩 Shared libraries (`.dll`, `.so`)


---


## 🔍 ABI Topics

1. 📐 Data layout & alignment  
2. 🛠️ Calling conventions  
3. 🔄 Type compatibility  
4. 🧭 Endianness  
5. 🔍 Reflection in C++

---

## 🧩 DLL Topics

1. 🧠 Memory allocation rules  
2. 🚨 Exception handling across boundaries  
3. ✅ Create/Destroy 


---

## 🧮 Primitive Types: Overview

- C++ only defines **minimum** sizes
- Actual sizes vary across platforms
- Most compilers use consistent defaults

---

## 📏 Integer Types

| Type     | Linux | Windows | Notes                |
|----------|-------|---------|----------------------|
| `char`   | 1 B   | 1 B     | Always 1 byte        |
| `bool`   | 1 B   | 1 B     | True / false         |
| `short`  | 2 B   | 2 B     | 16-bit integer       |
| `int`    | 4 B   | 4 B     | 32-bit integer       |                                                          |

---

## 📐 Platform Differences

| Type          | Linux   | Windows | Notes                                   |
|---------------|---------|---------|-----------------------------------------|
| `long`        | 8 B     | 4 B     | Major difference!                       |
| `long long`   | 8 B     | 8 B     | Same on both                            |
| `float`       | 4 B     | 4 B     | IEEE 754                                |
| `double`      | 8 B     | 8 B     | IEEE 754                                |
| `long double` | 16 B    | 8 B     | Extra precision on Linux                |
| `void*`       | 8 B     | 8 B     | Pointer = 64-bit                        |
| `size_t`      | 8 B     | 8 B     | Based on architecture                   |
| `wchar_t`     | 4 B     | 2 B     | UTF-32 vs UTF-16                        |

---

# 📌 Pointers in ABI & APIs

- ❌ **Not portable across processes**
  - Valid only within same address space
  - Size may vary (`32` vs `64` bit)

- 🧩 **OK in DLL APIs** (same process)
  - But: define who owns / frees it!

- ❌ **Not valid for:**
  - 🛰️ Serialization
  - 🌐 Network communication


---

## 🧪 Practical Code Check

```cpp
#include <climits>

static_assert(CHAR_BIT == 8, "Expected 8-bit bytes");
static_assert(sizeof(long) == 8, "Expected 8-bytes long");

```

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


# 📦 POD: Plain Old Data

- C++ types compatible with C layout
- ✅ No constructors, destructors, inheritance, or virtual functions
- 🔧 Easy to copy, serialize, or share across binaries


---

# Example 

```cpp
struct A1
{
    void foo();
};
```

```cpp
struct A2
{
    virtual void foo();
};
```

What is `sizeof(A1)` and `sizeof(A2)` ?

<!-- 
A1 - 1 (minimal)
A2 - 8 (pointer)
-->


---

# ❓ What if You Export This?

```cpp
DLL_API void trk_Create(const std::string& ini_file);
DLL_API void trk_Destroy();
DLL_API void trk_Track();
```

⚠️ Is std::string layout the same across compilers?
🧩 ABI mismatch risk!


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


# 📞 Calling Conventions

- 🧾 Define how args/return values move between functions

### Common Types:
- `cdecl`: Caller cleans up
- `stdcall`: Callee cleans up (Windows APIs)
- `fastcall`: Args passed via registers


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


# 🔢 Bitfields in C++

- Define fields smaller than a full byte or word
```cpp
struct Flags {
    unsigned int a : 1;
    unsigned int b : 3;
    unsigned int c : 4;
};
```

✅ Saves space  
❌ Layout is compiler- and platform-dependent

---

# ⚠️ Bitfields and ABI

- ❌ No standard layout or packing rules
- ⚠️ Affected by:
  - Endianness
  - Alignment
  - Compiler version/settings


🧩 Prefer `uint8_t` + masks for portable layouts

---


![bg](images/dont.webp)

<!-- 
Bad practice
-->

---

# ✅ ABI Compatibility: Do's

- Use only **POD** types (Plain Old Data)  
- Use fixed-size types from `<cstdint>` (e.g. `uint32_t`)  
- Enforce structure layout with `#pragma pack(1)`  
- Ensure **same endianness** across systems  
- Keep layout predictable & stable

---

# ⚠️ ABI Compatibility: Don'ts

- ❌ Don't use `std::string`, `std::vector`, virtual functions
- ❌ Don't assume compiler will pad the same way
- ❌ Try avoiding bitfields in public interfaces  

---

# ⚠️ DLL Issues

- ❌ Never throw exceptions across DLL boundaries  
- ✅ Always catch exceptions before they escape
- 🔁 Memory Ownership:
  - Passing pointers between modules is dangerous
  - DLL should both **allocate** and **deallocate** its own memory

---

# 💾 Memory in DLLs

* Returning error message:
   * `const char* GetErrorMessage()`
   * `void GetErrorMessage(size_t size, char* message)`

* Who allocates? 
* Who deallocates? When ?

---

# 🚨 DLL Exception Rules

- Never let a `throw` escape across a DLL boundary
- Exception ABI may vary between:
  - Compilers
  - Runtime versions
  - Debug vs. Release
- Behavior is **undefined or crash-prone**

---

# 🧪 Exception Handling Demo

```cpp
// In DLL
extern "C" void do_work() {
    try {
        throw std::runtime_error("Fail");
    } catch (const std::exception& e) {
        log_error(e.what());
    }
}
```

✅ Catch everything inside  
✅ Expose only error codes or strings


---

# 🧱 Create / Destroy Pattern

- Common for DLLs and C APIs
- Keeps ABI stable using an **opaque pointer**

```cpp
struct MyObject;

MyObject* MyDll_Create(const char* inifile);
void MyDll_Destroy(MyObject& obj);

// functions
void MyDll_DoSomething(MyObject* obj);
```

---

# 🧼 Opaque Handle Benefits

- 🛡️ Hides internal structure
- 🔗 Avoids C++ classes dependency
- ♻️ Clean lifecycle: `Create()` + `Destroy()`

---


# 🔄 C++ RAII Wrapper

```cpp
class MyDllWrapper {

public:
    MyDllWrapper() {
        obj = MyDll_Create();
    }

    ~MyDllWrapper() {
        MyDll_Destroy(obj);
    }

    void DoSomething() {
        MyDll_DoSomething(obj);
    }
private:
    MyObject* m_obj;
};
```


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

## Example : clang

```cpp
CINDEX_LINKAGE const char * 	clang_getCString (CXString string) 

// Retrieve the character data associated with the given string.

// The returned data is a reference and not owned by the user. 
// This data is only valid while the CXString is valid. 
// This function is similar to std::string::c_str().

CINDEX_LINKAGE void 	        clang_disposeString (CXString string)
// Free the given string set.


```
---

## Example : zlib

```cpp
ZEXTERN const char * ZEXPORT gzerror OF((gzFile file, int *errnum));
/*
     Return the error message for the last error which occurred on file.
   errnum is set to zlib error number.  If an error occurred in the file system
   and not in the compression library, errnum is set to Z_ERRNO and the
   application may consult errno to get the exact error code.

     The application must not modify the returned string.  Future calls to
   this function may invalidate the previously returned string.  If file is
   closed, then the string previously returned by gzerror will no longer be
   available.

     gzerror() should be used to distinguish errors from end-of-file for those
   functions above that do not distinguish those cases in their return values.
*/

```
---

# 📚 DLL API Documentation Checklist

- 🔄 **Synchronous or Asynchronous?**  
  Does the function return immediately or run in the background?

- 🧠 **Memory Model**  
  - Who allocates/free memory?  
  - Can I hold a pointer? For how long?

- 🧭 **Call Order**  
  - Must I call `Init()` before `DoWork()`?  
  - Do I need to `Destroy()` or `Release()`?

---

- 🚨 **Error Reporting**  
  - How do I detect errors?  
  - `GetLastError()`, return codes, or `GetLastErrorMessage()`?

- 📦 **Thread Safety**  
  - Can I call it from multiple threads?


---

## **Reflection in C++**

**Reflection** is the ability of a program to introspect and possibly modify its structure and behavior at runtime or compile time.

* Some macro manipulation
* Boost.PFR
* Other libraries

---

## Boost.PFR

```cpp
#include <boost/pfr.hpp>
#include <iostream>

struct Person {
    std::string name;
    int age;
};

int main() {
    Person p{"Alice", 25};
    boost::pfr::for_each_field(p, [](const auto& field) {
        std::cout << field << '\n';
    });
}

```

---

# ❓ Q: What happens if you change the order of fields in a struct that is used in a shared library?

- `①` Nothing, the compiler automatically adjusts offsets.  
- `②` It may break ABI compatibility because offsets change.  
- `③` The linker will resolve the new field order dynamically.  
- `④` Only affects Windows, not Linux.  

<!---
✅ **Answer:** **② It may break ABI compatibility because offsets change.**  
🔍 **Explanation:** Changing the field order alters the memory layout, which breaks binary compatibility for programs compiled with the old layout.
-->

---

# ❓ Q: Which of these C++ features does NOT affect ABI?

- `①` Virtual functions  
- `②` Inline functions  
- `③` Name mangling  
- `④` Static local variables  

<!---
✅ **Answer:** **④ Static local variables**  
🔍 **Explanation:** Virtual functions change vtable layout, inline functions may affect linking, and name mangling affects symbol resolution.  
Static local variables only impact runtime behavior but do not affect binary compatibility.
-->

---

# ❓ Q: What is the most reliable way to maintain ABI compatibility in a shared library?

- `①` Use `extern "C"` for exported symbols.  
- `②` Never change struct layouts, vtables, or function signatures.  
- `③` Use symbol versioning (`-Wl,--version-script`).  
- `④` All of the above.  

<!---
✅ **Answer:** **④ All of the above.**  
🔍 **Explanation:** ABI compatibility requires careful management of symbols, function signatures, and struct layouts, along with symbol versioning when necessary.
-->

---

# ❓ Q: Which of the following changes in a shared library **DOES NOT** break ABI?

- `①` Changing a function parameter from `int` to `long`  
- `②` Adding a new function at the end of the `.so`  
- `③` Removing an unused function  
- `④` Changing the order of struct fields  

<!---
✅ **Answer:** **② Adding a new function at the end of the `.so`**  
🔍 **Explanation:** Adding new functions does not break ABI as long as existing functions and their signatures remain unchanged.
-->

---

# ❓ Q: Which tool helps check exported symbols in a shared object in Linux?

- `①` `ldd`  
- `②` `nm -D`  
- `③` `objdump -t`  
- `④` `readelf -h`  

<!---
✅ **Answer:** **② `nm -D`**  
🔍 **Explanation:** `nm -D` lists all **dynamically exported symbols** in a shared library (`.so`).
-->

---

# ❓ Q: Why do different compilers (GCC, Clang, MSVC) generate incompatible binaries?

- `①` Different function calling conventions.  
- `②` Different name mangling schemes.  
- `③` Different vtable layouts.  
- `④` All of the above.  

<!---
✅ **Answer:** **④ All of the above.**  
🔍 **Explanation:** Each compiler has **different ABI rules** for function calling, name mangling, and class vtable layouts, making cross-compiler compatibility difficult.
-->

---


 
