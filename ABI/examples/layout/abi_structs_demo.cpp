#include <iostream>
#include <cstddef> // for offsetof

#pragma pack(push, 1)
struct MyStruct {
    char a;    // 1 byte
    int  b;    // 4 bytes
    short c;   // 2 bytes
};
#pragma pack(pop)

struct FlagsPadded {
    unsigned int a : 1;
    unsigned int b : 2;
    unsigned int c : 5;
};

#pragma pack(push, 1)
struct FlagsPacked {
    unsigned char a : 1;
    unsigned char b : 2;
    unsigned char c : 5;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Telemetry {
    union {
        int i;
        FlagsPacked f;
    };
};
#pragma pack(pop)

static_assert(sizeof(MyStruct) == 7, "MyStruct size mismatch (packed).");
static_assert(sizeof(FlagsPacked) == 1, "FlagsPacked size mismatch.");
static_assert(sizeof(Telemetry) == 4, "Telemetry size mismatch.");

int main() {
    std::cout << "sizeof(MyStruct) = " << sizeof(MyStruct) << std::endl;
    std::cout << "offsetof(MyStruct, a) = " << offsetof(MyStruct, a) << std::endl;
    std::cout << "offsetof(MyStruct, b) = " << offsetof(MyStruct, b) << std::endl;
    std::cout << "offsetof(MyStruct, c) = " << offsetof(MyStruct, c) << std::endl;
    
    std::cout << "sizeof(FlagsPadded) = " << sizeof(FlagsPadded) << std::endl;
    std::cout << "sizeof(FlagsPacked) = " << sizeof(FlagsPacked) << std::endl;
    std::cout << "sizeof(Telemetry) = " << sizeof(Telemetry) << std::endl;

    Telemetry t;
    t.i = 0;
    t.f.a = 1;
    t.f.b = 2;
    t.f.c = 5;
    std::cout << "Telemetry as int = " << t.i << std::endl;

    return 0;
}
