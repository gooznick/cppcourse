#pragma once

#include <memory>

class MyClassImpl;  // Forward declaration

class MyClass {
public:
    MyClass(const std::string& name);
    ~MyClass();  // Required to properly delete the pimpl object
    void printName();

private:
    std::unique_ptr<MyClassImpl> pImpl;  // Pointer to implementation
};

