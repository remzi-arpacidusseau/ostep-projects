
# Map Reduce

In 2004, engineers at Google introduced a new paradigm for large-scale
parallel data processing known as MapReduce (see the original paper
[here](https://static.googleusercontent.com/media/research.google.com/en//archive/mapreduce-osdi04.pdf),
and make sure to look in the citations at the end). One key aspect of
MapReduce is that it makes programming such tasks on large-scale clusters easy
for developers; instead of worrying about how to manage parallelism, handle
machine crashes, and many other complexities common within clusters of
machines, the developer can instead just focus on writing little bits of code
(described below) and the infrastructure handles the rest.

In this project, you'll be building a simplified version of MapReduce for just
a single machine. While somewhat easier than with a single machine, there are
still numerous challenges, mostly in building the correct concurrency
support. Thus, you'll have to think a bit about how to build the MapReduce
implementation, and then build it work efficiently and correctly.

## Background

To understand how to make progress on this project, you should understand the
basics of thread creation, mutual exclusion (with locks), and
signaling/waiting (with condition variables). These are described in the
following book chapters:

- [Intro to Threads](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf)
- [Threads API](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)
- [Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
- [Using Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf)
- [Condition Variables](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)

Read these chapters carefully in order to prepare yourself for this project.

## General Idea

Let's now get into the exact code you'll have to build. The MapReduce
infrastructure you will build supports the execution of user-defined `Map()`
and `Reduce()` functions.

As from the original paper: "`Map()`, written by the user, takes an input pair
and produces a set of intermediate key/value pairs. The MapReduce library
groups together all intermediate values associated with the same intermediate
key K and passes them to the `Reduce()` function."

"The `Reduce()` function, also written by the user, accepts an intermediate
key K and a set of values for that key. It merges together these values to
form a possibly smaller set of values; typically just zero or one output value
is produced per `Reduce()` invocation. The intermediate values are supplied to
the user's reduce function via an iterator."

A classic example, written here in pseudocode, shows how to count the number
of occurrences of each word in a set of documents:

```
map(String key, String value):
  // key: document name
  // value: document contents
  for each word w in value:
    EmitIntermediate(w, "1");

reduce(String key, Iterator values):
  // key: a word
  // values: a list of counts
  int result = 0;
  for each v in values:
    result += ParseInt(v);
  print key, result;
```

What's fascinating about MapReduce is that so many different kinds of relevant
computations can be mapped onto this framework. The original paper lists many
examples, including word counting (as above), a distributed grep, a URL
frequency access counters, a reverse web-link graph application, a term-vector
per host analysis, and others. 

What's also quite interesting is how easy it is to parallelize: many mappers
can be running at the same time, and later, many reducers can be running at
the same time. Users don't have to worry about how to parallelize their
application; rather, they just write `Map()` and `Reduce()` functions and the
infrastructure does the rest.

## Details

We give you here a `.h` file that specifies exactly what you must build in
your MapReduce library:

```
#ifndef __mapreduce_h__
#define __mapreduce_h__

// Various function pointers
typedef char *(*Getter)();
typedef void (*Mapper)(char *file_name);
typedef void (*Reducer)(char *key, Getter get_func, int get_index);
typedef unsigned long (*Partitioner)(char *key, int num_buckets);

// Key functions exported by MapReduce
void MR_Emit(char *key, char *value);
unsigned long MR_DefaultHashPartition(char *key, int num_buckets);
void MR_Run(int argc, char *argv[],
            Mapper map, int num_mappers,
            Reducer reduce, int num_reducers,
            Partitioner partition);
```





## Considerations


- **xxx**. yyy.



## Grading

Your code should turn in `mapreduce.c` which implements the above functions
correctly and efficiently. It will be compiled with test applications with the
`-Wall -Werror -pthread -O` flags; it will also be valgrinded to check for
memory errors.

Your code will first be measured for correctness, ensuring that it performs
the maps and reductions correctly. If you pass the correctness tests, your
code will be tested for performance; higher performance will lead to better
scores.




