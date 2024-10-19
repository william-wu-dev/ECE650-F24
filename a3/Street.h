#ifndef STREET_H
#define STREET_H
#include <string>
#include <utility>
#include <vector>

#include "Point.h"

namespace a3 {

class Street {
private:
    std::string street_name;
    std::vector<Point> segment_points;
public:
    static int cnt;  // unique street id

    Street();

    void add_segment_point(const Point &point);

    std::string issue_add_street() const;

    std::string issue_remove_street() const;

    std::string get_street_name() const;

    Point get_last_point() const;

    void assert_no_overlap(const Point &start, const Point &end) const;

    size_t get_segment_points_size() const;
};

} // a3

#endif //STREET_H
