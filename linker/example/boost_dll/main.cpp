#include <boost/dll/import.hpp>
#include <boost/function.hpp>
#include <iostream>


int main() {
    try {

        // Import function from the shared library
        auto giveMe5 = boost::dll::import<int()>(
            "libgm5.so",     // path to the library
            "GiveMe5",       // name of the function
            boost::dll::load_mode::default_mode
        );

        std::cout << "GiveMe5() returns: " << giveMe5() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading symbol: " << e.what() << std::endl;
    }

    return 0;
}
