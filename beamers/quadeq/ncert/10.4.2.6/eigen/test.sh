#!/bin/bash
input_file=./tests/test.c
num_testcases=`find . | grep 'input_' | wc -l`

# Check if gcc is installed
if command -v gcc &> /dev/null
then
    echo "gcc is installed"
else
    echo "Install GCC"
    echo "Exiting..."
    exit 1
fi

# Compile the input file
echo "Compiling..."
gcc $input_file -o $input_file.out -lm
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi
echo "Compilation successful!"

test(){
    testNo=$1
    inFile="tests/input/input_$testNo.txt"
    outFile="tests/output/output_$testNo.txt"
    expectedFile="tests/expected/expected_$testNo.txt"

    cat $inFile | "$input_file.out" > $outFile
    echo "Eigen values of input #$testNo -> $outFile"
}

for i in $(seq 1 $num_testcases)
do
    test $i
done
