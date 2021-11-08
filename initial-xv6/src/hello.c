#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{ int i=1;
  printf(1, "%d \n", getreadcount());
  printf(1, "%d \n", i);
  char buf[100];
  (void) read(4, buf, 1);
  printf(1, "%d \n", getreadcount());
  i++;
  printf(1, "%d \n", i);
  exit();
}
