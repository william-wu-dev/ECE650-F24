#!/usr/bin/bash

python3 a1ece650.py < test_in.txt > test_out.txt 2>test_err.txt

if [ ! -d "./test_out" ] 
then
    mkdir ./test_out
fi

if [ ! -d "./test_err" ] 
then
    mkdir ./test_err
fi

TEST_CASE_COUNT=9
readonly TEST_CASE_COUNT

for ((i=1; i<=TEST_CASE_COUNT; i++))
do
    python3 a1ece650.py < "./test_in/test_in_${i}.txt" > "./test_out/test_out_${i}.txt" 2>"./test_err/test_err_${i}.txt"
done

for ((i=1; i<=TEST_CASE_COUNT; i++))
do
    python3 a1ece650.py < "./test_in/test_in_1${i}.txt" > "./test_out/test_out_1${i}.txt" 2>"./test_err/test_err_1${i}.txt"
done
