#include <iostream>
#include <csignal>
#include <csetjmp>
#include <cstdlib>

std::jmp_buf jump_buffer;

void signal_handler(int signal) {
    if (signal == SIGFPE) {
        std::cout << "Caught SIGFPE (Division by Zero)!" << std::endl;
        // We cannot just return, because the CPU will retry the div instruction
        // and loop forever. We must jump out.
        std::longjmp(jump_buffer, 1);
    }
}

int main() {
    // Register the signal handler
    std::signal(SIGFPE, signal_handler);

    int a = 100;
    int b = 0;
    int c = 0;

    std::cout << "Attempting division..." << std::endl;

    if (setjmp(jump_buffer) == 0) {
        // This block is executed first
        c = a / b; // This causes SIGFPE
        std::cout << "Result: " << c << std::endl; // Never reached
    } else {
        // This block is executed after longjmp
        std::cout << "Recovered from SIGFPE!" << std::endl;
        c = 0; // Fallback value
    }

    std::cout << "Program continues... c = " << c << std::endl;

    return 0;
}
