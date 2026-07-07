
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wgrep: searchterm [file ...]\n");
    exit(1);
  }

  char *search_term = argv[1];
  char *buffer = NULL;
  size_t size = 0;
  ssize_t nread;
  FILE *fp;

  if (argc == 2) {
    fp = stdin;
  } else {
    fp = fopen(argv[2], "r");
    if (fp == NULL) {
      printf("wgrep: cannot open file\n");
      exit(1);
    }
  }

  while ((nread = getline(&buffer, &size, fp)) > -1) {
    char *result = strstr(buffer, search_term);

    if (result != NULL) {
      fwrite(buffer, 1, nread, stdout);
    }
  }

  fclose(fp);
  return 0;
}
