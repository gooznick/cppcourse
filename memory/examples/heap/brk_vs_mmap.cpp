// malloc_brk_mmap_demo.cpp
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <cstring>

static void print_brk(const char* tag) {
    void* brk_now = sbrk(0);
    std::printf("%s: sbrk(0) = %p\n", tag, brk_now);
}

static void show_mapping_for_ptr(void* p) {
    std::FILE* f = std::fopen("/proc/self/maps", "r");
    if (!f) { std::perror("fopen"); return; }

    std::uintptr_t addr = (std::uintptr_t)p;
    char line[512];

    while (std::fgets(line, sizeof(line), f)) {
        unsigned long start = 0, end = 0;
        if (std::sscanf(line, "%lx-%lx", &start, &end) == 2) {
            if (addr >= start && addr < end) {
                std::printf("ptr %p is in mapping: %s", p, line);
                break;
            }
        }
    }
    std::fclose(f);
}

int main() {
    print_brk("start");

    // Small allocation: often from heap/brk-managed area (may or may not trigger brk immediately)
    std::size_t small_sz = 1024; // 1 KiB
    void* small = std::malloc(small_sz);
    std::printf("small malloc(%zu) = %p\n", small_sz, small);
    print_brk("after small");
    show_mapping_for_ptr(small);

    std::puts("");

    // Large allocation: typically via mmap in glibc (default threshold often ~128 KiB, dynamic)
    std::size_t big_sz = 4 * 1024 * 1024; // 4 MiB
    void* big = std::malloc(big_sz);
    std::printf("big   malloc(%zu) = %p\n", big_sz, big);
    print_brk("after big");
    show_mapping_for_ptr(big);

    // Touch memory so mappings are actually used
    std::memset(small, 0x11, small_sz);
    std::memset(big,   0x22, 4096); // touch first page

    std::free(big);
    std::free(small);
    return 0;
}
