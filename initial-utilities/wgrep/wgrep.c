#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE (512)
void matchLine(char *pattern, char *line);

int main(int argc, char *argv[]) {
    if (argc<2) {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }

    if (argc==2) {
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, stdin)!=NULL) {
            if (strstr(buffer, argv[1])!=NULL) {
                printf("%s", buffer);
            }
        }

        return 0;
    }

    for (size_t i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp==NULL) {
            printf("wgrep: cannot open file\n");
            exit(1);
        }

        // search by line
        char *line = NULL;
        size_t linecap = BUFFER_SIZE;
        ssize_t linelen;
        while ((linelen = getline(&line, &linecap, fp)) > 0) {
            if (strstr(line, argv[1])!=NULL) {
                printf("%s", line);
            }
        }

        fclose(fp);
        
    }

    return 0;
}

void _strstr(char *pattern, char *line) {
    int flag = 0;
    size_t _strlen = strlen(pattern);
    size_t linelen = strlen(line);
    if (_strlen>linelen) return;
    // printf("%zu %zu %zu\n", _strlen, linelen, linelen-_strlen);
    for (size_t j = 0; j <= linelen-_strlen; j++) {
        if (pattern[0]==line[j]) {
            // if word is found, stop searching
            if (flag==1) {
                break;
            }

            // match pattern
            for (size_t k = 0; k < _strlen; k++) {
                if (pattern[k]==line[k+j]) {
                    if (k==_strlen-1) {
                        flag=1;
                        printf("%s", line);
                        break;
                    }
                } else {
                    break;
                }
            }
        }

        if (flag==1) {
            break;
        }
    }

    return;
}
