#! /usr/bin/bash

CLEAR_BUILD_FOLDER=true

PATH_TO_CMAKE_SOURCE="./"
PATH_TO_CMAKE_BUILD="./cmake-build-clang-tidy"

# clear cmake build path
if ${CLEAR_BUILD_FOLDER}; then
    echo "starting to clear cmake build file"
    printf "\n"
    rm -rf ${PATH_TO_CMAKE_BUILD}/*
fi

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}

printf "\n"
printf "starting clang-tidy\n"
clang-tidy -p ${PATH_TO_CMAKE_BUILD} *.cpp
# python3 run-clang-tidy.py -p cmake-build-clang-tidy Graph.cpp
# python3 run-clang-tidy.py -p cmake-build-clang-tidy test_graph.cpp
