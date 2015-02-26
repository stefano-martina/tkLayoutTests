#!/bin/bash

timestamp=$(date +"%s")

if [ $# -eq 0 ]
then
    #tests=(test1a test1b test1c test1d test2 test3 test3bis test3ter test3quater test4 test5 test6 test7 test8 test9)
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

echo "The following tests will be updated:"
echo "    ${tests[@]}"
echo ""

if [ -d ~/Cern/tklayout-dir-conf ]
then
    mv ~/Cern/tklayout-dir-conf  ~/Cern/tklayout-dir-conf-BAKTEST$timestamp
fi

for test in "${tests[@]}"
do
    echo "updating $test"
    ln -s `pwd`/$test/conf ~/Cern/tklayout-dir-conf
    cd ~/Cern/tklayout-dir-conf/geometries/BarrelEndcap_Nick2013/Traditional&& ~/Cern/tklayout-trunk/bin/tklayout -m -N 3000 TechnicalProposal2014.cfg&& cd -&& rm -fR $test/www/TechnicalProposal2014 && cp -R ~/Cern/tklayout-dir-www/TechnicalProposal2014 $test/www/&& rm $test/orig.root&& cp $test/www/TechnicalProposal2014/matFull001.root $test/orig.root
    rm ~/Cern/tklayout-dir-conf
    echo "updated $test"
done

if [ -d ~/Cern/tklayout-dir-conf-BAKTEST$timestamp ]
then
    mv ~/Cern/tklayout-dir-conf-BAKTEST$timestamp ~/Cern/tklayout-dir-conf
fi


