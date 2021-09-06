#! /bin/bash

if ! [[ -x kv ]]; then
    echo "kv executable does not exist"
    exit 1
fi

../tester/run-tests.sh $*






