#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE (512)

int main(int argc, char *argv[]) {
    if (argc==1) {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (size_t i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp==NULL) {
            printf("wunzip: cannot open file\n");
            exit(1);
        }
        int count=0;
        char c='\0';
        while (fread(&count, sizeof(int), 1, fp) > 0) {
            fread(&c, sizeof(char), 1, fp);
            for (size_t j = 0; j < count; j++) {
                printf("%c", c);
            }
        }
    }

    return 0;
}
