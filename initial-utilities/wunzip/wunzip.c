#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    if(argc==1)
	{
		printf("wunzip: file1 [file2 ...]\n");
		exit(1);
	}
    else 
	{

		int a;
		char b;
		for(int j = 1; j<argc; j++)
		{
			FILE *fp = fopen(argv[j], "r");
			while (!feof(fp))
			{
				int check = fread(&a, 4,1, fp);
				if(check == 0)
				{break;}
				check = fread(&b, 1, 1,fp);
				if(check == 0)
	                         {break;}
				for(int i=0; i<a; i++ )
				printf("%c", b);			
			}
			fclose(fp);
		}
	}
	exit(0);
}

































