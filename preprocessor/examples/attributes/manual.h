#include <string>

class Person {
private:
    std::string name;
    int age;
    double height;
    
public:
    std::string get_name() const { return name; }
    int get_age() const { return age; }
    double get_height() const { return height; }
    
    void set_name(std::string value) { name = value; }
    void set_age(int value) { age = value; }
    void set_height(double value) { height = value; }
};
