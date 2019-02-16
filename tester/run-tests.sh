#! /usr/bin/env bash

# run_test testnumber
run_test () {
    local testnum=$1
    local testfile=tests/$testnum.run
    eval $(cat $testfile) > tests-out/$testnum.out 2> tests-out/$testnum.err
}

print_error_message () {
    local testnum=$1
    local contrunning=$2
    local filetype=$3
    builtin echo -e "\e[31mtest $testnum: standard $filetype incorrect\e[0m"
    echo "  what results should be found in file: tests/$testnum.$filetype"
    echo "  what results produced by your program: tests-out/$testnum.$filetype"
    echo "  compare the two using diff, cmp, or related tools to debug"
    if (( $contrunning == 0 )); then
	exit 1
    fi
}

# check_test testnumber contrunning out/err
check_test () {
    local testnum=$1
    local contrunning=$2
    local filetype=$3

    # option to use cmp instead?
    returnval=$(diff tests/$testnum.$filetype tests-out/$testnum.$filetype)
    if (( $? != 0 )); then
	echo -n 1
    else
	echo -n 0
    fi
}

# run_and_check testnumber contrunning verbose printerror
#   testnumber: the test to run and check
#   printerrer: if 1, print an error if test does not exist
run_and_check () {
    local testnum=$1
    local contrunning=$2
    local verbose=$3
    local failmode=$4

    if [[ ! -f tests/$testnum.run ]]; then
	if (( $failmode == 1 )); then
	    echo "test $testnum does not exist" >&2; exit 1
	fi
	exit 0
    fi
    if (( $verbose == 1 )); then
	echo "running test $testnum"
	cat tests/$testnum.desc
    fi
    run_test $testnum
    outcheck=$(check_test $testnum $contrunning out)
    errcheck=$(check_test $testnum $contrunning err)
    # echo "results: outcheck:$outcheck errcheck:$errcheck"
    if (( $outcheck == 0 )) && (( $errcheck == 0 )); then
	builtin echo -e "\e[32mtest $testnum: passed\e[0m"
    else
	if (( $outcheck == 1 )); then
	    print_error_message $testnum $contrunning out
	fi
	if (( $errcheck == 1 )); then
	    print_error_message $testnum $contrunning err
	fi
    fi
}

# usage: call when args not parsed, or when help needed
usage () {
    echo "usage: run-tests.sh [-h] [-v] [-t test]"
    echo "  -h                help message"
    echo "  -v                verbose"
    echo "  -t n              run only test n"
    echo "  -c                continue even after failure"
    return 0
}

#
# main program
#
verbose=0
contrunning=0
specific=""

args=`getopt hvct: $*`
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
    -c)
        contrunning=1
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
    run_and_check $specific $contrunning $verbose 1
    exit 0
fi

# run all tests
(( testnum = 1 ))
while true; do
    run_and_check $testnum $contrunning $verbose 0
    (( testnum = $testnum + 1 ))
done

exit 0










