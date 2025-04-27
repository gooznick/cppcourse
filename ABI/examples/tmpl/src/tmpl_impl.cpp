#include "tmpl/tmpl_api.h"
#include "tmpl_version.h"

#include <new>   // for std::nothrow
#include <cstdlib> // for nullptr
#include <cmath>  // for std::pow

// Internal class
class Tmpl_Impl {
public:
    tmpl_result_t last_error = TMPL_OK;

    int Add(int a, int b) {
        last_error = TMPL_OK;
        return a + b;
    }

    int Mul(int a, int b) {
        last_error = TMPL_OK;
        return a * b;
    }

    int Pow(int base, int exp) {
        last_error = TMPL_OK;
        return static_cast<int>(std::pow(base, exp));
    }

    tmpl_result_t Div(const Tmpl_DivInputs& inputs, float& out_result) {
        if (inputs.denominator == 0.0f) {
            last_error = TMPL_ERR_DIV_ZERO;
            return TMPL_ERR_DIV_ZERO;
        }
        out_result = inputs.numerator / inputs.denominator;
        last_error = TMPL_OK;
        return TMPL_OK;
    }

    const char* GetLastError() const {
        switch (last_error) {
            case TMPL_OK: return "OK";
            case TMPL_ERR_NULL_HANDLE: return "Null pointer error";
            case TMPL_ERR_DIV_ZERO: return "Division by zero";
            case TMPL_ERR_INTERNAL: return "Internal error";
            default: return "Unknown error";
        }
    }
};

// C API Implementation

TMPL_API Tmpl_Handle tmpl_Create(const char* /*ini_filename*/) {
    Tmpl_Handle handle = {};
    handle.impl = static_cast<void*>(new Tmpl_Impl());
    return handle;
}

TMPL_API void tmpl_Destroy(Tmpl_Handle* handle) {
    if (!handle || !handle->impl) {
        return;
    }
    delete static_cast<Tmpl_Impl*>(handle->impl);
    handle->impl = nullptr;
}

TMPL_API int tmpl_Add(Tmpl_Handle handle, int a, int b) {
    if (!handle.impl) return 0;
    return static_cast<Tmpl_Impl*>(handle.impl)->Add(a, b);
}

TMPL_API int tmpl_Mul(Tmpl_Handle handle, int a, int b) {
    if (!handle.impl) return 0;
    return static_cast<Tmpl_Impl*>(handle.impl)->Mul(a, b);
}

TMPL_API int tmpl_Pow(Tmpl_Handle handle, int base, int exp) {
    if (!handle.impl) return 0;
    return static_cast<Tmpl_Impl*>(handle.impl)->Pow(base, exp);
}

TMPL_API tmpl_result_t tmpl_Div(Tmpl_Handle handle, const Tmpl_DivInputs* inputs, float* out_result) {
    if (!handle.impl || !inputs || !out_result) {
        if (handle.impl) {
            static_cast<Tmpl_Impl*>(handle.impl)->last_error = TMPL_ERR_NULL_HANDLE;
        }
        return TMPL_ERR_NULL_HANDLE;
    }

    const Tmpl_DivInputs& in = *inputs;
    float& res = *out_result;
    return static_cast<Tmpl_Impl*>(handle.impl)->Div(in, res);
}

TMPL_API const char* tmpl_GetLastError(Tmpl_Handle handle) {
    if (!handle.impl) return "Invalid handle";
    return static_cast<Tmpl_Impl*>(handle.impl)->GetLastError();
}

TMPL_API void tmpl_GetVersion(int* major, int* minor, int* patch) {
    if (major) *major = TMPL_MAJOR_VERSION;
    if (minor) *minor = TMPL_MINOR_VERSION;
    if (patch) *patch = TMPL_PATCH_VERSION;
}