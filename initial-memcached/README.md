
# Introduction

**Before beginning:** Read this [lab tutorial](http://pages.cs.wisc.edu/~remzi/OSTEP/lab-tutorial.pdf); it has some useful tips for programming in the C environment.

This project is a warm-up for operating systems class. It serves to get you into the mindset of a C programmer, something you will become quite familiar with over the next few months. It also gets you familliar with poking around large code bases, which is useful post-school. Finally, it's fun!

The basic idea is to work with [memcached](https://memcached.org/), a widely-used networked caching system. Many distributed services in the world are built on top of it.

You'll be adding a small feature to `memcached` for this project. Read below for details!

# Background

## Download and Build

To start using `memcached`, first you have to download it. This is readily done as follows:

```sh
prompt> curl -O http://www.memcached.org/files/memcached-1.6.17.tar.gz
```

Now you have obtained the tar'd and gzip'd source code. To build `memcached`, just follow these steps:

```sh
prompt> tar xvzf memcached-1.6.17.tar.gz
prompt> cd memcached-1.6.17
prompt> ./configure
prompt> make
```

At this point, a lot of stuff will happen to build `memcached`, but hopefully it just works and you are ready to try it out!

## Run

Now we will run `memcached`. For this, it is best to have two terminal windows open; we will assume these are on the same computer (they can be on different ones - this is a network server after all! - but then the instructions will be slightly different).

In the first terminal, we will run `memcached`. To do so, from the build directory, type the following:

```sh
prompt> ./memcached
```

To stop running it at any time, just hit `control c` (i.e., hold down the `control` key and hit the `c` key). This will send a `SIGINT` signal to `memcached`, which it will catch but in response it will exit. Try it!

Now, let's assume that `memcached` is running. In the second terminal window, we will attach to it and put some data into it, using a simple `telnet` session (if you don't know what `telnet` is, look it up!). We can do so as follows:

```sh
telnet 127.0.0.1 11211
```

If this is successful, it will say something like this:
```
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
```

If not, it will say:
```
Trying 127.0.0.1...
telnet: connect to address 127.0.0.1: Connection refused
telnet: Unable to connect to remote host
```
(in which case, `memcached` probably isn't running).

Hopefully, it works! If so, we're now ready to type something into this second window, to send `memcached` a command. How exciting!









# Feature


## Tips

Here are some tips:

- **Start small, and get things working incrementally.** 

- **Testing is critical.** A great programmer we once knew said you have to
write five to ten lines of test code for every line of code you produce;
testing your code to make sure it works is crucial. Write tests to see if your
code handles all the cases you think it should. Be as comprehensive as you can
be. Of course, when grading your projects, we will be. Thus, it is better if
you find your bugs first, before we do.

- **Keep old versions around.** Keep copies of older versions of your
program around, as you may introduce bugs and not be able to easily
undo them. A simple way to do this is to keep copies around, by
explicitly making copies of the file at various points during
development. For example, let's say you get a simple version of `kv.c`
working (say, that just reads in the file); type `cp kv.c kv.v1.c` to
make a copy into the file `kv.v1.c`. More sophisticated
developers use version control systems such as git; such a tool is
well worth learning, so do it! 

