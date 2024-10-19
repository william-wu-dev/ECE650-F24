#include "Street.h"
#include "GeneralException.h"

namespace a3 {
    /**
     * This function get the orientation of ordered points a(x_1, y_1), b(x_2, y_2), and c(x_3, y_3).
     * The way to get orientation is to compute the slop of line segment <a, b> and line segment <b, c>
     * Simplify the slop equation and avoiding zero division problem, the equation to compare is:
     * (y_2 - y_1) * (x_3 - x_2) - (y_3 - y_2) * (x_2 - x_1) <, =, or > 0 ?
     * @param a Point 1
     * @param b Point 2
     * @param c Point 3
     * @return =: encoded as 0, collinear(on the same line)
              >: encoded as 1, clock wise
              <: encoded as 2, counter clock wise
     */
    int get_orientation(const Point &a, const Point &b, const Point &c) {
        const auto res = (b.get_y() - a.get_y()) * (c.get_x() - b.get_x()) - (c.get_y() - b.get_y()) * (
                             b.get_x() - a.get_x());

        if (res == 0)
            return 0;
        else if (res > 0)
            return 1;
        else
            return 2; // res < 0
    }

    /**
     * This is the function that checks whether two line segments <p1, q1> and <p2, q2> intersect or overlap
     * @param p1 start point of line one
     * @param q1 end point of line one
     * @param p2 start point of line two
     * @param q2 end point of line two
     * @return
     * -1: not intersect.
     * 0: collinear but 2 intersection, i.e., intersect by overlap.
     * 1: collinear but only 1 intersection
     * 2: intersect regularly, use the intersect function to find intersection.
     * @note DO NOT CHECK THE LAST LINE SEGMENT AND THE NEW SEGMENT, THEY ARE BOUND TO INTERSECT
     */
    int check_intersection(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
        const auto o_p1_q1_p2 = get_orientation(p1, q1, p2);
        const auto o_p1_q1_q2 = get_orientation(p1, q1, q2);
        const auto o_p2_q2_p1 = get_orientation(p2, q2, p1);
        const auto o_p2_q2_q1 = get_orientation(p2, q2, q1);

        if (o_p1_q1_p2 != o_p1_q1_q2 && o_p2_q2_p1 != o_p2_q2_q1) // intersect regularly (not overlap)
            return 2;

        if (o_p1_q1_p2 == 0 && o_p1_q1_q2 == 0 && o_p2_q2_p1 == 0 && o_p2_q2_q1 == 0) {
            // on the same line, need more comparison to determine intersect
            // if x is all equal, compare y
            if (p1.get_x() == q1.get_x() && q1.get_x() == p2.get_x() && p2.get_x() == q2.get_x()) {
                if (std::min(p1.get_y(), q1.get_y()) > std::max(p2.get_y(), q2.get_y()) ||
                    std::min(p2.get_y(), q2.get_y()) > std::max(p1.get_y(), q1.get_y())) {
                    return -1;
                }
                if (std::min(p1.get_y(), q1.get_y()) == std::max(p2.get_y(), q2.get_y()) ||
                    std::min(p2.get_y(), q2.get_y()) == std::max(p1.get_y(), q1.get_y())) {
                    return 1;
                }
                return 0;
            }
            // compare the projection of x, only if x is not all equal
            if (std::min(p1.get_x(), q1.get_x()) > std::max(p2.get_x(), q2.get_x()) ||
                std::min(p2.get_x(), q2.get_x()) > std::max(p1.get_x(), q1.get_x())) {
                return -1;
            }
            if (std::min(p1.get_x(), q1.get_x()) == std::max(p2.get_x(), q2.get_x()) ||
                std::min(p2.get_x(), q2.get_x()) == std::max(p1.get_x(), q1.get_x())) {
                return 1;
                }
            // if one segment smaller x is larger than the other segment larger x, then no intersection
            return 0;
        }

        return -1; // all other situations are not intersect
    }

    int Street::cnt = 1;

    Street::Street() {
        this->street_name = "street " + std::to_string(cnt);
        cnt += 1;
    }

    void Street::add_segment_point(const Point &point) noexcept(false) {
        // check validity of this point

        // if add process result in zero-length line segment, raise exception
        // only check when there is more than 0 segment point
        if (!this->segment_points.empty()) {
            auto &previous_point = this->segment_points.back();
            if (point == previous_point) {
                std::string message = "generate a zero-length line segment: ";
                message += previous_point.to_string();
                message += " ";
                message += point.to_string();
                throw GeneralException(message);
            }
        }

        // if add process result in the new line segment intersect or overlap with
        // previous line segments in the same street, raise exception.
        // only check when there are more than 1 segment point, i.e., more than 1 line segment
        if (this->segment_points.size() > 1) {
            // check any other line segment except for the last line segment
            for (auto i = 0; i < this->segment_points.size() - 2; i++) {
                const auto &p1 = this->segment_points[i];
                const auto &q1 = this->segment_points[i + 1];
                const auto &p2 = this->segment_points.back();
                const auto &q2 = point;

                auto res = check_intersection(p1, q1, p2, q2);
                if (res == 0) {
                    std::string message = "generated line segment ";
                    message += p2.to_string();
                    message += " ";
                    message += q2.to_string();
                    message += " collinear-2 intersects with line segment ";
                    message += p1.to_string();
                    message += " ";
                    message += q1.to_string();
                    throw GeneralException(message);
                }
                if (res == 1) {
                    std::string message = "generated line segment ";
                    message += p2.to_string();
                    message += " ";
                    message += q2.to_string();
                    message += " collinear-1 intersects with line segment ";
                    message += p1.to_string();
                    message += " ";
                    message += q1.to_string();
                    throw GeneralException(message);
                }
                if (res == 2) {
                    std::string message = "generated line segment ";
                    message += p2.to_string();
                    message += " ";
                    message += q2.to_string();
                    message += " regularly intersects with line segment ";
                    message += p1.to_string();
                    message += " ";
                    message += q1.to_string();
                    throw GeneralException(message);
                }
            }

            // check the last line segment
            const auto &a = this->segment_points[this->segment_points.size() - 2];
            const auto &b = this->segment_points[this->segment_points.size() - 1];
            const auto &c = point;
            if (get_orientation(a, b, c) == 0) {
                if (a.get_x() == b.get_x() && b.get_x() == c.get_x()) {
                    if ((b.get_y() > a.get_y() && c.get_y() < b.get_y()) ||
                        (b.get_y() < a.get_y() && c.get_y() > b.get_y())) {
                        std::string message = "generated line segment ";
                        message += b.to_string();
                        message += " ";
                        message += c.to_string();
                        message += " overlaps with line segment ";
                        message += a.to_string();
                        message += " ";
                        message += b.to_string();
                        throw GeneralException(message);
                    }
                } else {
                    if ((b.get_x() > a.get_x() && c.get_x() < b.get_x()) ||
                        (b.get_x() < a.get_x() && c.get_x() > b.get_x())) {
                        std::string message = "generated line segment ";
                        message += b.to_string();
                        message += " ";
                        message += c.to_string();
                        message += " overlaps with line segment ";
                        message += a.to_string();
                        message += " ";
                        message += b.to_string();
                        throw GeneralException(message);
                    }
                }
            }
        }

        // checked OK
        this->segment_points.push_back(point);
    }

    std::string Street::issue_add_street() const {
        std::string res = "add \"";
        res += this->street_name;
        res += "\"";
        for (auto point: this->segment_points) {
            res += " ";
            res += point.to_string();
        }
        return res;
    }

    std::string Street::issue_remove_street() const {
        std::string res = "rm \"";
        res += this->street_name;
        res += "\"";
        return res;
    }

    std::string Street::get_street_name() const {
        return this->street_name;
    }

    Point Street::get_last_point() const {
        return this->segment_points.back();
    }

    void Street::assert_no_overlap(const Point &start, const Point &end) const noexcept(false){
        const auto &p2 = start;
        const auto &q2 = end;
        for (auto i = 0; i < this->segment_points.size() - 1; i++) {
            const auto &p1 = this->segment_points[i];
            const auto &q1 = this->segment_points[i + 1];

            auto res = check_intersection(p1, q1, p2, q2);
            if (res == 0) {
                std::string message = "generated line segment ";
                message += p2.to_string();
                message += " ";
                message += q2.to_string();
                message += " collinear-2 intersects with line segment ";
                message += p1.to_string();
                message += " ";
                message += q1.to_string();
                message += " from ";
                message += this->get_street_name();
                throw GeneralException(message);
            }
        }
    }

    size_t Street::get_segment_points_size() const {
        return this->segment_points.size();
    }
} // a3
