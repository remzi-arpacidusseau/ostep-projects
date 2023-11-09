#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {


    char s[10000];
    strcpy(s, argv[1]);
    void wgrep(char *argv, char *s);

    if (argc == 2) {
        char stream[10000];
        
        fgets(stream, 10000, stdin);
        wgrep(stream, s);
    }

    argv++;
    while (argc-- != 2) {
        wgrep(*++argv, s);
    }
    
    return 0;
}

void wgrep(char *argv, char *s) {
    FILE *f = fopen(argv, "r");
    if (f == NULL) {
        printf("wgrep: cannot open file\n");
        exit(1);
    }
    size_t len = 10000;
    char *line = NULL;

    char *p;
    char *sp = s;
    while (getline(&line, &len, f) != EOF) {
        char *lineptr = line;
        
        while ((p = lineptr++) && *p != '\n') {
            int i = 0;
            for (; *p++ == *sp++; ++i);
            if (i == strlen(s)) printf("%s", line);
            sp = s;
        }   
    }
}