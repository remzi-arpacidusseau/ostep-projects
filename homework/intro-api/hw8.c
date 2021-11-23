#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]) {
  printf("pid: %d\n", (int) getpid());
  int fd[2];
  if (pipe(fd)==-1) { // open a pipe
    printf("err\n");
    exit(1);
  }
  int p1 = fork();
  if (p1<0) {
    fprintf(stderr, "fork failed\n");
    exit(1);
  } else if (p1==0) {
    printf("child 1 start\n");
    char msg[20] = "msg from child 1\n";
    write(fd[1], msg, strlen(msg));
    printf("I'm child1 pid: %d\n", getpid());
    exit(0);
  }
  int p2 = fork();
  if (p2<0) {
    fprintf(stderr, "fork 2 failed\n");
    exit(1);
  } else if (p2==0) {
    waitpid(p1, NULL, 0);
    printf("child 2 start\n");
    int r=0;
    char buf[100];
    if ((r=read(fd[0], buf, 100))>=0) {
      buf[r]='\0';
      printf("this is a message from pipe: \n %s\n", buf);
    } else {
      printf("no pipe msg\n");
    }
    printf("im child 2\n");
    exit(0);
  }
  waitpid(p1, NULL, 0);
  waitpid(p2, NULL, 0);
  close(fd[0]);
  close(fd[1]);
  printf("I'm parent of p1:%d p2:%d(pi1= %d)\n", p1, p2, getpid());

  return 0;
}
