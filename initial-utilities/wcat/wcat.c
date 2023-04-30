#include <stdio.h>

#define BUFFER_SIZE 64

int main(int argc, char *argv[])
{
    FILE *fp;

    char buffer[BUFFER_SIZE];
    for (int i = 1; i < argc; ++i)
    {
        // Open the file
        fp = fopen(argv[i], "r");
        if (fp == NULL)
        {
            printf("wcat: cannot open file\n");
            return 1;
        }

        // Read contents
        while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
        {
            printf("%s", buffer);
        }

        // Close the file
        fclose(fp);
    }

    return 0;
}