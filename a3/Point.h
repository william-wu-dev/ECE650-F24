#ifndef POINT_H
#define POINT_H
#include <string>

namespace a3 {

class Point {
private:
    float x;
    float y;
public:
    Point(const float x, const float y)
        : x(x),
          y(y) {
    }
    Point(int _x, int _y);

    float get_x() const;

    void set_x(float x);

    float get_y() const;

    void set_y(float y);

    std::string to_string() const;
};

} // a3

#endif //POINT_H
