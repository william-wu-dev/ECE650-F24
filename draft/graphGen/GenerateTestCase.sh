NUMBER_OF_GRAPH_FOR_EACH_V=10

# check graphGen availability
if [ ! -f "./graphGen" ]; then
    echo "./graphGen not found, please place it in the same folder as this script."
    exit 1
fi

# use a unique folder name for one generation
foldername="test_case_$(date '+%Y-%m-%d_%H-%M-%S')"
mkdir ${foldername}
printf "folder for all test cases created: ${foldername}\n"
printf "\n"

# create folder for each V
for ((v = 5; v <= 50; v += 5)); do
    printf "start to create test case for v = %d\n" ${v}
    # create folder
    v_foldername="test_case_${v}"
    v_in_foldername="test_in_${v}"
    mkdir -p "${foldername}/${v_foldername}/${v_in_foldername}"
    printf "test in folder created under: ${foldername}/${v_foldername}/${v_in_foldername}\n"

    # generate NUMBER_OF_GRAPH_FOR_EACH_V graphs for each v and stored there 
    for ((i = 1; i <= NUMBER_OF_GRAPH_FOR_EACH_V; i++)); do
        ./graphGen ${v} > "${foldername}/${v_foldername}/${v_in_foldername}/test_in_${v}_${i}.txt"
        printf "test in for v = %d graph #%d created\n" ${v} ${i}
    done
    printf "\n"
done