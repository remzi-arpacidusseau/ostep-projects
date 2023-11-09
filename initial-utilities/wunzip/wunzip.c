#include <stdio.h>



int main() {

    FILE *f = fopen("file.txt", "r");
    char line[100];
    fread(line, sizeof(int), 1, f);
    printf("%d\n", *(line));
}