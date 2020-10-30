#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE (1024)

int main(int argc, char *argv[])
{
	if(argc!=1)
	{
		for(int i=1; i<argc; i++)
        	{
                	FILE *fp = fopen(argv[i], "r");
                	if(fp == NULL)
                	{
                    		printf("wcat: cannot open file\n");
	                    	exit(1);
        	        }
                	char buffer[1024];
        	        while(fgets(buffer, 1024, fp) != NULL)
	                {
                    	printf("%s", buffer);
                	}
                	fclose(fp);
        	}
	}
	exit(0);
}

































