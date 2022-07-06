#include <stdio.h>
#include <string.h>


int main(int argc, char *argv[]) {

    char c;
    char prev;
    char filename[100];
    strcpy(filename, argv[1]);
    FILE *f = fopen(filename, "r");
    int i = 1;
    while ((c = fgetc(f)) != EOF) {  
        if (c == prev) i++;
        if (c != prev) {
            fwrite(&i, sizeof(int), 1, stdout);
            putchar(c);
            i = 1;
        }
        prev = c;
    }
    
    
}