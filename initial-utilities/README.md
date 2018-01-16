
# Unix Utilities

In this project, you'll build a few different UNIX utilities, simple versions
of commonly used commands like **cat**, **ls**, etc. We'll call each of them a
slightly different name to avoid confusion; for example, instead of **cat**,
you'll be implementing **my-cat**.

Objectives:
* Re-familiarize yourself with the C programming language
* Re-familiarize yourself with a shell / terminal / command-line of UNIX
* Learn (as a side effect) how to use a proper code editor such as emacs
* Learn a little about how UNIX utilities are implemented

While the project focuses upon writing simple C programs, you can see from the
above that even that requires a bunch of other previous knowledge, including a
basic idea of what a shell is and how to use the command line on some
UNIX-based systems (e.g., Linux or macOS), how to use an editor such as emacs,
and of course a basic understanding of C programming. If you **do not** have
these skills already, this is not the right place to start.

## my-cat

The program **my-cat** is a simple program. Generally, it reads a file as
specified by the user and prints its contents. A typical usage is as follows,
in which the user wants to see the contents of main.c, and thus types: 

```
prompt> my-cat main.c
#include <stdio.h>
...
```

As shown, **my-cat** reads the file **main.c** and prints out its contents. 

You'll need to learn how to use a few library routines from the C standard
library (often called **libc**) to implement the source code for this program,
which we'll assume is in a file called **my-cat.c**. All C code is
automatically linked with the C library, which is full of useful functions you
can call to implement your program. Learn more about the C library
[here](https://en.wikipedia.org/wiki/C_standard_library) and perhaps
[here](https://www-s.acm.illinois.edu/webmonkeys/book/c_guide/)<sup>[1](#myfootnote1)</sup>.  

For this project, we recommend using the following routines to do file input
and output: **fopen()**, **fgets()**, and **fclose()**. Whenever you use a new
function like this, the first thing you should do is read about it -- how else
will you learn to use it properly?

On UNIX systems, the best way to read about such functions is to use what are
called the **man** pages (short for **manual**). In our HTML/web-driven world,
the man pages feel a bit antiquated, but they are useful and informative and
generally quite easy to use.

To access the man page for **fopen()**, for example, just type the following
at your UNIX shell prompt: 
```
prompt> man fopen
```

Then, read! Reading man pages effectively takes practice; why not start
learning now?

We will also give a simple overview here. The **fopen()** function "opens" a
file, which is a common way in UNIX systems to begin the process of file
access. In this case, opening a file just gives you back a pointer to a
structure of type **FILE**, which can then be passed to other routines to
read, write, etc. 

Here is a typical usage of **fopen()**:

```c
FILE *fp = fopen("main.c", "r");
if (fp == NULL) {
    printf("cannot open file\n");
    exit(1);
}
```








## my-grep

## my-zip and my-unzip

## my-sort

## my-uniq



### Footnotes


<a name="myfootnote1">1</a>: Unfortunately, there is a lot to learn about the
C library, but at some point, you've just got to **read documentation** to
learn what is available. Why not now, when you are young? Or, if you are old,
why not now, before it's ... ahem ... too late?




