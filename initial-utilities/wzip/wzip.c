#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE (512)

int main(int argc, char* argv[]) {
    if (argc==1) {
        printf("wzip: file1 [file2 ...]\n");
        exit(1);
    }

    // count same
    int count=0;
    // first char
    char c='\0';
    for (size_t i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp==NULL) {
            printf("wzip: cannot open file\n");
            exit(1);
        }

        char *line = NULL;
        size_t linecap = BUFFER_SIZE;
        ssize_t linelen;
        while ((linelen = getline(&line, &linecap, fp)) > 0) {
            for (size_t j = 0; j < linelen; j++) {
                // printf("%d %c\n", count, c);
                if (c==line[j]) {
                    count++;
                } else {
                    if (c!='\0') {
                        fwrite(&count, sizeof(int), 1, stdout);
                        fwrite(&c, sizeof(char), 1, stdout);
                    }
                    c=line[j];
                    count=1;
                }
            }
        }
        fclose(fp);
    }
    fwrite(&count, sizeof(int), 1, stdout);
    fwrite(&c, sizeof(char), 1, stdout);
    
    return 0;
}
