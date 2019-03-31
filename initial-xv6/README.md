
# Intro To Kernel Hacking

To develop a better sense of how an operating system works, you will also 
do a few projects *inside* a real OS kernel. The kernel we'll be using is a
port of the original Unix (version 6), and is runnable on modern x86
processors. It was developed at MIT and is a small and relatively
understandable OS and thus an excellent focus for simple projects.

This first project is just a warmup, and thus relatively light on work. The
goal of the project is simple: to add a system call to xv6. Your system call,
**getreadcount()**, simply returns how many times that the **read()** system
call has been called by user processes since the time that the kernel was
booted. 

## Background

If you haven't watched the [discussion
video](https://www.youtube.com/watch?v=vR6z2QGcoo8), you might want to read
this [background
section](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/initial-xv6/background.md). 

More information about xv6, including a very useful book written by the MIT
folks who built xv6, is available
[here](https://pdos.csail.mit.edu/6.828/2017/xv6.html). Do note, however, that
we use a slightly older version of xv6 (for various pedagogical reasons), and
thus the book may not match our code base exactly.


## Your System Call

Your new system call should look have the following return codes and
parameters: 

```c
int getreadcount(void)
```

Your system call returns the value of a counter (perhaps called **readcount**
or something like that) which is incremented every time any process calls the
**read()** system call. That's it!

## Tips

Watch this [discussion video](https://www.youtube.com/watch?v=vR6z2QGcoo8) --
it contains a detailed walk-through of all the things you need to know to
unpack xv6, build it, and modify it to make this project successful.

One good way to start hacking inside a large code base is to find something
similar to what you want to do and to carefully copy/modify that. Here, you
should find some other system call, like **getpid()** (or any other simple
call). Copy it in all the ways you think are needed, and then modify it to do
what you need.

Most of the time will be spent on understanding the code. There shouldn't
be a whole lot of code added.

Using gdb (the debugger) may be helpful in understanding code, doing code
traces, and is helpful for later projects too. Get familiar with this fine
tool!

# Running Tests

Running tests for your system call is easy. Just do the following from
inside the `initial-xv6` directory:

```sh
prompt> ./test-getreadcounts.sh
```

If you implemented things correctly, you should get some notification
that the tests passed. If not ...

The tests assume that xv6 source code is found in the `src/` subdirectory.
If it's not there, the script will complain.

The test script does a one-time clean build of your xv6 source code
using a newly generated makefile called `Makefile.test`. You can use
this when debugging (assuming you ever make mistakes, that is), e.g.:

```sh
prompt> cd src/
prompt> make -f Makefile.test qemu-nox
```

You can suppress the repeated building of xv6 in the tests with the
`-s` flag. This should make repeated testing faster:

```sh
prompt> ./test-getreadcounts.sh -s
```

The other usual testing flags are also available. See [the testing
README](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/tester/README.md)
for details.

