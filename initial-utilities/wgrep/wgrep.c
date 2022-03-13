#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


bool starts_with(char *buffer, char *term) {
    if (*term == '\0') return true;
    if (*buffer == '\0') return false;
    for (; *buffer != '\0'; ++buffer) {
        if (*buffer == *term) return starts_with(buffer + 1, term + 1);
        else return false;
    }
    return false;
}


bool contains(char *buffer, char *term) {
    for (; *buffer != '\0'; ++buffer) {
        if (starts_with(buffer, term)) return true;
    }
    return false;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }
    char *term = argv[1];

    FILE *f;
    char *line;
    size_t len;
    if (argc < 3) {
        f = stdin;
        while (getline(&line, &len, f) != -1) {
            if (contains(line, term))
                printf("%s", line);
        }
        return 0;
    }

    for (int i = 2; i < argc; ++i) {
        char *file = argv[i];
        f = fopen(file, "r");
        if (f == NULL) {
            printf("wgrep: cannot open file\n");
            return 1;
        }

        while (getline(&line, &len, f) != -1) {
            if (contains(line, term))
                printf("%s", line);
        }
        fclose(f);
    }
}
