
The 'run-tests.sh' script is called by various testers to do the work of
testing. Each test is actually fairly simple: it is a comparison of standard
output and standard error, as per the program specification.

In any given program specification directory, there exists a specific `tests/`
directory which holds the expected standard output and error in files called
`n.out` and `n.err` for each test `n`. The testing framework just starts at
`1` and keeps incrementing tests until it can't find any more or encounters a
failure. Thus, adding new tests is easy; just add the relevant files to the
tests directory at the lowest available number.

A couple of other files are needed to fully describe a test. The first is a
run file (e.g., `n.run`) which shows exactly how to run the test; the second
is a description file (e.g., `n.desc`) which just describes the test.

In most cases, a wrapper script is used to call `run-tests.sh` to do the
necessary work.

The options for `run-tests.sh` include:
* `-h` (the help message)
* `-v` (verbose: print what each test is doing)
* `-t n` (run only test `n`)
* `-c` (continue even after a test fails)


