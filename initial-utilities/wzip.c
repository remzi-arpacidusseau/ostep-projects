
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
    if(argc==1)
	{
		printf("wzip: file1 [file2 ...]\n");
		exit(1);
	}
    else
	{
		char str[4096];
            FILE *fp = fopen(argv[1], "r");
            while(fgets(str,4096 , fp) != NULL)
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
//                                printf("%d", counter);
                                fwrite(&counter, sizeof(int), 1, stdout);
                    			fwrite(&q, sizeof(char), 1, stdout);
                            }
				}
              fclose(fp);


    }
}







































