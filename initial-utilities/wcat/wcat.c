
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  char buf[4096];

  for (int i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (fp == NULL) {
      printf("wcat: cannot open file\n");
      exit(1);
    }

    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, sizeof(buf), fp)) > 0) {
      size_t bytes_write = fwrite(buf, 1, bytes_read, stdout);

      if (bytes_write != bytes_read) {
        fclose(fp);
        exit(1);
      }
    }

    if (ferror(fp)) {
      fclose(fp);
      exit(1);
    }

    fclose(fp);
  }

  return 0;
}
