---
marp: true
---

# Fixed-Point Numbers in C/C++

## Basic Types

C/C++ provides several fixed-point integer types:

```cpp
int a = 42;       // Typically 4 bytes
long b = 42L;     // Typically 4 bytes on Windows, 8 on Linux
short c = 42;     // Typically 2 bytes
char d = 'A';     // Typically 1 byte
```

Sizes vary by platform. Use `sizeof()` to check.

```cpp
#include <iostream>
int main() {
    std::cout << "Size of int: " << sizeof(int) << " bytes\n";
    std::cout << "Size of long: " << sizeof(long) << " bytes\n";
}
```

---

## int vs long in Linux and Windows

On **Linux (64-bit)**:
```cpp
sizeof(int) = 4;
sizof(long) = 8;
```
On **Windows (64-bit)**:
```cpp
sizeof(int) = 4;
sizof(long) = 4;
```

To ensure portability, use `int64_t` from `<cstdint>` for explicit 64-bit integers.

---

## What are `size_t` and `ptrdiff_t`?

- `size_t`: Used for sizes of objects, unsigned.
- `ptrdiff_t`: Difference between two pointers, signed.

```cpp
#include <iostream>
#include <cstddef>
int main() {
    int arr[10];
    size_t sz = sizeof(arr);
    ptrdiff_t diff = &arr[5] - &arr[0];
    std::cout << "Array size: " << sz << " bytes\n";
    std::cout << "Pointer difference: " << diff << " elements\n";
}
```

---

## Overflow and Underflow (Signed & Unsigned)

```cpp
#include <iostream>
#include <climits>
int main() {
    unsigned int u = 0;
    std::cout << "Unsigned underflow: " << (u - 1) << "\n";
    
    int s = INT_MAX;
    std::cout << "Signed overflow: " << (s + 1) << "\n";
}
```

On overflow, signed numbers have **undefined behavior**, while unsigned numbers wrap around.
( g++ Unsigned underflow: 4294967295, Signed overflow: -2147483648)

---

## `char`, `signed char`, `unsigned char`

```cpp
char c = -1;            // Undefined behavior if character is not 0-127 ascii value
signed char sc = -1;    // Always valid
unsigned char uc = 255; // Maximum value
```

`char` will be used to store ASCII characters, and not numbers.

Check `CHAR_MIN` and `CHAR_MAX` in `<limits.h>` to see if `char` is signed or unsigned.

---

## Division by Zero

```cpp
int a = 10, b = 0;
int c = a / b; // Undefined behavior
```

For floating-point division, `1.0 / 0.0` results in `+inf`.

In linux, the message will be "Floating point exception" (historical reasons).

### Division by zero - Compiler Behavior and Prevention

- **GCC/Clang**: Triggers a floating-point exception (FPE) and crashes.
- **MSVC**: Triggers an exception and may abort execution.

To prevent termination (linux):

```cpp
#include <csignal>
#include <iostream>

void signal_handler(int signal) {
    std::cout << "Caught signal: " << signal << " (Division by zero)\n";
}

int main() {
    signal(SIGFPE, signal_handler);
    int a = 10, b = 0;
    int c = a / b; // This will trigger the handler
}
```

---

## Windows 

```cpp
#include <iostream>
#include <windows.h>

int main() {
    __try {
        int a = 10, b = 0;
        int c = a / b; // Would normally crash
        std::cout << "Result: " << c << "\n";
    }
    __except (GetExceptionCode() == EXCEPTION_INT_DIVIDE_BY_ZERO ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
        std::cerr << "Caught divide by zero exception!\n";
    }
}
```

---

## Modulo by Zero

```cpp
int a = 10, b = 0;
int c = a % b; // Undefined behavior
```

Modulo by zero results in a runtime error (same hw exception as division by zero)

---
## Numeric Postfixes in C++ for Fixed-Point Types

In C++, different number suffixes can be used to specify various numeric types:

- **Fixed-Point and Integer Types:**
  - `123` → `int`
  - `123u` → `unsigned int`
  - `123l` → `long`
  - `123ul` → `unsigned long`
  - `123ll` → `long long`
  - `123ull` → `unsigned long long`

```cpp
#include <iostream>
int main() {
    auto a = 123;   // int
    auto b = 123u;  // unsigned int
    auto c = 123l;  // long
    auto d = 123ll; // long long
    auto e = 3.14f; // float
    auto f = 3.14L; // long double
}
```

---

## `numeric_limits` in C++

The `<limits>` header provides useful constants:

```cpp
#include <iostream>
#include <limits>
int main() {
    std::cout << "Max int: " << std::numeric_limits<int>::max() << "\n";
    std::cout << "Min int: " << std::numeric_limits<int>::min() << "\n";
    std::cout << "Is int signed? " << std::numeric_limits<int>::is_signed << "\n";
}
```

---

## Using c++17 to prevent underflow

```cpp

#include <iostream>
#include <type_traits>

template <typename T>
T safe_subtract(T a, T b) {
    if constexpr (std::is_unsigned_v<T>) {
        if (b > a) {
            std::cerr << "Underflow prevented! Returning 0.\n";
            return 0; // Prevent underflow for unsigned types
        }
    }
    return a - b; // Safe for signed types
}
```

---

## Libraries for Unlimited Precision Computations

- **GMP (GNU Multiple Precision)**: Arbitrary precision integers.
- **MPFR**: Floating-point arithmetic with precision control.
- **Boost.Multiprecision**: Supports arbitrary precision numbers in C++.

Example using GMP:

```cpp
#include <gmp.h>
#include <iostream>
int main() {
    mpz_t big;
    mpz_init(big);
    mpz_set_str(big, "123456789123456789", 10);
    mpz_mul_ui(big, big, 2);
    gmp_printf("%Zd\n", big);
    mpz_clear(big);
}
```

---

## Division by Zero with SSE/AVX

```cpp
#include <immintrin.h>
#include <iostream>
int main() {
    __m128 a = _mm_set1_ps(10.0f);
    __m128 b = _mm_set1_ps(0.0f);
    __m128 c = _mm_div_ps(a, b);
    float result[4];
    _mm_storeu_ps(result, c);
    std::cout << "SSE result: " << result[0] << "\n";
}
```

SSE/AVX operations return `+inf` or `-inf` for division by zero instead of causing an exception.

Note that the result is floating point.

---

## More on Fixed-Point Numbers

Fixed-point numbers store values with a fixed number of decimal places, useful in embedded systems.

Example of a fixed-point type using `int32_t`:

```cpp
#include <iostream>
#define SCALE 100
int main() {
    int32_t fixed_a = 2500; // Represents 25.00
    int32_t fixed_b = 1500; // Represents 15.00
    int32_t fixed_c = (fixed_a * fixed_b) / SCALE; // Fixed-point multiplication
    std::cout << "Fixed-Point Result: " << fixed_c / 100.0 << "\n";
}
```

Fixed-point arithmetic avoids floating-point precision issues at the cost of reduced flexibility.

---

## uint64_t is VERY LARGE

```python
# Given:
# - uint64_t max value: 2^64 - 1
# - Microseconds per year: 365.25 * 24 * 60 * 60 * 1,000,000

max_uint64 = 2**64 - 1
microseconds_per_year = 365.25 * 24 * 60 * 60 * 1_000_000

# Compute how many years uint64_t can store microseconds
years_supported = max_uint64 / microseconds_per_year
print(years_supported)
```

>>> 584542.0460906264

---

## ** Fixed-Point vs Floating-Point: When to Use?**
| **Criteria** | **Fixed-Point** | **Floating-Point** |
|-------------|----------------|-----------------|
| **Speed on modern CPUs** | ❌ Slower (division) | ✅ Faster (`*`, `/`) |
| **Accuracy (precision issues)** | ✅ More predictable | ⚠️ Rounding errors |
| ** SIMD (AVX, SSE)** | ❌ Limited | ✅ Highly optimized |

### **✅ Best Practices**
- **Use fixed-point only when you must** .
- **Prefer floating-point multiplication (`float a * b`)** over fixed-point division.
- **Avoid integer division (`int a / b`) whenever possible**—replace it with reciprocal multiplication.

---

# Summary

- Integer sizes vary by platform.
- `size_t` for sizes, `ptrdiff_t` for pointer differences.
- Unsigned overflows wrap; signed overflows are undefined.
- `char` can be signed or unsigned.
- Division/modulo by zero is undefined in C++.
- Use `numeric_limits` for type properties.
- SSE/AVX handles division by zero differently.

**Always check your assumptions when working with integers!**

