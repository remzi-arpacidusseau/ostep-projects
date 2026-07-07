#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("wunzip: file1 [file2 ...]\n");
    exit(1);
  }

  for (size_t i = 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "rb");
    if (fp == NULL) {
      printf("wunzip: cannot open file\n");
      exit(1);
    }

    int count;
    char ch;

    while (fread(&count, sizeof(int), 1, fp) == 1) {
      if (fread(&ch, sizeof(char), 1, fp) != 1) {
        break;
      }

      for (int j = 0; j < count; j++) {
        printf("%c", ch);
      }
    }

    fclose(fp);
  }
  return 0;
}
