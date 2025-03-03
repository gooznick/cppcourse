#include <boost/preprocessor.hpp>
#include <iostream>

// Define attributes as a valid sequence of tuples
#define ATTRIBUTES ((name, std::string)) ((age, int)) ((height, double))

// Macro to generate class members
#define GENERATE_MEMBERS(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(2, 1, elem) BOOST_PP_TUPLE_ELEM(2, 0, elem);

// Macro to generate getters
#define GENERATE_GETTERS(r, data, elem) \
    BOOST_PP_TUPLE_ELEM(2, 1, elem) BOOST_PP_CAT(get_, BOOST_PP_TUPLE_ELEM(2, 0, elem)) () const { \
        return BOOST_PP_TUPLE_ELEM(2, 0, elem); \
    }

// Macro to generate setters
#define GENERATE_SETTERS(r, data, elem) \
    void BOOST_PP_CAT(set_, BOOST_PP_TUPLE_ELEM(2, 0, elem)) (BOOST_PP_TUPLE_ELEM(2, 1, elem) value) { \
        BOOST_PP_TUPLE_ELEM(2, 0, elem) = value; \
    }

// Define a class using the macros
class Person {
private:
    BOOST_PP_SEQ_FOR_EACH(GENERATE_MEMBERS, _, ATTRIBUTES)

public:
    BOOST_PP_SEQ_FOR_EACH(GENERATE_GETTERS, _, ATTRIBUTES)
    BOOST_PP_SEQ_FOR_EACH(GENERATE_SETTERS, _, ATTRIBUTES)
};

int main() {
    Person p;
    p.set_name("John");
    p.set_age(30);
    std::cout << "Name: " << p.get_name() << ", Age: " << p.get_age() << std::endl;
}
