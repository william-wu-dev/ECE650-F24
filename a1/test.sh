#!/usr/bin/bash

python3 a1ece650.py < test_in.txt > test_out.txt 2>test_err.txt

TEST_CASE_COUNT=9
readonly TEST_CASE_COUNT

for ((i=1; i<=TEST_CASE_COUNT; i++))
do
    python3 a1ece650.py < "./test_in/test_in_${i}.txt" > "./test_out/test_out_${i}.txt" 2>"./test_err/test_err_${i}.txt"
done
