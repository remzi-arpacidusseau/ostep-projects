#! /bin/bash

if ! [[ -d src ]]; then
    echo "src dir does not exist"
    exit 1
fi

../tester/run-tests.sh $*


