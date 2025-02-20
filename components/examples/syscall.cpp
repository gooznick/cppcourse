#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/sched.h>  // Contains clone_args struct
int main() {
    std::cout << "Running on Ubuntu 22.04" << std::endl;

    // Call clone3() syscall (exists in Linux 5.5+, fails on older systems)
    struct clone_args args = {};
    long ret = syscall(SYS_clone3, &args, sizeof(args));

    if (ret == -1) {
        perror("clone3 failed");
        return 1; // Fail on Ubuntu 16.04
    }

    return 0; // Runs successfully on Ubuntu 22.04
}


// /opt/components/cross/syscall 
// Running on Ubuntu 22.04
// clone3 failed: Function not implemented

// g++ -static syscall.cpp -osyscall
//  ./syscall 
// Running on Ubuntu 22.04
//  ldd syscall
//         not a dynamic executable
