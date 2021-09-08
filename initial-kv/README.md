
# Introduction

**Before beginning:** Read this [lab tutorial](http://pages.cs.wisc.edu/~remzi/OSTEP/lab-tutorial.pdf); it has some useful tips for programming in the C environment.

This project is a not-as-simple warm-up for operating systems class.
It also serves to get you into the mindset of a C programmer,
something you will become quite familiar with over the next few
months. Good luck!

You will write a simple program called `kv`. It is a simple persistent
key-value store.  Key-value storage systems, like
[RocksDB](http://rocksdb.org/) from Facebook and
[LevelDB](https://github.com/google/leveldb) from Google, are widely
used in industry for various purposes; here, you will write a simple
one (or a complex one, who knows?) and remember the basics of C and
systems programming.

The program will have a few options. The first is to insert some (key,
value) pairs into the database. This is accomplished as follows:

```sh
prompt> ./kv 
prompt> ./kv p,10,remzi
prompt> ./kv p,20,andrea p,40,someotherperson
```

The above line means the users typed in the name of the key-value
program `kv` (the `./` in front of it simply refers to the current
working directory (called dot, referred to as `.`) and the slash (`/`)
is a separator; thus, in this directory, look for a program named
`kv`) and gave it either no command-line arguments, one command-line
argument (`p,10,remzi`), or two command-line arguments (`p,20,andrea
p,40,someotherperson`).

The first invocation, with no arguments, doesn't do anything; not too
exciting, eh?

The second one is more exciting, or, at least, as exciting as a
command-line key-value store gets! It tells the key value system to
`put` a key value pair into the database (this is what the `p`
indicates), and specifically the key is equal to `10` and the value,
in this case, is equal to `remzi`.

As you can see, our simple key-value store assumes that keys are
integers, and that values are arbitrary strings (except, for
simplicity, they cannot contain a comma).

The third example just shows that the command-line interface should
allow multiple put commands (or indeed, any combination of commands)
to be specified on one command-line invocation, in this case, insert
keys `20` and `40` with values `andrea` and `someotherperson`. 

So far, so good. But can we get the values out, like any good database
should? The answer is yes!  But how? The answer is with the `get`
command, which is invoked as follows:

```sh
prompt> ./kv g,10
10,remzi
prompt>
```

Here you can see that when we `get` the key `10`, the program prints
out the key value, followed by a comma, followed by the value (in this
case, `remzi`). We accomplish this output simply by calling `printf`
and printing the results to **standard output**.

The full list of commands which your KV store should support are:
- *put*: The format is `p,key,value`, where `key` is an integer, and
`value` an arbitrary string (without commas in it).
- *get*: The format is `g,key`, where `key` is an integer. If the key
is present, the system should print out the key, followed by a comma,
followed by the value, followed by a newline (`\n`). If not present,
print an error message on a line by itself, of the form `K not found`
where `K` is the actual value of the key, i.e., some integer. 
- *delete*: The format is `d,key`, which either deletes the relevant
key-value pair (and prints nothing), or fails to do so (and prints
`K not found` where `K` is the actual value of the key, i.e., some
integer).
- *clear*: The format is `c`. This command simply removes all
key-value pairs from the database.
- *all*: The format is `a`. This command prints out all key-value
pairs in the database, in any order, with one key-value pair per line,
each key and value separated by a comma.

# Details

Here are some details that may help you complete the project.

## Persistence

One thing you have to figure out in this project is how to "persist"
the keys and values, so that they can be retrieved by later
invocations of the `kv` command. Persistence is something we cover
later in the course, but the idea here is simple: it means `kv` will
have to write out keys and values to a file (or multiple files), and
then the next time it's run, be able to read them back in in order to
fulfill requests.

For example, let's say you run the following:

```sh
prompt> ./kv p,1,first
```

The `kv` program should now store the key `1` and value `first` in its
database. Thus, when you later run `kv` and try to get the `1` key,
you get the value back:

```sh
prompt> ./kv g,1
1,first
prompt> 
```

There are many many ways to implement such a feature. Here, we suggest
something very simple. The idea will be to use a single file (called,
say, `database.txt`), where you store all of this information in
plain-text format.

For example, in a database with a few keys and values in it, you might
store the information in a plain-text file, one line per entry. The
contents of the file might then like this:

```sh
prompt> cat database.txt
1,first
2,second
prompt>
```

So, how should `kv` use this file? One simple approach is to read the
file into memory in its entirety at startup into some kind of data
structure, say a linked list or hash table. Then, when processing put,
get, delete, or other commands, all `kv` would do is update the
in-memory data structure. Then, before exiting, the program should
write out the file again, storing all key/value pairs for future use.

Of course, more sophisticated techniques can be used to improve
performance, allow efficient access to very large databases, and
tolerate crashes; none of these things are required for this project. 

## Assumptions and Errors

- **Bad command:** If the command line specifies a bad command, e.g.,
    something that is not a `p`, `g`, `a`, `c`, or `d`, print out the
    warning `bad command` on a line by itself and keep processing the
    rest of the command line; importantly, do *not* exit.

- **Unexpected error:** On any unexpected error condition, such as
    malloc() failing, or failure to open a file successfully, print
    out a useful error message and exit. This won't be tested, but may
    be useful for you during development.

## Useful Routines

For reading in the input file, the following routines will make your life
easy: `fopen()`, `getline()`, and `fclose()`.

For printing (to screen, or to a file), use `printf()`. For reading
from or writing to a file, you can use `fread()`, `fwrite()`, or
perhaps `fprintf()` or even `getline()`.

The routine `malloc()` is useful for memory allocation.

The routine `strsep()` is useful for parsing. For example, when you
get a string like `p,10,remzi`, `strsep()` can be used to split out
the different pieces.

Also useful is `atoi()` for converting a string to an integer.
  
If you don't know how to use these functions, use the man pages. For
example, typing `man malloc` at the command line will give you a lot of
information on malloc.

## Tips

Here are some tips:

- **Start small, and get things working incrementally.** For example, first
get a program that simply parses the command line successfully for one
command. Then, add a loop and parse multiple commands on one command line.
Then, add the ability to add elements to an in-memory data structure,
but don't worry about persistence. Then add persistence. Or something
like that. 

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

