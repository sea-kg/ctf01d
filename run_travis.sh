#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo ""
        echo "!!! FAIL: $3"
        echo "********************************************************************************"
        echo ""
        exit $1
    else
        echo ""
        echo "*** SUCCESS: $2"
        echo "********************************************************************************"
        echo ""
    fi
} 

./clean.sh
check_ret $? "Clean"

./build_simple.sh
check_ret $? "Build"

cd unit-tests.wsjcpp
check_ret $? "Change directory to unit-tests.wsjcpp"

./build_simple.sh
check_ret $? "Build unit-tests"

./unit-tests
check_ret $? "Run unit-tests"

cd ..
check_ret $? "Change directory to root"

## clean
./clean.sh
check_ret $? "Clean"