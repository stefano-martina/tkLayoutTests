#!/bin/bash
if [ $# -eq 0 ]
then
    tests=($(ls -d test*))
else
    tests=()
    for arg in "$@"
    do
        if [ -d "$arg" ]
        then
            tests+=($arg)
        fi
    done
fi

echo "The pdf for the following tests will be created:"
echo "    ${tests[@]}"
echo ""

rm csv/*

for test in "${tests[@]}"
do
    cp $test/in.csv csv/${test//\/}.csv
    echo "copied csv for $test"
done

