### **Features of the `tpl` Shared Library Example**  

#### **📌 General Library Design**
- ✅ **Modern C++ Implementation** with **C API (`extern "C"`)** for compatibility.  
- ✅ **Handle-based API** for clean memory management (`tpl_Create` / `tpl_Destroy`).  
- ✅ **Strict Typing with `stdint.h`** (`uint8_t`, `uint16_t`, `uint32_t`).  
- ✅ **Prefixed Naming (`tpl_`)** to prevent symbol conflicts.  


#### **📌 Memory Management**
- ✅ **Encapsulated `tpl_handle_t` Struct** to prevent direct modification.  
- ✅ **Safe `new` / `delete`** inside `tpl_Create` / `tpl_Destroy`.  


#### **📌 Exception Handling & Error Reporting**
- ✅ **Catches All Exceptions (`std::exception`, `...`)** and prevents crashes.  
- ✅ **Returns Meaningful Error Codes** (`-1` for invalid args, `-2` for std errors, `-99` for unknown).  


#### **📌 Shared Library Best Practices**
- ✅ **Proper Export/Import Handling**:
  - `__declspec(dllexport)` for Windows.  
  - `__attribute__((visibility("default")))` for GCC/Clang.  
- ✅ **Linux `version script` for Symbol Export Control** (`tpl_version.script`).  
- ✅ **Hides Unwanted Symbols** using `CXX_VISIBILITY_PRESET hidden`.  
- ✅ **Sets RPATH to `$ORIGIN` for Local Loading**.  

#### **📌 CMake Best Practices**
- ✅ **Sets `CMAKE_INSTALL_PREFIX` to `build/install`** for local installs.  
- ✅ **Installs Library, Headers, and Tests** in correct locations.  
- ✅ **Uses `PUBLIC_HEADER` to Install API Headers**.  
- ✅ **Supports Versioning (`libtpl.so.1 -> libtpl.so.1.0.0`)**.  
- ✅ **Includes `test_tpl` Executable for Testing API**.  

