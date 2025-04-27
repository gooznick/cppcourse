#include "tmpl/tmpl_api.h"
#include <cassert>
#include <iostream>

int main() {
    // Create handle
    const char* ini_filename = "test.ini";
    Tmpl_Handle handle = tmpl_Create(ini_filename);
    assert(handle.impl != nullptr);

    // Test Add
    int sum = tmpl_Add(handle, 3, 7);
    assert(sum == 10);
    assert(tmpl_GetLastError(handle) && std::string(tmpl_GetLastError(handle)) == "OK");

    // Test Mul
    int prod = tmpl_Mul(handle, 4, 5);
    assert(prod == 20);
    assert(std::string(tmpl_GetLastError(handle)) == "OK");

    // Test Pow
    int p = tmpl_Pow(handle, 2, 5);
    assert(p == 32);
    assert(std::string(tmpl_GetLastError(handle)) == "OK");

    // Test Div normal
    Tmpl_DivInputs div_inputs = {10.0f, 2.0f};
    float result = 0.0f;
    tmpl_result_t div_ret = tmpl_Div(handle, &div_inputs, &result);
    assert(div_ret == TMPL_OK);
    assert(result == 5.0f);
    assert(std::string(tmpl_GetLastError(handle)) == "OK");

    // Test Div by zero
    Tmpl_DivInputs div_zero_inputs = {10.0f, 0.0f};
    float bad_result = 0.0f;
    tmpl_result_t div_zero_ret = tmpl_Div(handle, &div_zero_inputs, &bad_result);
    assert(div_zero_ret == TMPL_ERR_DIV_ZERO);
    assert(std::string(tmpl_GetLastError(handle)) == "Division by zero");

    // Destroy handle
    tmpl_Destroy(&handle);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
