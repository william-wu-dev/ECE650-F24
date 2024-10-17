#include <fstream>
#include "../GeneralException.h"

/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int randint(const int min, const int max) noexcept(false) {
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        throw a3::GeneralException("unable to open /dev/urandom");
    }

    // start reading, unsigned int reading only
    unsigned int x = 0;
    urandom.read(reinterpret_cast<char *>(&x), sizeof(unsigned int));

    // calculated in unsigned then transferred to int. after the modulo, it is guaranteed that the result is in the int
    // range. Unless, b - a + 1 exceed in range. 2147483647
    const int mid = static_cast<int>(x % static_cast<unsigned int>(max - min + 1));

    const int res = mid + min;

    // close random stream
    urandom.close();

    return res;
}

const int M = 1e5 + 10;
int book[M];

TEST_CASE("randint generation in range") {
    const int a = -20, b = 20;
    const int N = 100;
    for (int i = 0; i < N; i++) {
        CHECK(randint(a, b) >= a);
        CHECK(randint(a, b) <= b);
    }
}

TEST_CASE("randint generation include all number") {
    const int a = 2, b = 10;
    const int N = 100;
    for (int i = 0; i < N; i++) {
        book[randint(a, b)] += 1;
    }

    for(int i = a; i <= b; i++) {
        CHECK(book[i] > 0);
    }
}
