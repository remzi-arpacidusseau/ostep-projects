
# Parallel Sort

Sorting, or alphabetizing as you called it as a child, is still a
critical task for data-intensive applications, including databases,
spreadsheets, and many other data-oriented applications. In this
project, you'll be building a high-performance parallel sort. 

There are three specific objectives to this assignment:

* To familiarize yourself with the Linux pthreads.
* To learn how to parallelize a program.
* To learn how to program for high performance.

## Background

To understand how to make progress on any project that involves concurrency,
you should understand the basics of thread creation, mutual exclusion (with
locks), and signaling/waiting (with condition variables). These are described
in the following book chapters:

- [Intro to Threads](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf)
- [Threads API](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)
- [Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
- [Using Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf)
- [Condition Variables](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)

Read these chapters carefully in order to prepare yourself for this project.

## Project Specification

Your parallel sort (`psort`) will take two command-line arguments.

```
prompt> ./psort input output
```

The input file will consist of records; within each record is a
key. The key is the first four bytes of the record. The records are
fixed-size, and are each 100 bytes (which includes the key).

A successful sort will read all the records into memory from the input
file, sort them, and then write them out to the output file.

You also have to force writes to disk by calling `fsync()` on the output file before finishing.

You can assume that this is a one-pass sort, i.e., the data can fit
into memory. You do not have to implement a multi-pass sort.

## Considerations

Doing so effectively and with high performance will require you to address (at
least) the following issues:

- **How to parallelize the sorting.** Of course, the central challenge of
    this project is to parallelize the sorting process. Think about what
    can be done in parallel, and what must be done serially by a single
    thread, and design your parallel sort as appropriate.

    One interesting issue that the "best" implementations will handle is this:
    what happens if one thread runs more slowly than another? Does the
    sort give more work to faster threads? 

- **How to determine how many threads to create.** On Linux, this means using
    interfaces like `get_nprocs()` and `get_nprocs_conf()`; read the man pages
    for more details. Then, create threads to match the number of CPU
    resources available.

- **How to efficiently perform each piece of work.** While parallelization
    will yield speed up, each thread's efficiency in performing the
    sorting is also of critical importance. You can glean some hints
    from papers like the famous [AlphaSort paper](https://www.cs.cmu.edu/~natassa/courses/15-721/papers/P233.PDF).

- **How to access the input/output files efficiently.** On Linux, there are many ways
    to read from a file, including C standard library calls like `fread()` and
    raw system calls like `read()`. One particularly efficient way is to use
    memory-mapped files, available via `mmap()`. By mapping the input file
    into the address space, you can then access bytes of the input file via
    pointers and do so quite efficiently. Similarly, how you write the
    output, and perhaps, how you overlap writing with sorting, can
    make your sort run faster.

## Grading

Your code should compile (and should be compiled) with the following flags:
`-Wall -Werror -pthread -O`. The last one is important: it turns on the
optimizer! In fact, for fun, try timing your code with and without `-O` and
marvel at the difference.

Your code will first be measured for correctness, ensuring that it sorts input
files correctly.

If you pass the correctness tests, your code will be tested for performance;
higher performance will lead to better scores. The fastest sort will
be declared the "fastest sorter" and appropriate awards will be given.

