#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char *argv[]) {
  printf("pid: %d\n", (int) getpid());
  char filename[10] = "./hw2.txt";
  int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
  int rc = fork();
  int looptimes=1000;
  if (rc<0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc==0) {
    printf("fd= %d\n", fd);
    for (int i=0; i<looptimes; ++i) {
      char buf1[20] = "hw2 by child\n";
      write(fd, buf1, strlen(buf1));
    }
    printf("I'm child pid: %d\n", getpid());
  } else {
    for (int i=0; i<looptimes; ++i) {
      char buf2[20] = "hw2 by parent\n";
      write(fd, buf2, strlen(buf2));
    }
    wait(NULL);
    printf("fd= %d\n", fd);
    close(fd);
    printf("I'm parent of %d (pid= %d)\n", rc, getpid());
  }

  return 0;
}
