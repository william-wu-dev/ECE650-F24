#include "../Street.h"
#include "../GeneralException.h"

/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("street name construction") {
    for (int i = 1; i <= 10; i++) {
        a3::Street st;
        CHECK(st.get_street_name() == "street " + std::to_string(i));
    }
}

TEST_CASE("street issue remove") {
    a3::Street st;
    CHECK(st.issue_remove_street() == "rm \"" + st.get_street_name() + "\"");
}

TEST_CASE("street issue add") {
    a3::Street st;
    st.add_segment_point(a3::Point(5, 10));
    st.add_segment_point(a3::Point(7, -2));
    CHECK(st.issue_add_street() == "add \"" + st.get_street_name() + "\" (5, 10) (7, -2)");
}

TEST_CASE("add zero length segment 1") {
    a3::Street st;
    st.add_segment_point(a3::Point(5, 5));
    CHECK_THROWS_WITH_AS(st.add_segment_point(a3::Point(5, 5));, "generate a zero-length line segment: (5, 5) (5, 5)",
                         a3::GeneralException);
}

TEST_CASE("add zero length segment 1") {
    a3::Street st;
    st.add_segment_point(a3::Point(7, 5));
    st.add_segment_point(a3::Point(5, 5));
    CHECK_THROWS_WITH_AS(st.add_segment_point(a3::Point(5, 5));, "generate a zero-length line segment: (5, 5) (5, 5)",
                         a3::GeneralException);
}

TEST_CASE("add self intersect segment") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(4, 5));
    st.add_segment_point(a3::Point(5, 3));
    CHECK_THROWS_WITH_AS(st.add_segment_point(a3::Point(2, 5));,
                         "generated line segment (5, 3) (2, 5) regularly intersects with line segment (2, 2) (4, 5)",
                         a3::GeneralException);
}

TEST_CASE("add collinear 1 segment") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(2, 6));
    st.add_segment_point(a3::Point(4, -2));
    st.add_segment_point(a3::Point(2, -2));
    CHECK_THROWS_WITH_AS(st.add_segment_point(a3::Point(2, 2)),
                         "generated line segment (2, -2) (2, 2) collinear-1 intersects with line segment (2, 2) (2, 6)",
                         a3::GeneralException);
}

TEST_CASE("add collinear 2 segment") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(2, 6));
    st.add_segment_point(a3::Point(4, -2));
    st.add_segment_point(a3::Point(2, -2));
    CHECK_THROWS_WITH_AS(st.add_segment_point(a3::Point(2, 7)),
                         "generated line segment (2, -2) (2, 7) collinear-2 intersects with line segment (2, 2) (2, 6)",
                         a3::GeneralException);
}

TEST_CASE("add self overlap segment") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(2, 5));
    CHECK_THROWS_WITH_AS(st.add_segment_point(a3::Point(2, -1)),
                         "generated line segment (2, 5) (2, -1) overlaps with line segment (2, 2) (2, 5)",
                         a3::GeneralException);
}

TEST_CASE("test get last point") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(2, 6));
    st.add_segment_point(a3::Point(4, -2));
    st.add_segment_point(a3::Point(2, -2));
    CHECK(st.get_last_point() == a3::Point(2, -2));
}

TEST_CASE("test assert no overlap") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(2, 6));
    st.add_segment_point(a3::Point(4, -2));
    st.add_segment_point(a3::Point(2, -2));
    std::string message = "generated line segment (0, -2) (6, -2) collinear-2 intersects with line segment (4, -2) (2, -2) from ";
    message += st.get_street_name();
    CHECK_THROWS_WITH_AS(st.assert_no_overlap(a3::Point(0, -2), a3::Point(6, -2)),
                         message.c_str()
                         , a3::GeneralException);
}

TEST_CASE("test assert no overlap 2") {
    a3::Street st;
    st.add_segment_point(a3::Point(2, 2));
    st.add_segment_point(a3::Point(2, 6));
    st.add_segment_point(a3::Point(4, -2));
    st.add_segment_point(a3::Point(2, -2));
    std::string message = "generated line segment (2, 8) (2, 4) collinear-2 intersects with line segment (2, 2) (2, 6) from ";
    message += st.get_street_name();
    CHECK_THROWS_WITH_AS(st.assert_no_overlap(a3::Point(2, 8), a3::Point(2, 4)),
                         message.c_str()
                         , a3::GeneralException);
}
