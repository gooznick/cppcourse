#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  // Standard integer types


// - Define export macros based on platform/compiler
#if defined(_WIN32) || defined(_WIN64)
    #ifdef TPL_BUILD
        #define TPL_API __declspec(dllexport)
    #else
        #define TPL_API __declspec(dllimport)
    #endif
#else
    #ifdef TPL_BUILD
        #define TPL_API __attribute__((visibility("default")))
    #else
        #define TPL_API
    #endif
#endif

// - Ensure 1-byte packing for structures
#pragma pack(push, 1)

// - Define a handle type for managing instances
typedef struct tpl_handle_t tpl_handle_t;

// - Define input data structure (aligned to 1 byte)
struct tpl_data_t{
    uint8_t field1;
    uint16_t field2;
    uint32_t field3;
};

// - Define the status structure returned by `tpl_Process`
struct tpl_status_t {
    int32_t error_code;
    char message[64];
};

#pragma pack(pop)

// - API FUNCTIONS (only these will be exported)

// Creates a handle (returns NULL on failure)
TPL_API tpl_handle_t* tpl_Create(void);

// Destroys a handle (safe to call with NULL)
TPL_API void tpl_Destroy(tpl_handle_t* handle);

// Processes data and returns a status
TPL_API tpl_status_t tpl_Process(tpl_handle_t* handle, const tpl_data_t* data);

#ifdef __cplusplus
}
#endif
