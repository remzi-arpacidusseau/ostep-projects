#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  printf("pid: %d\n", (int) getpid());
  int rc = fork();
  if (rc<0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (rc==0) {
    printf("hello");
    printf("I'm child pid: %d\n", getpid());
  } else {
    wait(NULL);
    printf("goodbye\n");
    printf("I'm parent of %d (pid= %d)\n", rc, getpid());
  }

  
  return 0;
}
