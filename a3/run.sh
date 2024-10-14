ARGUMENT_ENABLE=false
CLEAR_BUILD_FOLDER=true
SEPARATE_ERR_AND_OUT=true

# FIXME: change this to fit your project
PATH_TO_CMAKE_SOURCE="./"
PATH_TO_CMAKE_BUILD="./build"

# clear cmake build path
if ${CLEAR_BUILD_FOLDER}; then
    echo "starting to clear cmake build file"
    printf "\n"
    rm -rf ${PATH_TO_CMAKE_BUILD}/*
fi

# build based on mode

if [ $# -ne 1 ]; then
    printf "usage: bash run.sh cmake_mode_number\n"
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
make install -C ${PATH_TO_CMAKE_BUILD}

## test
#TARGET_NAME="ece650-a2"
#
#if [ ! -d "./test_out" ]; then
#    mkdir ./test_out
#else
#    rm -rf ./test_out/*
#fi
#
#if [ ! -d "./test_err" ]; then
#    mkdir ./test_err
#else
#    rm -rf ./test_err/*
#fi
#
## FIXME: change this to fit your test case count
#TEST_CASE_COUNT=11
#readonly TEST_CASE_COUNT
#
#printf "\n"
#printf "target %s test started, %d test case in total\n" "${TARGET_NAME}" ${TEST_CASE_COUNT}
#
#for ((i = 1; i <= TEST_CASE_COUNT; i++)); do
#    printf "starting test case #%d\n" ${i}
#    if ${ARGUMENT_ENABLE}; then
#        "${PATH_TO_CMAKE_BUILD}/${TARGET_NAME}" "./test_in/test_in_${i}.txt" "./test_out/test_out_${i}.txt"
#    elif ${SEPARATE_ERR_AND_OUT}; then
#        "${PATH_TO_CMAKE_BUILD}/${TARGET_NAME}" <"./test_in/test_in_${i}.txt" >"./test_out/test_out_${i}.txt" 2>"./test_err/test_err_${i}.txt"
#    else
#        "${PATH_TO_CMAKE_BUILD}/${TARGET_NAME}" <"./test_in/test_in_${i}.txt" &>"./test_out/test_out_${i}.txt"
#    fi
#done
