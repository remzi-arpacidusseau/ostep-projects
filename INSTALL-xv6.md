
# Install Notes

To get xv6 up and running on your machine, you'll need a few tools
installed. Here, we'll walk you through what you need to do.

## The xv6 Source Code

To obtain the xv6 source code, just clone it from github:

```sh
prompt> git clone https://github.com/mit-pdos/xv6-public
loning into 'xv6-public'...
remote: Enumerating objects: 13974, done.
remote: Total 13974 (delta 0), reused 0 (delta 0), pack-reused 13974
Receiving objects: 100% (13974/13974), 17.15 MiB | 2.73 MiB/s, done.
Resolving deltas: 100% (9534/9534), done.
prompt> cd xv6-public
```

There! Now you have completed the easiest part.

## MacOS Build Environment for xv6

To play with xv6 on a Mac, you'll need to install two pieces of software. The
first is the [`qemu`](https://www.qemu.org/download/) machine simulation
environment. 

What is a simulator? TBD.
Why use one? TBD.

To install `qemu` on a Mac, just do the following:

```sh
prompt> sudo port install qemu
```

This will take a little while, so pour yourself a cup of coffee and enjoy. 

This command assumes you have [MacPorts](https://www.macports.org/)
installed. If you don't, go to the [MacPorts install
page](https://www.macports.org/install.php) and follow instructions first. 

Th command further assumes that the path to port (usually `/opt/local/bin`) is
in your path. You should probably add this directory to your path anyhow; the
`qemu` software will get installed there too.

When finished, you should be able to run the `qemu` software. To run the x86
version (which is what is needed for xv6), type the following command:

```sh
prompt> qemu-system_x86-64 -nographic
```

This will run the `qemu` emulator, but with no kernel to boot! Alas, not that
useful. 

To quit `qemu`, type `C-a x` (that is, hold down `control` and while doing so,
press `a`, then let go of `control`, then press `x`). 

The second piece of software you'll need is the `gcc` cross-compilation
toolchain. To install these pieces of software, type:

```sh
prompt> sudo port install i386-elf-gcc gdb
```

When you're done, you're ready to try to build and run xv6. Change into the
`xv6-public` directory, and type:

```sh
prompt> make TOOLPREFIX=i386-elf- qemu-nox
```

If all has worked well, you'll see something like:

```sh
Booting from Hard Disk..xv6...
cpu1: starting 1
cpu0: starting 0
sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart 2 inodestart 32 bmap
sta8
init: starting sh
$
```

The `$` is the shell command prompt: who-hoo! 

Remember to type `C-a x` to quit the emulation.

A couple of quick edits might be useful to the xv6 `Makefile`. First, change
the `TOOLPREFIX` line to uncommented, and to look as follows:

```sh
TOOLPREFIX = i386-elf-
```

This will all you to type the following simpler `make` command:

```sh
prompt> make qemu-nox
```

Note: `qemu-nox` is the way this `Makefile` allows you to build and run xv6
without spinning up a separate window, but rather running `qemu` directly
within your terminal, which we prefer. If you want the new window to pop up
instead, just run `make qemu`.

We also usually edit the `Makefile` to use one CPU (not two). Look for the
line that says:

```sh
CPUS := 2
```

and change it to:

```sh
CPUS := 1
```

Now, time to go do the projects!


## Linux

TBD.