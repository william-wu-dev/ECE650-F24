#include "Point.h"

namespace a3 {
    Point::Point(const int _x, const int _y) {
        x = static_cast<float>(_x);
        y = static_cast<float>(_y);
    }

    float Point::get_x() const {
        return x;
    }

    void Point::set_x(const float x) {
        this->x = x;
    }

    float Point::get_y() const {
        return y;
    }

    void Point::set_y(const float y) {
        this->y = y;
    }

    std::string Point::to_string() const {
        std::string res = "(";
        res += std::to_string(static_cast<int>(this->x));
        res += ", ";
        res += std::to_string(static_cast<int>(this->y));
        res += ")";
        return res;
    }

} // a3