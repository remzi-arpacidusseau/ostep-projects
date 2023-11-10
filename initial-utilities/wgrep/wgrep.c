#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE (1024)

int main(int argc, char *argv[])
{
	if(argc==1)
	{
		printf("wgrep: searchterm [file ...]\n");
		exit(1);
	}
	else if (argc == 2)
	{
		char input[4096];
		while(fgets(input,4096, stdin) != NULL);
		{
			if(strstr(input, argv[1]) != NULL)
			{
				printf("%s", input);
			}
		}
	}
	else if (argc == 3)
	{
		FILE *fp = fopen(argv[2], "r");
		if(fp == NULL)
		{
			printf("wgrep: cannot open file\n");
			exit(1);
		}
		char buffer[1024];
		while(fgets(buffer, 1024, fp) != NULL)
		{
			if(strstr(buffer, argv[1])!= NULL)
			{
			printf("%s", buffer);
			}
		}
		fclose(fp);
	}
	
	exit(0);
}

































