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
    printf("I'm child pid: %d\n", getpid());
    // doing nothing?? wait child's child, and here is no child
  } else {
    int w =waitpid(rc, NULL, WNOHANG);
    // wait return child id
    printf("wait return code: %d\n", w);
    printf("I'm parent of %d (pid= %d)\n", rc, getpid());
  }

  
  return 0;
}
