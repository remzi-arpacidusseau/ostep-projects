#include <stdio.h>


void emit_encoded(int count, char c) {
    fwrite((void *) &count, sizeof(int), 1, stdout);
    fwrite((void *) &c, sizeof(char), 1, stdout);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    int count = 0;
    int prev = 0;
    int ord;
    for (int i = 1; i < argc; ++i) {
        char *filename = argv[i];
        FILE *f = fopen(filename, "r");
        if (f == NULL) {
            printf("error: could not open file\n");
            return 1;
        }

        while ((ord = fgetc(f)) != EOF) {
            if (prev == 0 || ord == prev) ++count;
            else {
                emit_encoded(count, (char) prev);
                count = 1;
            }
            prev = ord;
        }

        fclose(f);
    }
    emit_encoded(count, (char) prev);
}
