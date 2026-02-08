#include <atomic>

struct Heavy { long data[100]; };      // 800 bytes 🐘

std::atomic<Heavy> slow;

int main()
{
    Heavy h1{1};
    slow.store(h1);
    return 0;
}


// __atomic_store(
//     800,
//     &slow,
//     source_buffer,
//     __ATOMIC_SEQ_CST
// );