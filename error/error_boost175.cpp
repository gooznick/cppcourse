#include <iostream>
#include <boost/outcome.hpp>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;

// Function that may fail
outcome::result<int, std::string> getValue(bool fail) {
    if (fail) return outcome::failure(std::string("Failed to get value"));
    return 42;
}

// Function that processes only successful results
outcome::result<int, std::string> multiplyByTwo(int x) {
    return x * 2;
}

int main() {
    auto result = getValue(false)
        .and_then(multiplyByTwo);  // Only calls if success

    if (result) {
        std::cout << "Success: " << result.value() << "\n";
    } else {
        std::cerr << "Error: " << result.error() << "\n";
    }

    return 0;
}
