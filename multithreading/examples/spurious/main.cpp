
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

std::vector<int> stack;
std::mutex mtx;
std::condition_variable cv;

void consumer(int id) {
    std::unique_lock<std::mutex> lk(mtx);
    
    // ❌ BUG: Using 'if' instead of 'while'
    // If we wake up, we assume data exists.
    if (stack.empty()) {
        cv.wait(lk); 
    }

    // CRASH HERE for the 2nd thread! 💥
    // The stack is empty (stolen by the 1st thread).
    int val = stack.back(); 
    stack.pop_back();
}

int main() {
    // 1. Add ONE item 🍎
    stack.push_back(1); 

    // 2. Wake BOTH threads (Simulating Spurious Wakeup for one) 🔔
    std::thread t1(consumer, 1);
    std::thread t2(consumer, 2);
    
    cv.notify_all(); // Wake everyone!

    t1.join(); t2.join();
}