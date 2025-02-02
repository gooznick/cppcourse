#include <csignal>
#include <iostream>

void signal_handler(int signal) {
    std::cout << "Caught SIGFPE: Division by zero detected!\n";
    std::exit(1);
}

int main() {
    std::signal(SIGFPE, signal_handler);
    
    int a = 10, b = 0;
    int c = a / b; // Triggers SIGFPE
}
