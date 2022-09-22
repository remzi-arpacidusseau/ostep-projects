
# Intro To Kernel Hacking

To develop a better sense of how an operating system works, you will also 
do a few projects *inside* a real OS kernel. The kernel we'll be using is a
port of the original Unix (version 6), and is runnable on modern x86
processors. It was developed at MIT and is a small and relatively
understandable OS and thus an excellent focus for simple projects.

This first project is just a warmup, and thus relatively light on work. The
goal of the project is simple: to add some system calls to xv6. The first
system call is called `trace(const char *pathname)` and the second is `getcount()`.
The first simply tells the OS to track how many times a particular file,
specified by the name `pathname`, has been opened. The second returns
that count. 

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
int trace(const char *pathname)
int getcount(void)
```

The first, `trace()`, simply records the pathname specified by the parameter `pathname`, into
some known location (e.g., an array of characters called `trace_pathname`). 
It should also reset the trace counter to 0, and enable tracing (i.e., set some kernel
internal integer, such as `trace_enabled`, to 1).

It should return 0 upon success, and -1 upon failure (e.g., if someone passes in a null pointer).
Note: assume for simplicity that the max pathname is 256 bytes. Also, it is ok to trace
a file that does not yet exist, so no need to check that.

The second, `getcount()`, returns the value of a counter (perhaps called `trace_counter`
or something like that). This counter should be incremented every time any process calls the
`open()` system call if the pathname passed to open matches the pathname being traced.
If tracing has not yet been enabled, it should simply return 0.

The trace count is *global*, which means there is one trace counter that gets enabled,
and it traces activity across all processes.

## Tips

Watch this [discussion video](https://www.youtube.com/watch?v=vR6z2QGcoo8) --
it contains a detailed walk-through of all the things you need to know to
unpack xv6, build it, and modify it to make this project successful.

One good way to start hacking inside a large code base is to find something
similar to what you want to do and to carefully copy/modify that. Here, you
should find some other system call, like `getpid()` (or any other simple
call). Copy it in all the ways you think are needed, and then modify it to do
what you need.

Most of your code should be in the file `sysfile.c`, which is where the
system call `open()` is defined (it's actually called `sys_open()` inside
the kernel). Thus, you should add your two system call definitions in here
(of course, other code has to change in order to add those two system calls).

Most of the time will be spent on understanding the code. There shouldn't
be a whole lot of code added.

Start slowly. For example, at first, just make very simple system calls
that just return the value 0 (but don't do anything new). Then add a trace
counter and have `getcount()` return its value, but don't yet implement
the tracing. Then finally figure out how to do the tracing.

`cprintf()` can be used in the kernel to print out debugging info.

You may need to implement some routines in the kernel, such as `strcmp()`
and `strcpy()`, to implement these system calls. You can always copy
the definitions of these routines from `ulib.c` (this is where they
are defined for user processes).

Using gdb (the debugger) may be helpful in understanding code, doing code
traces, and is helpful for later projects too. Get familiar with this fine
tool!

# Running Tests

We will share more details about running tests shortly.

