cmake_minimum_required(VERSION 2.8)
project(ece650-minisat)

if (EXISTS "${CMAKE_SOURCE_DIR}/minisat")
  add_subdirectory(minisat)
else()
  message (FATAL_ERROR "MiniSat not found.\n"
		  "Use \n$ cd ${CMAKE_CURRENT_SOURCE_DIR}\n"
		  "$ git clone https://git.uwaterloo.ca/ece650-common/minisat.git\n"
		  "or \n$ git clone ist-git@git.uwaterloo.ca:ece650-common/minisat.git\n"
		  "to clone MiniSat into the source directory.")
endif()

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# create the main executable
# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/minisat)
## add additional .cpp files if needed
add_executable(ece650-minisat ece650-minisat.cpp)
# link minisat library with the main executable
target_link_libraries(ece650-minisat minisat-lib-static)

add_executable(ece650-minisat-with-vector minisat-example-with-vector.cpp)
target_link_libraries(ece650-minisat-with-vector minisat-lib-static)

add_executable(minisat-using-2d-vector minisat-using-2d-vector.cpp)
target_link_libraries(minisat-using-2d-vector minisat-lib-static)

add_executable(minisat-test-solver-result minisat-test-solver-result.cpp)
target_link_libraries(minisat-test-solver-result minisat-lib-static)


## add additional .cpp files if needed
add_executable(ece650-minisat-test-solver-result ece650-minisat-test-solver-result.cpp)
# link minisat library with the main executable
target_link_libraries(ece650-minisat-test-solver-result minisat-lib-static)
