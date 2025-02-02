#include <iostream>
#include <boost/outcome.hpp>
#include <boost/system/error_code.hpp>

namespace outcome = BOOST_OUTCOME_V2_NAMESPACE;
namespace sys = boost::system;

// Define custom error types
enum class MyError {
    FileNotFound = 1,
    InvalidFormat,
    ComputationFailed
};

// Custom error category
class MyErrorCategory : public sys::error_category {
public:
    const char* name() const noexcept override {
        return "MyErrorCategory";
    }

    std::string message(int ev) const override {
        switch (static_cast<MyError>(ev)) {
            case MyError::FileNotFound: return "File not found";
            case MyError::InvalidFormat: return "Invalid file format";
            case MyError::ComputationFailed: return "Computation failed";
            default: return "Unknown error";
        }
    }
};

// Singleton instance of the category
const MyErrorCategory& myErrorCategory() {
    static MyErrorCategory instance;
    return instance;
}

// Convert MyError to boost::system::error_code
sys::error_code make_error_code(MyError e) {
    return {static_cast<int>(e), myErrorCategory()};
}

// Function returning boost::outcome::result
outcome::result<int> loadFile(bool fail) {
    if (fail) return outcome::failure(make_error_code(MyError::FileNotFound));
    return 42; // Success case
}

// Function that processes the result
outcome::result<int> processFile(bool fail) {
    BOOST_OUTCOME_TRY(value, loadFile(fail));
    if (value == 42) return outcome::failure(make_error_code(MyError::InvalidFormat));
    return value * 2; // Success case
}


outcome::result<int> compute(bool fail) {
    if (fail) return outcome::failure(make_error_code(MyError::ComputationFailed));
    return 100;
}

outcome::result<int> fullPipeline(bool fail) {
    BOOST_OUTCOME_TRY(fileData, loadFile(fail));  // Propagate file loading
    BOOST_OUTCOME_TRY(parsedData, processFile(fail));  // Propagate processing
    BOOST_OUTCOME_TRY(computationResult, compute(fail));  // Propagate computation
    return fileData + parsedData + computationResult;
}

int main() {
    auto result = fullPipeline(false);

    if (result) {
        std::cout << "Pipeline success: " << result.value() << "\n";
    } else {
        std::cerr << "Pipeline error: " << result.error().message() << "\n";
    }

    return 0;
}

