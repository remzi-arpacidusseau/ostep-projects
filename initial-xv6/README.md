
# Intro To Kernel Hacking

To develop a better sense of how an operating system works, you will also 
do a few projects *inside* a real OS kernel. The kernel we'll be using is a
port of the original Unix (version 6), and is runnable on modern x86
processors. It was developed at MIT and is a small and relatively
understandable OS and thus an excellent focus for simple projects.
More information about xv6, including a very useful book which you might want
to read, is available [here](https://pdos.csail.mit.edu/6.828/2017/xv6.html).

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




