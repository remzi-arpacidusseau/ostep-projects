
# Distributed File System

In this assignment, you will be developing a working *distributed file
server.* We provide you with only the bare minimal UDP communication
code; you have to build the rest. 

## A Basic File Server

Your file server is built as a stand-alone UDP-based server. It should wait
for a message and then process the message as need be, replying to the given
client.

Your file server will store all of its data in an on-disk file which will
be referred to as the *file system image*. This image contains the on-disk
representation of your data structures; you should use these system calls
to access it: `open(), read(), write(), lseek(), close(), fsync().`

To access the file server, you will be building a client library. The
interface that the library supports is defined in [mfs.h](mfs.h).  The
library should be called `libmfs.so`, and any programs that wish to access
your file server will link with it and call its various routines.

## On-Disk File System: A Log-Structured File System

Your on-disk file system structures should roughly follow that of the
log-structured file system discussed
[here](https://pages.cs.wisc.edu/~remzi/OSTEP/file-lfs.pdf). On-disk,
the first structure should be a singular checkpoint region. The
checkpoint region should contain a disk pointer to the current end of
the log; it should also contain pointers to pieces of the inode map
(assume there are a maximum of 4096 inodes; assume each piece of the
inode map has 16 entries).

Other than the checkpoint region, your on-disk image just consists of an
ever-growing log (i.e., we won't be implementing cleaning). Thus, whenever you
write to the disk, you'll just write all file system updates to the end of the
log, and then update the checkpoint region as need be. For example, if you are
adding a new block to a file, you would write the data block, new version of
the inode, and a new piece of the inode map to the end of the log; when this
write completes, you should update the checkpoint region with the requisite
new values. 

The inode map is just an array, indexed by inode number. Each entry is a
simple 4-byte integer, which is just the disk address of the location of the
inode in question.

Each inode should be simple: a size field (the number of the last byte in
the file), a type field (regular or directory), and 14 direct pointers; thus,
the maximum file size is 14 times the 4KB block size, or 56 KB.

One other structure you'll have to manage on disk are
directories. Each directory has an inode, and points to one or more
data blocks that contain directory entries. Each directory entry
should be simple, and consist of 32 bytes: a name and an inode number
pair. The name should be a fixed-length field of size 28 bytes; the
inode number is just an integer (4 bytes). When a directory is
created, it should contain two entries: the name `.` (dot), which
refers to this new directory's inode number, and `..` (dot-dot), which
refers to the parent directory's inode number. For directory entries
that are not yet in use (in an allocated 4-KB directory block), the
inode number should be set to -1. This way, utilities can scan through
the entries to check if they are valid.

When your server is started, it is passed the name of the file system
image file. If this file does not exist, the file server should create
it, and initialize it properly, and force it to disk. Such
initialization includes creating the checkpoint region, the initial
inode map, and creating a single root directory with proper `.` and
`..` entries. The root inode number should be 0.

When booting off of an existing image, your server should read in the
checkpoint region (and keep an in-memory version of it), as well as the entire
inode map and keep it in-memory too.


## Client library

The client library should export the following interfaces:

- `int MFS_Init(char *hostname, int port)`: `MFS_Init()` takes a host name
and port number and uses those to find the server exporting the file system.
- `int MFS_Lookup(int pinum, char *name)`: `MFS_Lookup()` takes the parent
inode number (which should be the inode number of a directory) and looks up
the entry `name` in it. The inode number of `name` is returned. Success: 
return inode number of name; failure: return -1. Failure modes: invalid pinum,
name does not exist in pinum.
- `int MFS_Stat(int inum, MFS_Stat_t *m)`: `MFS_Stat()` returns some
information about the file specified by inum. Upon success, return 0,
otherwise -1. The exact info returned is defined by `MFS_Stat_t`. Failure modes:
inum does not exist. 
- `int MFS_Write(int inum, char *buffer, int block)`: `MFS_Write()` writes a
block of size 4096 bytes at the block offset specified by `block`. Returns 0
on success, -1 on failure. Failure modes: invalid inum, invalid block, not a
regular file (because you can't write to directories).
- `int MFS_Read(int inum, char *buffer, int block)`: `MFS_Read()` reads
a block specified by `block` into the buffer from file specified by
`inum`. The routine should work for either a file or directory;
directories should return data in the format specified by
`MFS_DirEnt_t`. Success: 0, failure: -1. Failure modes: invalid inum,
invalid block. 
- `int MFS_Creat(int pinum, int type, char *name)`: `MFS_Creat()` makes a
file (`type == MFS_REGULAR_FILE`) or directory (`type == MFS_DIRECTORY`)
in the parent directory specified by `pinum` of name `name`. Returns 0 on
success, -1 on failure. Failure modes: pinum does not exist, or name is too
long. If `name` already exists, return success (think about why).
- `int MFS_Unlink(int pinum, char *name)`: `MFS_Unlink()` removes the file or
directory `name` from the directory specified by `pinum`. 0 on success, -1
on failure. Failure modes: pinum does not exist, directory is NOT empty. Note
that the name not existing is NOT a failure by our definition (think about why
this might be). 
- `int MFS_Shutdown()`: `MFS_Shutdown()` just tells the server to force all
of its data structures to disk and shutdown by calling `exit(0)`. This interface
will mostly be used for testing purposes.


## Server Idempotency

The key behavior implemented by the server is *idempotency*.
Specifically, on any change to the file system state (such as a
`MFS_Write`, `MFS_Creat`, or `MFS_Unlink`), all the dirtied buffers in the
server are committed to the disk.  The server can achieved this end by
calling `fsync()` on the file system image. Thus, before returning a
success code, the file system should always `fsync()` the image.

Now you might be wondering: why do this? Simple: if the server crashes, the
client can simply timeout and retry the operation and know that it is OK to do
so. Read [this chapter](https://pages.cs.wisc.edu/~remzi/OSTEP/dist-nfs.pdf) on NFS
for details. 

Now you might be wondering: how do I implement a timeout? Simple, with the
`select()` interface. The `select()` calls allows you to wait for a reply
on a certain socket descriptor (or more than one, though that is not needed
here). You can even specify a timeout so that the client does not block
forever waiting for data to be returned from the server. By doing so, you can
wait for a reply for a certain amount of time, and if nothing is returned, try
the operation again until it is successful.

## Program Specifications

Your server program must be invoked exactly as follows:

prompt> server [portnum] [file-system-image]

The command line arguments to your file server are to be interpreted as follows.  

- portnum: the port number that the file server should listen on.
- file-system-image: a file that contains the file system image.

If the file system image does not exist, you should create it and properly
initialize it to include an empty root directory.

Your client library should be called `libmfs.so`. It should implement
the interface as specified by `mfs.h`, and in particular deal with
the case where the server does not reply in a timely fashion; the way
it deals with that is simply by retrying the operation, after a
timeout of some kind (default: five second timeout).

## Relevant Chapters

Read these:
- [Log-structured File System](https://pages.cs.wisc.edu/~remzi/OSTEP/file-lfs.pdf)
- [Distributed Systems](https://pages.cs.wisc.edu/~remzi/OSTEP/dist-intro.pdf)
- [Distributed File System: NFS](https://pages.cs.wisc.edu/~remzi/OSTEP/dist-nfs.pdf)


## Some Helper Code

To get you going, we have written some simple UDP code that can send a
message and then receive a reply from a client to a server. It can be found in
[here](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/dist-intro).

You'll also have to learn how to make a shared library. Read [here](https://tldp.org/HOWTO/Program-Library-HOWTO/shared-libraries.html) for more information.






