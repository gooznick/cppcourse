
#include <thread>
#include <iostream>

int counter = 0; // Shared Resource 

void run() {
    for (int i = 0; i < 100000; ++i) {
        counter++; // The Race is Here 
    }
}

int main() {
    std::thread t1(run);
    std::thread t2(run);
    t1.join(); t2.join();

    std::cout << "Expected: 200000\n";
    std::cout << "Actual:   " << counter << "\n";
}