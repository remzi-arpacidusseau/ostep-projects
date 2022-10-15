#! /bin/bash

if ! [[ -x bank ]]; then
    echo "bank executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*

