
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    if(argc==1)
	{
		printf("wzip: file1 [file2 ...]\n");
		exit(1);
	}
    else if(argc == 2)
	{
		char str[8192];
                FILE *fp = fopen(argv[1], "r");
            	while(fgets(str,8192 , fp) != NULL)
		{
			int l = strlen(str);
        	      	for(int i=0; i<l; i++)
                        {
                              	char q = str[i];
                                int counter = 1;
                                while (q == str[i + 1])
                                {
                        		counter++;
                       	    		i++;
                      		}
                                fwrite(&counter, sizeof(int), 1, stdout);
                                fwrite(&q, sizeof(char), 1, stdout);
                         }
		}
                fclose(fp);
       }
     else
	{
		char str[4096];
		char q = '1';
		int counter = 1;
                for(int j=1; j<argc; j++)
                {
                    counter++;
                    FILE *fp = fopen(argv[j], "r");
                    while(fgets(str,4096 , fp) != NULL)
                    {
                        int l = strlen(str);
                        for(int i=0; i<l; i++)
                        {
                         	if(q != str[i] )
                                {
                                	counter = 1;
                                }
                                q = str[i];
	                        while (q == str[i + 1])
                                { 
                                      counter++;
                                      i++;
                                }
                                if(j ==(argc-1))
                                {
                                 	fwrite(&counter, sizeof(int), 1, stdout);
                                        fwrite(&q, sizeof(char), 1, stdout);
                                }
                          }
                     }
                     fclose(fp);
                }
	}
}







































