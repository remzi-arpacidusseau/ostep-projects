#include <stdio.h>

#define BUFFER_SIZE 80


int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        char *file = argv[i];
        FILE *f = fopen(file, "r");
        if (f == NULL) {
            printf("wcat: cannot open file\n");
            return 1;
        }
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, f) != NULL) {
            printf("%s", buffer);
        }
        fclose(f);
    }

    return 0;
}
