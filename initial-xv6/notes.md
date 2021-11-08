# P1b Notes

## install qemu

### Ubuntu

```bash
apt install qemu-system-x86
```

You may also need to install some other package.

### OS X

Go to [Install Notes](../INSTALL-xv6.md)

## qemu usage

```bash
qemu-system-x86 --nographic
```

> This will run the `qemu` emulator, but with no kernel to boot! 

> --nographic option: disable graphical output and redirect serial I/Os to console

### how to quit from qemu

`ctrl - a ` then release both and click `x` 

## xv6 install

```bash
git clone git://github.com/mit-pdos/xv6-public.git
cd xv6-public
make qemu-nox
```

> Note: `qemu-nox` is the way this `Makefile` allows you to build and run xv6 without spinning up a separate window, but rather running `qemu` directly within your terminal, which we prefer. If you want the new window to pop up instead, just run `make qemu`.

We also usually edit the `Makefile` to use one CPU (not two). Look for the line that says:

```
CPUS := 2
```

and change it to:

```
CPUS := 1
```

## Task

### hello example in video

1. Add a hello.c file under xv6 directory with following codes.

```c
#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  printf(1, "your name is '%s'\n", argv[0]);
  return 0;
}
```

2. Add the program in `Makefile` under `UPROGS` 

```makefile
...
UPROGS=\
        _cat\
        _echo\
        _forktest\
        _grep\
        _init\
        _kill\
        _ln\
        _ls\
        _mkdir\
        _rm\
        _sh\
        _stressfs\
        _usertests\
        _wc\
        _zombie\
        _hello\ # this line
...
```

3. Run `make qemu-nox` 
4. Run `hello` in qemu

### q1b 

Move `xv6-public` to `ostep-projects/initial-xv6/` as `src` 

> The tests assume that xv6 source code is found in the `src/` subdirectory.

```c
# src/sysfile.c
#...
int readcount=0;
int
sys_getreadcount(void)
{
  return readcount;
}

int
sys_read(void)
{
  struct file *f;
  int n;
  char *p;
  
  // put incremention here, because in test cases call read() method failed(always return 
  // -1), and according to the instructions, it should count just the number how many times 
  // the read method is called.
  readcount++;

  if(argfd(0, 0, &f) < 0 || argint(2, &n) < 0 || argptr(1, &p, n) < 0)
    return -1;
  return fileread(f, p, n);
}
#...
```

Also need to add some difinition of the syscall method in `syscall.c` , `syscall.h` , `user.h` and `usys.S` 

Run test: `./test-getreadcount.sh` 

#### Tips

+ You can run `make -f Makefile.test qemu-nox` to do testing/debugging after you ran the test. 

+ Test case in `src/test_1`  ... `tests/1.run` 
