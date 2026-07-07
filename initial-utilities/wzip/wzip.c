#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    exit(1);
  }

  int count = 0;
  int current = 0;
  int have_current = 0;

  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "rb");
    if (fp == NULL) {
      printf("wzip: cannot open file\n");
      exit(1);
    }

    int c;
    while ((c = fgetc(fp)) != EOF) {
      if (!have_current) {
        current = c;
        count = 1;
        have_current = 1;
      } else if (c == current) {
        count++;
      } else {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&current, sizeof(char), 1, stdout);
        current = c;
        count = 1;
      }
    }

    fclose(fp);
  }

  if (have_current) {
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&current, sizeof(char), 1, stdout);
  }

  return 0;
}
