#include <string>


class Point {
public:
    int x, y;
    std::string name;
    
    Point(int x, int y, std::string name) : x(x), y(y), name(std::move(name)) {}
};

int main()
{
    Point p{1,2, "mypoint"};
    return 0;

}