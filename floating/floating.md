---
marp: true
---

# Floating-Point Numbers in C++

## Introduction

Floating-point numbers in C++ follow the IEEE 754 standard and are used for representing real numbers with finite precision.

```cpp
#include <iostream>
int main() {
    float a = 3.14f;
    double b = 3.14;
    long double c = 3.14L;
    std::cout << "Float: " << a << "\n";
    std::cout << "Double: " << b << "\n";
    std::cout << "Long Double: " << c << "\n";
}
```
---

## Binary Representation

<img src="images/IEEE-754.jpg"  />

---

## Accuracy & Precision

Floating-point numbers are **not exact** due to finite precision.

```cpp
#include <iostream>
int main() {
    double a = 0.1 + 0.2;
    if (a == 0.3)
        std::cout << "0.1 + 0.2 == 0.3\n";
    else
        std::cout << "0.1 + 0.2 != 0.3 (Actual: " << a << ")\n";
}
```

**Why?** Floating-point arithmetic introduces small rounding errors.

---

## Deterministic Computation

Floating-point calculations can yield different results on different hardware due to **compiler optimizations and floating-point settings**.

```cpp
#include <iostream>
#include <iomanip>
int main() {
    double a = 1.0 / 3.0;
    std::cout << std::setprecision(20) << a << "\n";
}
```

To ensure determinism:
- Use `-ffloat-store` (GCC, Clang)
- Avoid extended precision registers
- Disable aggressive optimizations (`-ffast-math` can introduce non-determinism!)

---

## Compilation Flags for Floating-Point Precision

### **Linux (GCC/Clang)**
- `-ffast-math` → Enables aggressive optimizations (not always IEEE compliant)
- `-fno-math-errno` → Removes checks for math errors (faster execution)
- `-mfpmath=sse` → Uses SSE instead of x87 FPU

### **Windows (MSVC)**
- `/fp:fast` → Fast but may introduce non-determinism
- `/fp:precise` → IEEE 754 compliant calculations
- `/arch:AVX` → Uses AVX instructions for floating-point operations

```cpp
#pragma STDC FENV_ACCESS ON // Ensures precise floating-point behavior
```

---

## SSE/AVX Floating-Point Computations

SIMD (Single Instruction, Multiple Data) operations allow parallel floating-point calculations.

```cpp
#include <immintrin.h>
#include <iostream>
int main() {
    __m128 a = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
    __m128 b = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
    __m128 result = _mm_add_ps(a, b);
    float res[4];
    _mm_storeu_ps(res, result);
    std::cout << "Result: " << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << "\n";
}
```

---

## Floating-Point Exceptions

Enable floating-point exceptions to catch invalid computations.

```cpp
#include <cfenv>
#include <iostream>
int main() {
    std::feclearexcept(FE_ALL_EXCEPT);
    double a = 1.0 / 0.0; // Infinity
    if (fetestexcept(FE_DIVBYZERO))
        std::cout << "Division by zero detected!\n";
}
```


---

## Surprising Floating-Point Computations

```cpp
#include <iostream>
int main() {
    double a = 1e16;
    double b = 1.0;
    std::cout << "(a + b) - a = " << ((a + b) - a) << "\n";
}
```

Why? **Loss of precision!** `1.0` is too small relative to `1e16`.

---

## Why Not Use Floating Points for Cycle Counters?

Using floating-point for time measurements can lead to inaccuracies due to precision loss.

```cpp
#include <chrono>
#include <iostream>
int main() {
    auto start = std::chrono::high_resolution_clock::now();
    // Some computation
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns\n";
}
```

Use **integer-based timers** instead!

---



## ULP (Unit in Last Place) & How to Compute It

- **ULP** = Smallest difference between two floating-point numbers.

```cpp
#include <iostream>
#include <cmath>
int main() {
    double x = 1.0;
    std::cout << "Nextafter: " << std::nextafter(x, 2.0) - x << "\n";
}
```

In Python:
```python
import numpy as np
ulp = np.nextafter(1.0, 2.0) - 1.0
print(ulp)
```


---

## NaN (Not-a-Number) and Infinity in C++

### ** What is NaN?**
- **NaN (Not-a-Number)** represents an **undefined or unrepresentable value** in floating-point computations.
- Examples of NaN:
  - `0.0 / 0.0` (Indeterminate form)
  - `sqrt(-1.0)` (Square root of a negative number)
  - `log(-1.0)` (Logarithm of a negative number)

```cpp
#include <iostream>
#include <cmath>
#include <limits>
int main() {
    double nan1 = 0.0 / 0.0;
    double nan2 = std::sqrt(-1.0);
    std::cout << "NaN example 1: " << nan1 << "\n";
    std::cout << "NaN example 2: " << nan2 << "\n";
    std::cout << "Is NaN? " << std::isnan(nan1) << "\n";
}
```

### ** What is Infinity (`inf`)?**
- **Infinity (`inf`)** occurs when a number exceeds the largest representable value or results from division by zero.
- Examples:
  - `1.0 / 0.0` → `+inf`
  - `-1.0 / 0.0` → `-inf`
  - `std::numeric_limits<double>::max() * 2` (Exceeds max representable number)

```cpp
#include <iostream>
#include <cmath>
#include <limits>
int main() {
    double pos_inf = 1.0 / 0.0;
    double neg_inf = -1.0 / 0.0;
    double overflow = std::numeric_limits<double>::max() * 2;
    std::cout << "Positive Infinity: " << pos_inf << "\n";
    std::cout << "Negative Infinity: " << neg_inf << "\n";
    std::cout << "Overflow Example: " << overflow << "\n";
    std::cout << "Is Inf? " << std::isinf(pos_inf) << "\n";
}
```

### ** NaN and Inf in Computations**
- **NaN propagates**: Any arithmetic operation involving NaN **results in NaN**.
- **Infinity follows arithmetic rules**:
  - `inf + inf = inf`
  - `inf - inf = NaN` (undefined)
  - `inf * 0 = NaN` (undefined)
  - `inf / inf = NaN` (undefined)

```cpp
#include <iostream>
#include <cmath>
int main() {
    double nan_value = std::sqrt(-1.0);
    double inf_value = 1.0 / 0.0;
    std::cout << "NaN + 1: " << (nan_value + 1) << "\n";
    std::cout << "Inf * 0: " << (inf_value * 0) << "\n";
    std::cout << "Inf - Inf: " << (inf_value - inf_value) << "\n";
}
```

### ** Handling NaN and Inf in C++**
- Use `std::isnan(x)` to check if a value is NaN.
- Use `std::isinf(x)` to check if a value is infinity.
- Be cautious when performing **comparisons**:
  - `NaN == NaN` is always **false**!
  - `NaN != NaN` is **true**.

```cpp
#include <iostream>
#include <cmath>
int main() {
    double nan_value = std::sqrt(-1.0);
    std::cout << "Is NaN == NaN? " << (nan_value == nan_value) << "\n";
    std::cout << "Is NaN != NaN? " << (nan_value != nan_value) << "\n";
}
```
---


---

## Common Floating-Point Exceptions
Floating-point exceptions include:
- **Division by zero** (`FE_DIVBYZERO`)
- **Overflow** (`FE_OVERFLOW`)
- **Underflow** (`FE_UNDERFLOW`)
- **Invalid operation (NaN)** (`FE_INVALID`)
- **Inexact result (Rounding errors)** (`FE_INEXACT`)

```cpp
#include <iostream>
#include <cfenv>
#include <cmath>

int main() {
    std::feclearexcept(FE_ALL_EXCEPT);
    double a = 1.0 / 0.0; // Division by zero
    if (fetestexcept(FE_DIVBYZERO))
        std::cout << "Floating-point exception: Division by zero!\n";
}
```

---

## Enabling Floating-Point Exceptions
By default, floating-point exceptions are **silent**. To catch them, we need to enable trapping.

```cpp
#include <iostream>
#include <cfenv>
#include <cmath>

#pragma STDC FENV_ACCESS ON  // Ensure floating-point environment access

int main() {
    std::fesetenv(FE_DFL_ENV); // Set default floating-point environment
    std::feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
    double a = 1.0 / 0.0; // This will now raise an exception
    return 0;
}
```

---

## Handling Floating-Point Exceptions with Signal Handling
C++ allows handling FPEs using signals (`SIGFPE`).

```cpp
#include <iostream>
#include <csignal>
#include <cmath>

void signal_handler(int signal) {
    std::cerr << "Floating-point exception caught!\n";
    exit(1);
}

int main() {
    std::signal(SIGFPE, signal_handler);
    double a = 1.0 / 0.0; // This triggers SIGFPE
}
```

---

## Platform-Specific Handling (Windows & Linux)
### **Linux/GCC & Clang**
Use `feenableexcept()` to enable FPE trapping:
```cpp
#include <fenv.h>
fesetenv(FE_DFL_ENV);
feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
```
### **Windows/MSVC**
Use `_controlfp()`:
```cpp
#include <float.h>
_controlfp_s(NULL, 0, _EM_ZERODIVIDE | _EM_INVALID | _EM_OVERFLOW);
```

---

# Summary
- Floating-point numbers have precision limits.
- Use appropriate compiler flags.
- Prefer integer-based time measurement.
- Be aware of floating-point surprises!



