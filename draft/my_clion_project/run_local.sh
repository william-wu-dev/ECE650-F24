# check if exist
# FIXME: comment out if this is on linux
if [ ! -d "/Volumes/macOS/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/" ]; then
    printf "/Volumes/macOS/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/ no such directory, please plug in the external disk\n"
    exit 1
fi

# prepare for cmake
# FIXME: comment out if this is on linux
export PATH="/Volumes/macOS/Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/:${PATH}"

# FIXME: change this to fit your project
PATH_TO_SOURCE="./"
PATH_TO_BUILD="./cmake-build-debug"

# clear cmake build path
rm -rf ${PATH_TO_BUILD}/*

# build based on mode

if [ $# -ne 1 ]; then
    printf "only 1 argument is allowed to specify cmake mode\n"
    exit 1
fi

if [ $1 -eq 0 ]; then
    printf "starting cmake \n"
    cmake -DCMAKE_BUILD_TYPE=Debug -S ${PATH_TO_SOURCE} -B ${PATH_TO_BUILD}
elif [ $1 -eq 1 ]; then
    printf "starting cmake \n"
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S ${PATH_TO_SOURCE} -B ${PATH_TO_BUILD}
elif [ $1 -eq 2 ]; then
    printf "starting cmake \n"
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DWITH_ASAN=ON -S ${PATH_TO_SOURCE} -B ${PATH_TO_BUILD}
else
    printf "unknown argument \"$1\", 0 for plain cmake, 1 for clang++ compile, 2 for address sanination activation.\n"
    exit 1
fi

# make in a directory
printf "\nstarting make\n"
make -C ${PATH_TO_BUILD}

# test
# FIXME: change this to your target name
TARGET_NAME="my_clion_project"

if [ ! -d "./test_out" ]; then
    mkdir ./test_out
else
    rm -rf ./test_out/*
fi

if [ ! -d "./test_err" ]; then
    mkdir ./test_err
else
    rm -rf ./test_err/*
fi

# FIXME: change this to fit your test case count
TEST_CASE_COUNT=3
readonly TEST_CASE_COUNT

printf "\ntarget ${TARGET_NAME} test started, ${TEST_CASE_COUNT} test case in total\n"

for ((i = 1; i <= TEST_CASE_COUNT; i++)); do
    printf "starting test case #${i}\n"
    "${PATH_TO_BUILD}/${TARGET_NAME}" <"./test_in/test_in_${i}.txt" >"./test_out/test_out_${i}.txt" 2>"./test_err/test_err_${i}.txt"
done
