#! /bin/bash

if ! [[ -x wunzip ]]; then
    echo "wunzip executable does not exist"
    exit 1
fi

../../tester/run-tests.sh $*


