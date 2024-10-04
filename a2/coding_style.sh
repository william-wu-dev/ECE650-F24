#! /usr/bin/bash
PATH_TO_CMAKE_SOURCE="./"
PATH_TO_CMAKE_BUILD="./cmake-build-clang-tidy"

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
python3 run-clang-tidy.py -p cmake-build-clang-tidy *.cpp
# python3 run-clang-tidy.py -p cmake-build-clang-tidy Graph.cpp
# python3 run-clang-tidy.py -p cmake-build-clang-tidy test_graph.cpp
