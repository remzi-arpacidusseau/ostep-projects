#include <stdio.h>

#define READ_SIZE sizeof(int) + sizeof(char)


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        char *filename = argv[i];
        FILE *f = fopen(filename, "r");

        char buffer[READ_SIZE];
        while (fread(buffer, READ_SIZE, 1, f) == 1) {
            int count = *((int *) buffer);
            char c = *((char *) (buffer + sizeof(int)));
            for (int i = 0; i < count; ++i) printf("%c", c);
        }

        fclose(f);
    }
}
