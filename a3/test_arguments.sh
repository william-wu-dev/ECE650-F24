ARGUMENT_ENABLE=false
CLEAR_BUILD_FOLDER=false
SEPARATE_ERR_AND_OUT=true

# FIXME: change this to fit your project
PATH_TO_CMAKE_SOURCE="./"
PATH_TO_CMAKE_BUILD="./cmake-build-debug"

if [ ! -d "./${PATH_TO_CMAKE_BUILD}" ]; then
    mkdir ./${PATH_TO_CMAKE_BUILD}
elif ${CLEAR_BUILD_FOLDER}; then
    echo "starting to clear cmake build file"
    printf "\n"
    rm -rf ${PATH_TO_CMAKE_BUILD}/*
fi

# build based on mode

if [ $# -ne 2 ]; then
    printf "usage: bash run.sh cmake_mode make_target\n"
    exit 1
fi

if [ "$1" -eq 0 ]; then
    printf "starting cmake \n"
    cmake -DCMAKE_BUILD_TYPE=Debug -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
elif [ "$1" -eq 1 ]; then
    printf "starting cmake \n"
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
elif [ "$1" -eq 2 ]; then
    printf "starting cmake \n"
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DWITH_ASAN=ON -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
else
    printf "unknown argument \"%s\", 0 for plain cmake, 1 for clang++ compile, 2 for address sanitation activation.\n" "$1"
    exit 1
fi

# make in a directory
printf "\n"
printf "starting make\n"
make -C ${PATH_TO_CMAKE_BUILD} $2

# run
#"./${PATH_TO_CMAKE_BUILD}/$2"
#
#ARGUMENT_TEST_IN="-s 5 -n 4 -l 5"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s 5"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-n 4"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-l 10"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-n 4 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

#ARGUMENT_TEST_IN="-s 1.1 -n 4 -l 10 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s 1 -n 4 -l 10 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s 5 -n 0 -l 10 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s 5 -n 4 -l 4 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 0"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s  -n 4 -l 10 -c 15"
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}
#
#ARGUMENT_TEST_IN="-s "
#echo "rgen ${ARGUMENT_TEST_IN}"
#"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 15 -k"
echo "rgen ${ARGUMENT_TEST_IN}"
"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

ARGUMENT_TEST_IN="-s 5 -n 4 -t 10 -c 15"
echo "rgen ${ARGUMENT_TEST_IN}"
"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 15 123456"
echo "rgen ${ARGUMENT_TEST_IN}"
"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 15 -q"
echo "rgen ${ARGUMENT_TEST_IN}"
"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 15 q"
echo "rgen ${ARGUMENT_TEST_IN}"
"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}

ARGUMENT_TEST_IN="-s 5 -n 4 -l 10 -c 21474836470"
echo "rgen ${ARGUMENT_TEST_IN}"
"./${PATH_TO_CMAKE_BUILD}/$2" ${ARGUMENT_TEST_IN}