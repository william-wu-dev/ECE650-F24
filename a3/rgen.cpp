#include <fstream>
#include <iostream>
#include <unistd.h>
#include "GeneralException.h"
#include "Street.h"
#include "Point.h"
#include <vector>
#include <limits.h>

#define DEBUG true
#define ERROR_MESSAGE false
#define SLEEP false

constexpr int ATTEMPT = 25;

bool is_positive_integer(const std::string &s) {
    for (const auto ch: s) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

/**
 * generate random integer in range [min, max]
 * @param min floor of random integer, inclusive
 * @param max ceiling of random integer, inclusive
 * @return random integer range in [min, max]
 */
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

/// main() must be declared with arguments
/// otherwise command line arguments are ignored
int main(int argc, char **argv) {
    // don't really need these flags because all the flags have value, we use value instead.
    // bool s_flag = false;
    // bool n_flag = false;
    // bool l_flag = false;
    // bool c_flag = false;

    int s_value = 10; // # of streets \in [2, k], k >= 2, default k = 10
    int n_value = 5; // # of line_seg in each street \in [1, k], k >= 1, default k = 5
    // note that # of seg-point will be n_value + 1
    int l_value = 5; // waiting time in seconds \in [5, k], k >= 5, default k = 5
    int c_value = 20; // range of coordinates \in [-k, k], coordinates are int, k >= 1, default k = 20

    int opt_read;

    opterr = 0;
    // If getopt() does not recognize an option character, it prints an error message to stderr, stores the character in
    // optopt, and returns '?'. The calling program may prevent the error message by setting opterr to 0.

    // Generally, the getopt() function is called from inside of a loop’s conditional statement. The loop terminates
    // when the getopt() function returns -1. A switch statement is then executed with the value returned by getopt()
    // function.
    // expected options are '-s value', '-n value', '-l value' and '-c value'
    try {
        while ((opt_read = getopt(argc, argv, ":s:n:l:c:")) != -1) {
            // ‘-1’ if there are no more options to process.
            switch (opt_read) {
                case 's': {
                    if (!is_positive_integer(optarg) || atoi(optarg) < 2 || atoi(optarg) > INT_MAX) {
                        std::string msg = "invalid argument for -s received: ";
                        msg += optarg;
                        throw a3::GeneralException(msg);
                    }
                    s_value = atoi(optarg);
                    break;
                }
                case 'n': {
                    if (!is_positive_integer(optarg) || atoi(optarg) < 1 || atoi(optarg) > INT_MAX) {
                        std::string msg = "invalid argument for -n received: ";
                        msg += optarg;
                        throw a3::GeneralException(msg);
                    }
                    n_value = atoi(optarg);
                    break;
                }
                case 'l': {
                    if (!is_positive_integer(optarg) || atoi(optarg) < 5 || atoi(optarg) > INT_MAX) {
                        std::string msg = "invalid argument for -l received: ";
                        msg += optarg;
                        throw a3::GeneralException(msg);
                    }
                    l_value = atoi(optarg);
                    break;
                }
                case 'c': {
                    if (!is_positive_integer(optarg) || atoi(optarg) < 1 || atoi(optarg) > INT_MAX) {
                        std::string msg = "invalid argument for -c received: ";
                        msg += optarg;
                        throw a3::GeneralException(msg);
                    }
                    c_value = atoi(optarg);
                    break;
                }
                case ':': {
                    std::string opt_without_arg;
                    opt_without_arg += static_cast<char>(optopt);
                    std::string msg = "option -";
                    msg += opt_without_arg;
                    msg += " requires an argument";
                    throw a3::GeneralException(msg);
                    break;
                }
                case '?': {
                    std::string opt_unknown;
                    opt_unknown += static_cast<char>(optopt);
                    std::string msg = "unknown option -";
                    msg += opt_unknown;
                    throw a3::GeneralException(msg);
                    break;
                }
                default: {
                    throw a3::GeneralException("unknown error");
                    break;
                };
            }
        }
        // opt that are not parsed
        if (optind < argc) {
            std::string msg = "non-optional arguments: ";
            for (auto i = optind; i < argc; i++) {
                msg += argv[i];
                msg += " ";
            }
            throw a3::GeneralException(msg);
        }
#if DEBUG
        std::cerr << "s_value = " << s_value << std::endl;
        std::cerr << "n_value = " << n_value << std::endl;
        std::cerr << "l_value = " << l_value << std::endl;
        std::cerr << "c_value = " << c_value << std::endl;
#endif
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        while (true) {
            // bookkeeping all streets
            std::vector<a3::Street> streets;

            // generate segments and command to std out
            int streets_size = randint(2, s_value);
            for (int i = 0; i < streets_size; i++) {
                // generate street
                a3::Street street;
                int segment_points_size = randint(1, n_value) + 1;
                for (int j = 0; j < segment_points_size; j++) {
                    bool flag = false; // flag for successfully generation
                    int fail_count = 0; // count failure attempts
                    while (!flag) {
                        // generate segment point, note x, y in [-k, k]
                        int x = randint(-c_value, c_value);
                        int y = randint(-c_value, c_value);
                        a3::Point generated_segment_point(x, y);
                        // test this point validity
                        try {
                            // if this street has a line segment, i.e., this is not the first add
                            if (street.get_segment_points_size() > 0) {
                                const auto &last_segment_point = street.get_last_point();
                                // check every other street
                                for (const auto &other_street: streets) {
                                    other_street.assert_no_overlap(last_segment_point, generated_segment_point);
                                }
                            }

                            // try to add, i.e., check with itself
                            street.add_segment_point(generated_segment_point);

                            // if no problem at all, this generation is successful
                            flag = true;
                        } catch (std::exception &e) {
                            fail_count += 1;
#if DEBUG && ERROR_MESSAGE
                            std::cerr << "Error: " << "failed attempt #" << fail_count << " for " << street.
                                    get_street_name() << ": " << e.what() << std::endl;
#endif
                            if (fail_count >= ATTEMPT) {
                                std::string message = "failed to generate valid input for ";
                                message += std::to_string(ATTEMPT);
                                message += " simultaneous attempts";
                                throw a3::GeneralException(message);
                            }
                        }
                    }
                }
                streets.push_back(street);
#if DEBUG
                std::cerr << "RGEN: " << street.issue_add_street() << std::endl;
#endif
            }

            // issue add
            for (const auto &street: streets) {
                std::cout << street.issue_add_street() << std::endl;
#if SLEEP
                sleep(1);
#endif
            }

            // issue gg command
            std::cout << "gg" << std::endl;

            // sleep random seconds
            int sleep_time = randint(5, l_value);
            sleep(sleep_time);

            // issue rm
            for (const auto &street: streets) {
                std::cout << street.issue_remove_street() << std::endl;
#if SLEEP
                sleep(1);
#endif
            }
        }
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}
