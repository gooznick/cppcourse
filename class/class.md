---
marp: true
---


# C++ Classes in Practice

## Introduction
C++ classes provide a powerful way to structure programs, offering **encapsulation, resource management, and reusability**. This presentation covers practical aspects of C++ classes, including **access control, exception alternatives, initialization patterns, RAII, and design patterns**.

---

## Public vs. Private Members

### **Access Specifiers:**
- **`public`**: Accessible from anywhere.
- **`private`**: Only accessible inside the class.
- **`protected`**: Accessible inside the class and its derived classes.

```cpp
class Example {
private:
    int hiddenValue;
public:
    void setValue(int val) { hiddenValue = val; }
    int getValue() { return hiddenValue; }
};
```

### **Encapsulation Benefits:**
- Prevents unintended modifications.
- Enables controlled access.
- Improves maintainability.

---

## Classes in practice

```cpp
#pragma once

class RectangleSearchParams {
public:
    int threshold;  // Brightness threshold (0-255)
    int minWidth;   // Minimum width of the rectangle
    int minHeight;  // Minimum height of the rectangle
    bool useAdaptiveThreshold; // Whether to use adaptive thresholding

    RectangleSearchParams(int th = 200, int w = 50, int h = 50, bool adaptive = false)
        : threshold(th), minWidth(w), minHeight(h), useAdaptiveThreshold(adaptive) {}
};

```

---

```cpp
#pragma once

class BrightRectangleDetector {
private:
    RectangleSearchParams params;  // Search parameters

public:
    explicit BrightRectangleDetector(const RectangleSearchParams& searchParams);

    cv::Rect findBrightRectangle(const cv::Mat& image) const;
private:

    bool isBrightRegion(const cv::Mat& region) const;
};

```


---

## RAII (Resource Acquisition Is Initialization)
RAII ensures that resources are acquired and released properly.

```cpp
#include <fstream>
class FileWrapper {
    std::ofstream file;
public:
    FileWrapper(const std::string& filename) { file.open(filename); }
    ~FileWrapper() { file.close(); }
};
```

### **RAII Benefits:**
✅ **Automatic Resource Management** (No leaks)  
✅ **Exception Safety**  
✅ **Simplifies Code**  

Sometimes one have to create the class on heap (std::unique_ptr) because of RAII.

cTor cannot return any value - it must throw exception.
--- 
## New feature == New class 

* Table reader
* Table downloader
* Matches consistency checker
* Environment variables static class
* Image overflow protected summer
* Executable runner

Each class can be tested, extended, exported easily.


---

# Basic data types

* Each applications needs its basic data type
  * Which data to store, where is it's memory
  * Thread safety for each operation
  * Complexity for each operation
  * Maybe serialization, debug prints, statistics, etc
  * msvc/gdb pretty print (natvis and gdb pretty printer)


---

# Forcing initialization order

```cpp
#include <iostream>

// Forward declarations
class Init2State;
class FullyInitialized;

// Step 1: Initial State (Only allows Init1)
class Init1State {
public:
    explicit Init1State() { std::cout << "Object created, Init1 required.\n"; }

    Init2State Init1();
};

// Step 2: Init2State (Only allows Init2)
class Init2State {
public:
    explicit Init2State() { std::cout << "Init1 done, Init2 required.\n"; }

    FullyInitialized Init2();
};


// Main class that starts with Init1State
class MyClass {
public:
    static Init1State Create() { return Init1State(); }
};

int main() {
    auto obj = MyClass::Create()
                   .Init1() // Must be called first
                   .Init2(); // Must be called second

    return 0;
}
```


---

# Singleton

```cpp
#include <iostream>

class Singleton {
private:
    static Singleton* instance;  // Static instance pointer

    // Private constructor to prevent direct instantiation
    Singleton() { std::cout << "Singleton Created\n"; }

public:
    // Delete copy constructor and assignment operator
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Static method to get the single instance
    static Singleton* getInstance() {
        if (!instance) {
            instance = new Singleton();
        }
        return instance;
    }

    void showMessage() { std::cout << "Singleton Instance Accessed\n"; }
};


```