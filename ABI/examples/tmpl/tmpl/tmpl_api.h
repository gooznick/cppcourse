#pragma once

// Visibility macros
#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef BUILDING_TMPL_DLL
    #define TMPL_API __declspec(dllexport)
  #else
    #define TMPL_API __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_TMPL_DLL
    #define TMPL_API __attribute__((visibility("default")))
  #else
    #define TMPL_API
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Error codes
typedef enum {
    TMPL_OK = 0,
    TMPL_ERR_NULL_HANDLE = 1,
    TMPL_ERR_DIV_ZERO = 2,
    TMPL_ERR_INTERNAL = 3,
} tmpl_result_t;

// Opaque handle
struct Tmpl_Handle
{
    void* impl; // Pointer to internal implementation
};

// Packed struct for division input
#pragma pack(push, 1)
typedef struct {
    float numerator;
    float denominator;
} Tmpl_DivInputs;
#pragma pack(pop)

// API functions
TMPL_API Tmpl_Handle tmpl_Create(const char* ini_filename);
TMPL_API void tmpl_Destroy(Tmpl_Handle* handle);
TMPL_API void tmpl_GetVersion(int* major, int* minor, int* patch);

TMPL_API int tmpl_Add(Tmpl_Handle handle, int a, int b);
TMPL_API int tmpl_Mul(Tmpl_Handle handle, int a, int b);
TMPL_API int tmpl_Pow(Tmpl_Handle handle, int base, int exp);
TMPL_API tmpl_result_t tmpl_Div(Tmpl_Handle handle, const Tmpl_DivInputs* inputs, float* out_result);

TMPL_API const char* tmpl_GetLastError(Tmpl_Handle handle);

#ifdef __cplusplus
}
#endif
