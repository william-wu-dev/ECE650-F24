cmake_minimum_required(VERSION 2.8)
project(ece650-threads)

find_package (Threads)
## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

# expose minisat header files
include_directories(${CMAKE_SOURCE_DIR}/include)

if (UNIX)
  # only available on unix-like system
  add_subdirectory(helper)
  add_subdirectory(prod-cons)
  add_subdirectory(lk)
  add_subdirectory(tinout)
  add_subdirectory(simple)
  add_subdirectory(webapp)
  add_subdirectory(checksum)
  add_subdirectory(cpp-live)
  add_subdirectory(simple-live)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  # only works on linux
  add_subdirectory(tim)
  add_subdirectory(kill-ex)
endif()
