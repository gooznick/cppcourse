#include <iostream>
#include <boost/preprocessor.hpp>

// Define the enum entries
#define COLOR_ENUM_SEQ (RED)(GREEN)(BLUE)

// Generate the enum definition
#define GENERATE_ENUM(r, data, elem) elem,

enum Color {
    BOOST_PP_SEQ_FOR_EACH(GENERATE_ENUM, _, COLOR_ENUM_SEQ)
};

// Generate the to_string function
#define GENERATE_STRING_CASE(r, data, elem) case elem: return BOOST_PP_STRINGIZE(elem);

const char* to_string(Color color) {
    switch (color) {
        BOOST_PP_SEQ_FOR_EACH(GENERATE_STRING_CASE, _, COLOR_ENUM_SEQ)
        default: return "Unknown";
    }
}

int main() {
    Color color = RED;
    std::cout << "Color: " << to_string(color) << std::endl;

    color = BLUE;
    std::cout << "Color: " << to_string(color) << std::endl;

    return 0;
}
