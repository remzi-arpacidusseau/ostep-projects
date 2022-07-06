#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc == 1) return 0;
  
    while (--argc) {
        char filename[100];
        strcpy(filename, *++argv);
        //printf("%s", filename);

        char buffer[1000];
        FILE *f = fopen(filename, "r");
        if (f == NULL) {
            printf("wcat: cannot open file\n");
            return 1;
        }

        while ((fgets(buffer, 1000, f)) != NULL) {
            printf("%s", buffer);
        }
    }

    return 0;
}