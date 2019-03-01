
# Projects for an Operating Systems Class

This repository holds a number of projects that can be used in an operating
systems class aimed at upper-level undergraduates and (in some cases)
beginning graduate students. They are based on years of teaching such a course
at the University of Wisconsin-Madison.

Also (increasingly) available are some tests to see if your code works; eventually
every project will have a set of tests available. The testing framework that is
currently available is found [here](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/tester).
A specific testing script, found in each project directory, can be used to run
the tests against your code. For example, in the initial utilities project, the
relatively simple `wcat` program that you create can be tested by running the 
`test-wcat.sh` script.

## C/Linux Projects

### Initial Projects

These projects are meant to get you warmed up with programming in the C/UNIX
environment. None are meant to be particularly hard, but should be enough so
that you can get more comfortable programming. 

Realize the best thing you can do to learn to program in any environment is to
program **a lot**. These small projects are only the beginning of that
journey; you'll have to do more on your own to truly become proficient.

* [Unix Utilities](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-utilities) (cat, grep, zip/unzip)
* Sort (text-based)
* Sort (binary)
* [Reverse](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-reverse) (very simple reverse program)

### Processes and Scheduling

* [Shell](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/processes-shell)

### Virtual Memory

* Memory Allocator

### Concurrency

* [Web Server](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-webserver)
* [Parallel Zip](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-pzip)
* [MapReduce](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-mapreduce)
* Web Crawler

### File Systems

* [File System Checker](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/filesystems-checker)

### Distributed Systems


## Kernel Hacking Projects (xv6)

These projects all are to be done inside the
[xv6](https://pdos.csail.mit.edu/6.828/2017/xv6.html) kernel developed at
MIT. Unlike the C/Linux projects, these give you direct experience inside a
real, working operating system (albeit a simple one).

### Initial Projects

* [Intro To xv6](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/initial-xv6)

### Processes and Scheduling

* [Scheduling (Lottery)](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/scheduling-xv6-lottery)

### Virtual Memory

* [Virtual Memory (Null Pointer and Read-Only Regions)](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/vm-xv6-intro)

### Concurrency

* [Kernel Threads (Basic Implementation)](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-xv6-threads)

### File Systems



