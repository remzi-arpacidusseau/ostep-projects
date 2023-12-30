#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    char buffer[BUFFER_SIZE];

    // file name check
    for (int i=1; i<argc; ++i) {
        if (access(argv[i], F_OK) == -1) {
            printf("%s does not exist in the current directory.\n", argv[i]);
        }
    }

    // print wcat
    for (int i=1; i<argc; ++i) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            printf("invalid file name : %s\n", argv[i]);
            exit(1);
        }

        printf("\n--------------- %s ---------------\n", argv[i]);
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            printf("%s", buffer);
        }
    }


}