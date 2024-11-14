# check parameters
if [ $# -ne 1 ]; then
    printf "usage: bash backup_analysis.sh test_case_folder_root\n"
    exit 1
fi

# create folder
v_backup_foldername="analysis_backup_$(date '+%Y-%m-%d_%H-%M-%S')"
mkdir -p "$1/${v_backup_foldername}"


# backup folder for each V
for ((v = 5; v <= 50; v += 5)); do
    printf "start to backup analysis for v = %d\n" ${v}
    v_foldername="test_case_${v}"

    # copy backup
    cp -r $1/${v_foldername} $1/${v_backup_foldername}
    printf "$1/${v_foldername} backed up to $1/${v_backup_foldername}\n"

    printf "\n"
done