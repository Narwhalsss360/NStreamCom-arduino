#!/bin/bash

printf "[Building...]\n"
make clean > /dev/null 2>&1
output=$(make)
build_code=$?
if [ $build_code -eq 0 ]; then
    printf "Build successful:\n"
    echo $output
    printf "\n"
else
    printf "Build failed with %d\n" $build_code
    exit $build_code
fi

cd tests
for test in $(ls)
do
    printf "[Testing %s...]\n" $test
    cd $test

    make clean > /dev/null 2>&1

    output=$(make)
    build_code=$?
    if [ ! $build_code -eq 0 ]; then
        printf "Build for %s failed with %d\n" $test $build_code
        exit $build_code
    fi

    program=./$test.out
    output=$(eval $program)
    program_code=$?
    if [ ! $program_code -eq 0 ]; then
        printf "Test %s failed with %d\n" $test $program_code
    else
        echo $output
    fi

    make clean > /dev/null
    cd ..
    printf "\n"
done
