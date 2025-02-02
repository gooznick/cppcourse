---
title: Error Handling Options in C++
author: Your Name
date: 2025-02-02
theme: default
---

# Introduction

- Error handling is crucial in C++ to ensure robustness and reliability.
- C++ provides multiple ways to handle errors, each with trade-offs.
- Choosing the right strategy depends on performance, safety, and maintainability requirements.

---

# 1. Exception Handling

- **Uses `try`, `catch`, and `throw` to handle errors.**
- **Pros:**
  - Clean separation of error handling from main logic.
  - Supports error propagation across multiple layers.
- **Cons:**
  - Can be expensive due to stack unwinding.
  - Not suitable for real-time or embedded systems.

Example:
```cpp
#include <iostream>
#include <stdexcept>

void riskyFunction(bool fail) {
    if (fail) throw std::runtime_error("Something went wrong");
    std::cout << "Success!\n";
}

int main() {
    try {
        riskyFunction(true);
    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << "\n";
    }
    return 0;
}
```

---

# 2. Error Codes

- **Uses return values to indicate success or failure.**
- **Pros:**
  - No performance overhead from stack unwinding.
  - Works well in embedded and real-time systems.
- **Cons:**
  - Can clutter code with error checking.
  - Harder to enforce handling (caller may ignore error codes).

Example:
```cpp
#include <iostream>

enum class ErrorCode { SUCCESS, FAILURE };

ErrorCode riskyFunction(bool fail) {
    return fail ? ErrorCode::FAILURE : ErrorCode::SUCCESS;
}

int main() {
    if (riskyFunction(true) == ErrorCode::FAILURE) {
        std::cerr << "Error occurred!\n";
    }
    return 0;
}
```

---

# 3. `std::optional` for Error Handling

- **Encapsulates an optional return value, avoiding exceptions.**
- **Pros:**
  - No need for error codes or special values.
  - Clearer API semantics.
- **Cons:**
  - Requires explicit handling (`has_value()` or `value_or()`).

Example:
```cpp
#include <iostream>
#include <optional>

std::optional<int> riskyFunction(bool fail) {
    if (fail) return std::nullopt;
    return 42;
}

int main() {
    auto result = riskyFunction(true);
    if (result) {
        std::cout << "Success: " << *result << "\n";
    } else {
        std::cerr << "Error occurred!\n";
    }
    return 0;
}
```

---

# 4. `std::expected` (C++23)

- **Similar to `std::optional`, but carries error information.**
- **Pros:**
  - Returns both success and error values.
  - Provides structured error handling.
- **Cons:**
  - Requires C++23 support.

Example:
```cpp
#include <iostream>
#include <expected>

std::expected<int, std::string> riskyFunction(bool fail) {
    if (fail) return std::unexpected("Error: computation failed");
    return 42;
}

int main() {
    auto result = riskyFunction(true);
    if (result) {
        std::cout << "Success: " << *result << "\n";
    } else {
        std::cerr << "Failure: " << result.error() << "\n";
    }
    return 0;
}
```

---

# 5. `boost::outcome`

- **A third-party library for structured error handling.**
- **Pros:**
  - More powerful than `std::expected`.
  - Works in pre-C++23 projects.
- **Cons:**
  - Requires Boost library.

Example:
```cpp
#include <iostream>
#include <boost/outcome.hpp>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

outcome::result<int> riskyFunction(bool fail) {
    if (fail) return outcome::failure(std::error_code(1, std::generic_category()));
    return 42;
}

int main() {
    auto result = riskyFunction(true);
    if (result) {
        std::cout << "Success: " << result.value() << "\n";
    } else {
        std::cerr << "Error: " << result.error().message() << "\n";
    }
    return 0;
}
```

---

# Comparison Table

| Method          | Pros | Cons |
|----------------|------|------|
| Exceptions (`throw`) | Clean separation, automatic unwinding | High overhead, not real-time friendly |
| Error Codes (`enum`) | Simple, efficient | Clutters code, error handling is manual |
| `std::optional` | Avoids special values, clear API | No detailed error message |
| `std::expected` | Returns success or error, structured | Requires C++23 |
| `boost::outcome` | Powerful, flexible | Requires external Boost dependency |

---

# Conclusion

- Choose the best error handling strategy **based on performance, maintainability, and system constraints.**
- Use **exceptions** when errors are **rare and critical**.
- Use **error codes** or **`std::optional`** for performance-critical or real-time applications.
- Use **`std::expected` (C++23) or `boost::outcome`** for structured error handling.

**Understand your use case and pick the right tool! 🚀**
