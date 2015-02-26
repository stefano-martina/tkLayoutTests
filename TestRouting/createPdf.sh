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

rm pdf/*

for test in "${tests[@]}"
do
    echo "creating pdf for $test"
	  root -l "macroPdf.cpp(\"${test//\/}\")"
    mv pdf/out.pdf pdf/${test//\/}.pdf
    echo "created pdf for $test"
done

