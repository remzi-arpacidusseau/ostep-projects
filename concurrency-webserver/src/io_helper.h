#ifndef __IO_HELPER__
#define __IO_HELPER__

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct sockaddr sockaddr_t;

// useful here: gcc statement expressions
// http://gcc.gnu.org/onlinedocs/gcc/Statement-Exprs.html
// macro ({ ...; x; }) returns value 'x' for caller
// e.g., macro 'fork_or_die()' below returns 'pid' value
#define fork_or_die() \
    ({ pid_t pid = fork(); assert(pid >= 0); pid; })
#define execve_or_die(filename, argv, envp) \
    assert(execve(filename, argv, envp) == 0); 
#define wait_or_die(status) \
    ({ pid_t pid = wait(status); assert(pid >= 0); pid; })
#define gethostname_or_die(name, len) \
    ({ int rc = gethostname(name, len); assert(rc == 0); rc; })
#define setenv_or_die(name, value, overwrite) \
    ({ int rc = setenv(name, value, overwrite); assert(rc == 0); rc; })
#define chdir_or_die(path) \
    assert(chdir(path) == 0); 
#define open_or_die(pathname, flags, mode) \
    ({ int rc = open(pathname, flags, mode); assert(rc >= 0); rc; })
#define read_or_die(fd, buf, count) \
    ({ ssize_t rc = read(fd, buf, count); assert(rc >= 0); rc; })
#define write_or_die(fd, buf, count) \
    ({ ssize_t rc = write(fd, buf, count); assert(rc >= 0); rc; })
#define lseek_or_die(fd, offset, whence) \
    ({ off_t rc = lseek(fd, offset, whence); assert(rc >= 0); rc; })
#define close_or_die(fd) \
    assert(close(fd) == 0); 
#define select_or_die(n, readfds, writefds, exceptfds, timeout) \
    ({ int rc = select(n, readfds, writefds, exceptfds, timeout); assert(rc >= 0); rc; })
#define dup2_or_die(fd1, fd2) \
    ({ int rc = dup2(fd1, fd2); assert(rc >= 0); rc; })
#define stat_or_die(filename, buf) \
    assert(stat(filename, buf) >= 0);
#define fstat_or_die(fd, buf) \
    { assert(fstat(fd, buf) >= 0); }
#define mmap_or_die(addr, len, prot, flags, fd, offset) \
    ({ void *ptr = mmap(addr, len, prot, flags, fd, offset); assert(ptr != (void *) -1); ptr; })
#define munmap_or_die(start, length) \
    assert(munmap(start, length) >= 0); 
#define socket_or_die(domain, type, protocol) \
    ({ int rc = socket(domain, type, protocol); assert(rc >= 0); rc; })
#define setsockopt_or_die(s, level, optname, optval, optlen) \
    { assert(setsockopt(s, level, optname, optval, optlen) >= 0); }
#define bind_or_die(sockfd, my_addr, addrlen) \
    { assert(bind(sockfd, my_addr, addrlen) >= 0); }
#define listen_or_die(s, backlog) \
    { assert(listen(s,  backlog) >= 0); }
#define accept_or_die(s, addr, addrlen) \
    ({ int rc = accept(s, addr, addrlen); assert(rc >= 0); rc; })
#define connect_or_die(sockfd, serv_addr, addrlen) \
    { assert(connect(sockfd, serv_addr, addrlen) >= 0); }
#define gethostbyname_or_die(name) \
    ({ struct hostent *p = gethostbyname(name); assert(p != NULL); p; })
#define gethostbyaddr_or_die(addr, len, type) \
    ({ struct hostent *p = gethostbyaddr(addr, len, type); assert(p != NULL); p; })

// client/server helper functions 
ssize_t readline(int fd, void *buf, size_t maxlen);
int open_client_fd(char *hostname, int portno);
int open_listen_fd(int portno);

// wrappers for above
#define readline_or_die(fd, buf, maxlen) \
    ({ ssize_t rc = readline(fd, buf, maxlen); assert(rc >= 0); rc; })
#define open_client_fd_or_die(hostname, port) \
    ({ int rc = open_client_fd(hostname, port); assert(rc >= 0); rc; })
#define open_listen_fd_or_die(port) \
    ({ int rc = open_listen_fd(port); assert(rc >= 0); rc; })

#endif // __IO_HELPER__
