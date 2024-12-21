#include <boost/pfr.hpp>
#include <iostream>

struct Person {
    std::string name;
    int age;
};

int main() {
    Person p{"Alice", 25};
    boost::pfr::for_each_field(p, [](const auto& field) {
        std::cout << field << '\n';
    });
}