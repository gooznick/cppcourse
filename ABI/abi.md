---
title: abi
author: eranbu
date: 12/2024
marp: true
backgroundColor: #fff
backgroundImage: linear-gradient(to right, #fff, #f0f0f0)

---

## ABI
- **Understanding ABI:** Bridging C/C++ and Python, Communication and More.

---

## **What is ABI?**
- **Definition**: ABI defines the binary interface between two program modules.

- **Key Components**:
  - Data types and alignment
  - Object layout in memory
  - Function calling conventions
  - Name mangling in C++

---

## **Key Topics Covered**
1. Alignment and Padding
2. Calling Conventions
3. Data Types and Compatibility
4. Endianness
5. Interaction Tools: `ctypes`, `cffi`
6. Reflection in C++: Using Boost
7. Practical Examples of Data Transfer Between C/C++ and Python
8. Additional Topics:
   - Handling Complex Data Structures
   - Error Handling Across Languages

---

## **Alignment and Padding**
- **Definition**: Ensures memory alignment for efficient CPU access.
- **Why Important**: Impacts performance and data transfer compatibility.
- **C/C++ Specifics**:
  - Structures may include padding to align data.
  - Use `#pragma pack` or `alignas` for custom control.
- **Python Considerations**:
  - Python's `struct` module respects alignment rules for binary data.

---

## **Example**

```cpp
struct MyStruct {
    char a;    // 1 byte
    int b;     // 4 bytes
    short c;   // 2 bytes
};
```

---

## **Example**

```cpp
struct MyStruct {
    char a;    // 1 byte
    int b;     // 4 bytes
    short c;   // 2 bytes
};
```

---

## In code

```cpp
int main() {
    std::cout<<"Size of struct:" << sizeof( MyStruct) << std::endl;
    std::cout<<"Offset of a   :" << offsetof( MyStruct, a) << std::endl;
    std::cout<<"Offset of b   :" << offsetof( MyStruct, b) << std::endl;
    std::cout<<"Offset of c   :" << offsetof( MyStruct, c) << std::endl;
    return 0;
}
```

```
Size of struct:12
Offset of a   :0
Offset of b   :4
Offset of c   :8
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

## Slide 5: **Calling Conventions**
- **Definition**: Rules for function arguments and return values.
- **Examples**:
  - **Cdecl**: Default in C/C++, caller cleans up.
  - **Stdcall**: Used in Windows APIs, callee cleans up.
  - **Fastcall**: Arguments passed via registers for speed.
- **Python Interoperability**:
  - `ctypes` and `cffi` respect calling conventions when invoking shared libraries.

---

## Slide 6: **Data Types and Compatibility**
- **C/C++ vs Python**:
  - Integer sizes (`int`, `long`, `size_t`) vs Python's dynamic `int`.
  - Strings (`char*`, `std::string`) vs Python's `str`.
- **Bridging the Gap**:
  - Use `ctypes` or `cffi` to define equivalent data types in Python.
  - Avoid pitfalls: Signed vs unsigned types, floating-point precision.

---

## Slide 7: **Endianness**
- **What is Endianness?**
  - Big-endian: Most significant byte first.
  - Little-endian: Least significant byte first.
- **Challenges**:
  - Mismatch can lead to incorrect data interpretation.
- **Solutions**:
  - C: Use functions like `htons`, `htonl`.
  - Python: Use `struct` module's endianness specifiers (`>` for big-endian, `<` for little-endian).

---

## Slide 8: **Tools for Interaction**
1. **ctypes**:
   - Dynamic loading of shared libraries.
   - Define data types and call functions directly.
   - Example: `ctypes.CDLL("library.so").function_name()`
2. **cffi (C Foreign Function Interface)**:
   - Define C declarations in Python.
   - Provides both ABI and API-level interaction.
3. **Boost.Python**:
   - Bridges C++ and Python with minimal boilerplate.
   - Example: Exposing C++ classes to Python.

---

## Slide 9: **Reflection in C++**
- **Boost.Reflection**:
  - Simplifies runtime introspection in C++.
  - Example: Accessing class methods and properties dynamically.
- **Practical Use Case**:
  - Pass dynamic objects from Python to C++ for processing.

---

## Slide 10: **Data Transfer Between C/C++ and Python**
- **Examples**:
  - Passing primitive types: `int`, `float`, `char`.
  - Sharing complex types: Arrays, structs, and objects.
- **Memory Management**:
  - Ensure proper ownership (malloc/free in C, Python's GC).
  - Use `PyCapsule` for opaque data sharing.

---

## Slide 11: **Additional Topics**
1. Handling Unicode Strings:
   - Conversion between `wchar_t` and Python strings.
2. Error Handling:
   - Propagate errors from C++ exceptions to Python.
3. Performance Considerations:
   - Reduce overhead with `cffi` ABI mode.
   - Use efficient data structures for bulk transfer.

---

## Slide 12: **Practical Example**
**Title**: Transferring a Struct Between C++ and Python  
- **C++ Struct**:
```cpp
struct Data {
    int id;
    float value;
};
```
- **Python Interaction**:
```python
from ctypes import *
class Data(Structure):
    _fields_ = [("id", c_int), ("value", c_float)]
```

---

## Slide 13: **Conclusion**
- **Key Takeaways**:
  - ABI ensures smooth interoperation.
  - Understanding alignment, calling conventions, and endianness is critical.
  - Tools like `ctypes` and `Boost.Python` simplify the process.
- **Final Thought**:
  - A well-designed ABI bridges the gap between performance and compatibility.

---

Would you like more details or code snippets for any section?