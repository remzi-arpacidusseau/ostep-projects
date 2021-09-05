#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    exit(0);
  }
  int i = 1;
  FILE *fp = NULL;
  while (i < argc)
  {
    if ((fp = fopen(argv[i], "r")) == NULL)
    {
      printf("wcat: cannot open file\n");
      exit(1);
    }
    char buffer[1000];
    while (fgets(buffer, 1000, fp) != NULL)
    {
      printf("%s", buffer);
    }
    i++;
  }
  fclose(fp);
  return 0;
}