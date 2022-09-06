
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

