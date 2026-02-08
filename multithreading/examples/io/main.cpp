
#include <iostream>
#include <thread>

void speak(int id) {

    std::cout << "Thread " << id << " says: " << "Hello!\n";
}

int main() {
    std::thread t1(speak, 1);
    std::thread t2(speak, 2);
    t1.join(); t2.join();
}