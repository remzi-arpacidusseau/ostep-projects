
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

This command line tells `telnet` to connect to the machine specified by IP address `127.0.0.1` (which is just a way to refer to the machine you're running on, or, in networking parlance, `localhost`) and a port number `11211` which is the default port that `memcached` binds to. Port numbers, for those not familiar, are there to allow multiple different programs, running on the same machine, to be communicating at the same time. You can think of it (approximately) like the IP address tells you which machine you are communicating with, and the port tells you which process running on that machine to send the message to. If you don't understand all of this, it's ok! You don't really need to for this project.

If this effort is successful, you will see:
```
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
```

If not, you will see:
```
Trying 127.0.0.1...
telnet: connect to address 127.0.0.1: Connection refused
telnet: Unable to connect to remote host
```
(in which case, `memcached` probably isn't running; so, run it!).

Hopefully, it works! If so, we're now ready to type something into this second window, to send `memcached` a command. How exciting!

Let's put a key/value pair into `memcached` and then retrieve it. To put a value in, we type the following into the active `telnet` session:

```
set firstname 0 3600 5
Remzi
```

The important parts are the name of the key (`firstname`), the length parameter (`5`) which says how long the value will be, the actual value itself (in this case, `Remzi`). There are two other parameters which we will basically ignore but they are a flag (`0`) to associate with the key and an expiry (`3600`) which is the number of seconds to keep this key/value pair in the cache.

Once you have typed in those things and hit `return` (actually you have hit `return` twice, once after the `5` and once after `Remzi`), if successful, `memcached` will print out the following:

```
STORED
```

which means you have successfully stored a value into your running server. Well done!

Now, let's look up the key. To do so, we just issue a get command, by typing:

```
get firstname
```

If successful, this will output:
```
VALUE firstname 0 5
Remzi
END
```

which is what we stored. That's it!

Now, let's end the `telnet` session. You can do so by typing `quit`.

```
quit
Connection closed by foreign host.
prompt>
```

`memcached` has many other commands, including ones that work only on integer values. For example, if you store the value `9` into the firstname key, you can not only look it up with get but also do math commands like `incr` to increment it or `decr` to decrement it. For example:

```
set x 0 3600 1
9
STORED
incr x 1
10
decr x 2
8
quit
```

A little weirdly, `x` here is stored as a string, but converted to an integer to do this math and then stored back as a string.

# Feature

Now, we get to the feature set to add. It is small but to implement it you will have to understand the `memcached` code a bit. The features to add:

- A `mult` command. This should look like `incr` and `decr` but do a multiply instead, e.g., if `x` was `10` to begin, `mult x 2` should result in `x` being set to `20` (this is standard C integer multiplication, which is implemented wit the `*` operator). The first argument should be an existing key, and the second argument an integer; anything else should fail in the same manner that `incr` fails. 

- A `div` command. Same as above, but performing integer division, e.g., if `x` was `11` to begin, `div x 2` should result in `x` being set to `5` (this is standard C integer division, which is implemented with the `/` operator). The first argument should be an existing key, and the second argument an integer; anything else should fail in the same manner that `incr` fails. 

That's it! 

## Tips

Here are some tips:

- *Understand the `incr` code path first*. Do this by finding where in the code an `incr` text command is processed (`grep` for `incr` in `*.c` files is a good place to start) and then read the code. Reading and understanding code is the first key step!

- *Do little changes to test your understanding*. For example, make some changes to the `incr` command (not because you need to) just to see if your changes work. For example, change `incr` to act like `decr` and then see if your change does what you expect.

- *Determine what you can copy to make `mult` and `div` work*. Much of the code will be similar to `incr` and `decr`. What can you re-use to make it as easy as possible to realize the new features?



