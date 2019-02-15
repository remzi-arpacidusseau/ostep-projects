#! /usr/bin/env bash

# run_test testnumber
run_test () {
    testfile=tests/$1.run
    # cat $testfile
    $(cat $testfile) > tests-out/$1.out 2> tests-out/$1.err
    return 0
}

# check_test testnumber out/err
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

# run_and_check testnumber printerror
#   testnumber: the test to run and check
#   printerrer: if 1, print an error if test does not exist
run_and_check () {
    if [[ ! -f tests/$1.run ]]; then
	if (( $2 == 1 )); then
	    echo "test $1 does not exist" >&2; exit 1
	fi
	exit 0
    fi
    run_test $1
    check_test $1 out
    check_test $1 err
    builtin echo -e "\e[32mtest $1: passed\e[0m"
    return 0
}

# usage: call when args not parsed, or when help needed
usage () {
    echo "usage: run-tests.sh [-h] [-v] [-t test]"
    return 0
}

#
# main program
#
verbose=0
specific=""

args=`getopt hvt: $*`
if [[ $? != 0 ]]; then
    usage; exit 1
fi

set -- $args
for i; do
    case "$i" in
    -h)
	usage
	exit 0
        shift;;
    -v)
        verbose=1
        shift;;
    -t)
        specific=$2
	shift
	number='^[0-9]+$'
	if ! [[ $specific =~ $number ]]; then
	    usage
	    echo "-t must be followed by a number" >&2; exit 1
	fi
        shift;;
    --)
        shift; break;;
    esac
done

# need a test directory; must be named "tests-out"
if [[ ! -d tests-out ]]; then
    mkdir tests-out
fi

# run just one test
if [[ $specific != "" ]]; then
    run_and_check $specific 1
    exit 0
fi

# run all tests
(( testnum = 1 ))
while true; do
    run_and_check $testnum 0
    (( testnum = $testnum + 1 ))
done

exit 0










