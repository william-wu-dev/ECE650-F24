cmake_minimum_required(VERSION 2.8...3.13)
project(ece650-a3)

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/run)

option(WITH_ASAN "Enabled AddressSanitizer to detect memory faults" OFF)
if(WITH_ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined" )
endif()


# create the main executable
## add additional .cpp files if needed
add_subdirectory(a2)  # contain ece650-a2
add_executable(ece650-a3 ece650-a3.cpp )
add_executable(rgen rgen.cpp GeneralException.h
        Point.cpp
        Point.h
        Street.cpp
        Street.h)

# create the executable for tests
add_executable(tests test/test.cpp)
add_executable(test_randint test/test_randint.cpp)
add_executable(test_point test/test_point.cpp
        Point.cpp
        Point.h)
add_executable(test_street test/test_street.cpp
        Point.cpp
        Point.h
        Street.cpp
        Street.h
        GeneralException.h)
install(PROGRAMS a1/a1ece650.py a1/line_parser.py a1/StreetDatabase.py a1/graph_generator.py a1/Graph.py DESTINATION bin)
install(TARGETS rgen ece650-a2 ece650-a3 RUNTIME DESTINATION bin)
