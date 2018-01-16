
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

Summary of what gets turned in:
* A bunch of single .c files for each of the utilities below: **my-cat.c**,
  **my-grep.c**, **my-zip.c**, **my-unzip.c**, **my-sort.c**, **my-uniq.c**.
* Each should compile successfully when compiled with the **-Wall** and
**-Werror** flags.
* Each should (hopefully) pass the tests we supply to you.

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

A couple of points here. First, note that **fopen()** takes two arguments: the
*name* of the file and the *mode*. The latter just indicates what we plan to
do with the file. In this case, because we wish to read the file, we pass "r"
as the second argument. Read the man pages to see what other options are
available. 

Second, note the *critical* checking of whether the **fopen()** actually
succeeded. This is not Java where an exception will be thrown when things goes
wrong; rather, it is C, and it is expected (in good programs, you know, the
only kind you'd want to write) that you always will check if the call
succeeded. Reading the man page tells you the details of what is returned when
an error is encountered; in this case, the macOS man page says:

```
Upon successful completion fopen(), fdopen(), freopen() and fmemopen() return
a FILE pointer.  Otherwise, NULL is returned and the global variable errno is
set to indicate the error. 
```

Thus, as the code above does, please check that **fopen()** does not return
NULL before trying to use the FILE pointer it returns.

Third, note that when the error case occurs, the program prints a message and
then exits with error status of 1. In UNIX systems, it is traditional to
return 0 upon success, and non-zero upon failure. Here, we will use 1 to
indicate failure.



Side note: if **fopen()** does fail, there are many reasons possible as to
why.  You can use the functions **perror()** or **strerror()** to print out
more about *why* the error occurred; learn about those on your own (using
... you guessed it ... the man pages!).

Once a file is open, there are many different ways to read from it. The one
we're suggesting here to you is **fgets()**, which is used to get input from
files, one line at a time. 

XXX.

To print out file contents, just use **printf()**. For example, after reading
in a line with **fgets()** into a variable **buffer**, you can just print out
the buffer as follows:

```
printf("%s", buffer);
```

Note that you should *not* add a newline (\\n) character to the printf(),
because that would be changing the output of the file to have extra
newlines. Just print the exact contents of the read-in buffer (which, of
course, many include a newline).

Finally, when you are done reading and printing, use **fclose()** to close the
file (thus indicating you no longer need to read from it).





### my-cat: Error Conditions

There are XXX error conditions you should worry about when running **my-cat**.


exit(1).


### my-cat: What To Turn In

Just turn in the single source file, **my-cat.c**. To grade this, we will
compile it as follows:

```
prompt> gcc -o my-cat my-cat.c -Wall -Werror
```

If your code has warnings of any kind, it will not compile, and thus will not
pass any tests.




## my-grep

## my-zip and my-unzip

## my-sort

## my-uniq



### Footnotes


<a name="myfootnote1">1</a>: Unfortunately, there is a lot to learn about the
C library, but at some point, you've just got to **read documentation** to
learn what is available. Why not now, when you are young? Or, if you are old,
why not now, before it's ... ahem ... too late?




