#! /usr/bin/env bash

# run_test test_number
run_test () {
    testfile=tests/$1.run
    # cat $testfile
    $(cat $testfile) > tests-out/$1.out 2> tests-out/$1.err
    return 0
}

# check_test test_number out/err
check_test () {
    outdiff=$(diff tests/$1.$2 tests-out/$1.$2)
    outerr=$?
    if (( $outerr != 0 )); then
	builtin echo -e "\e[31mtest $testnum: standard $2 incorrect\e[0m"
	echo "should be:"
	cat tests/$1.$2
	echo "is:"
	cat tests-out/$1.$2
	exit 1
    fi
    return 0
}

if [[ ! -d tests-out ]]; then
    mkdir tests-out
fi

(( testnum = 1 ))

while true; do
    if [[ ! -f tests/$testnum.run ]]; then
	exit 0
    fi
    run_test $testnum
    check_test $testnum out
    check_test $testnum err
    builtin echo -e "\e[32mtest $testnum: passed\e[0m"
    (( testnum = $testnum + 1 ))
done

exit 0










