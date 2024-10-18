#include "../Point.h"

/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("create and print point") {
    const int x = 11, y = -12;
    a3::Point p(x, y);
    CHECK(p.to_string() == std::string("(11, -12)"));
}

TEST_CASE("point equality operation") {
    a3::Point p(1, 2);
    a3::Point q(1, 2);
    a3::Point r(1, 3);
    CHECK(p == q);
    CHECK(p != r);
}


