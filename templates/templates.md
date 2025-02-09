---
title: Practical Usage of Templates in C++
author: eranbu
date: 2025-02-02
theme: default
---

# Introduction

- Templates are a key feature of C++ that enable generic programming.
- They allow writing code that works with different types while avoiding redundancy.
- Used extensively in the STL and custom libraries.

---

# What are Templates?

- Templates allow defining functions and classes that operate on generic types.
- **Types are determined at compile-time.**
- Avoids writing multiple versions of the same code for different data types.

Example:
```cpp
template <typename T>
T add(T a, T b) {
    return a + b;
}

int main() {
    std::cout << add(2, 3) << "\n";        // int
    std::cout << add(2.5, 3.5) << "\n";    // double
}
```

---

# Class Templates

- Can define classes that work with any data type.
- Used in STL containers like `std::vector`, `std::map`, etc.

Example:
```cpp
template <typename T>
class Box {
    T value;
public:
    Box(T val) : value(val) {}
    void show() { std::cout << "Value: " << value << "\n"; }
};

int main() {
    Box<int> intBox(10);
    Box<std::string> strBox("Hello");
    intBox.show();
    strBox.show();
}
```

---

# Template Specialization

- Allows defining special behavior for specific types.

Example:
```cpp
template <typename T>
class Printer {
public:
    void print(T value) { std::cout << value << "\n"; }
};

// Specialization for `std::string`
template <>
class Printer<std::string> {
public:
    void print(std::string value) { std::cout << "String: " << value << "\n"; }
};
```

---

# Pros of Templates

✅ **Code Reusability** – Write generic code once, use it for multiple types.

✅ **Type Safety** – Errors are caught at compile time instead of runtime.

✅ **Performance** – No runtime overhead; all type handling is resolved at compile time.

✅ **Extensibility** – Can be easily adapted for new types and functionalities.

---

# Cons of Templates

❌ **Compilation Time** – Increased due to template instantiations.

❌ **Error Messages** – Can be verbose and difficult to understand.

❌ **Code Bloat** – Each template instantiation generates a separate version in binary.

❌ **Limited Debugging** – Debuggers may struggle with templated code.

---

# Best Practices

- Use templates when **code duplication** exists for different types.
- Prefer **type deduction** (e.g., `auto`, `decltype`) to avoid unnecessary complexity.
- Use **template specialization** carefully to handle edge cases.
- Limit deep template nesting to **avoid complex error messages.**

--- 

# `if constexpr` instead of specialization and enable_if

```cpp
#include <iostream>
#include <type_traits>

template <typename T>
void processPixel(T& pixel) {
    if constexpr (std::is_integral_v<T>) {
        // Process integer pixels (e.g., normalize to [0, 255])
        pixel = (pixel > 255) ? 255 : pixel;
    } else if constexpr (std::is_floating_point_v<T>) {
        // Process floating-point pixels (e.g., clamp to [0.0, 1.0])
        pixel = (pixel > 1.0) ? 1.0 : pixel;
    } else {
        std::cout << "Unsupported pixel type!\n";
    }
}
```


---

# Examples

- Computations for different types of images
- Read/Parse different types of messages
