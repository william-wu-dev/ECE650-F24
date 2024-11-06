ARGUMENT_ENABLE=false
CLEAR_BUILD_FOLDER=false
SEPARATE_ERR_AND_OUT=true
REPLACE_MINISAT=false

# change this to fit your project
PATH_TO_CMAKE_SOURCE="./"
PATH_TO_CMAKE_BUILD="./build"

# check parameters
if [ $# -ne 3 ]; then
    printf "usage: bash analysis.sh cmake_mode_number test_case_folder_root graph_vertex_number_v\n"
    exit 1
fi

# check and copy minisat library
if ${REPLACE_MINISAT}; then
  echo "starting to clear minisat"
      printf "\n"
    rm -r "./minisat"
fi

if [ ! -d "./minisat" ]; then
  echo "starting to copy minisat"
      printf "\n"
    cp -r "../../lib/minisat" "./minisat"
fi

# clear cmake build path
if ${CLEAR_BUILD_FOLDER}; then
    echo "starting to clear cmake build file"
    printf "\n"
    rm -rf ${PATH_TO_CMAKE_BUILD}/*
fi

# build based on mode

if [ "$1" -eq 0 ]; then
    printf "starting cmake default\n"
    cmake -DCMAKE_BUILD_TYPE=Debug -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
elif [ "$1" -eq 1 ]; then
    printf "starting cmake for clang++ compile\n"
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
elif [ "$1" -eq 2 ]; then
    printf "starting cmake for address sanination compile\n"
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DWITH_ASAN=ON -S ${PATH_TO_CMAKE_SOURCE} -B ${PATH_TO_CMAKE_BUILD}
elif [ $1 -eq 3 ]; then
    printf "skip cmake\n"
else
    printf "unknown argument \"%s\", 0 for plain cmake, 1 for clang++ compile, 2 for address sanitation activation, 3 for don't build it again.\n" "$1"
    exit 1
fi

# make in a directory
printf "\n"
if [ $1 -ne 3 ]; then
    printf "starting make\n"
    make -C ${PATH_TO_CMAKE_BUILD}
else
    printf "skip make\n"
fi

## test
TARGET_NAME="ece650-prj"

# change this to fit your test case count
NUMBER_OF_RUNS_FOR_EACH_GRAPH=10
NUMBER_OF_GRAPH_FOR_EACH_V=10

printf "\n"
printf "===================Analysis Session Info======================\n"
printf "1. target name: %s\n" "${TARGET_NAME}"
printf "2. test case folder root: %s\n" $2
printf "3. current test with graph of vertices number: %d\n" $3
printf "4. number of graphs in this number of verticies: %d\n" ${NUMBER_OF_GRAPH_FOR_EACH_V}
printf "5. number of runs for each graph: %d\n" ${NUMBER_OF_RUNS_FOR_EACH_GRAPH}
printf "==============================================================\n"

folder_for_this_session="$2/test_case_$3"
# checking folders
if [ ! -d "${folder_for_this_session}/test_out_$3" ]; then
    mkdir ${folder_for_this_session}/test_out_$3
else
    rm -rf ${folder_for_this_session}/test_out_$3/*
fi

if [ ! -d "${folder_for_this_session}/test_err_$3" ]; then
    mkdir ${folder_for_this_session}/test_err_$3
else
    rm -rf ${folder_for_this_session}/test_err_$3/*
fi

# begin run
for ((i = 1; i <= NUMBER_OF_GRAPH_FOR_EACH_V; i++)); do
    printf "begin test on test_in_$3_%d.txt: \n" $i
    for ((j = 1; j <= NUMBER_OF_RUNS_FOR_EACH_GRAPH; j++)); do
        "${PATH_TO_CMAKE_BUILD}/${TARGET_NAME}" <"${folder_for_this_session}/test_in_$3/test_in_$3_${i}.txt" >>"${folder_for_this_session}/test_out_$3/test_out_$3_${i}.csv" 2>>"${folder_for_this_session}/test_err_$3/test_err_$3_${i}.txt"
        printf "%d.." $j
    done
    # add a function to the csv file
    printf "=AVERAGE(A1:A10),=AVERAGE(B1:B10),=AVERAGE(C1:C10),=AVERAGE(D1:D10),=AVERAGE(E1:E10),=AVERAGE(F1:F10)," >> "${folder_for_this_session}/test_out_$3/test_out_$3_${i}.csv"
    printf "\n"
done
