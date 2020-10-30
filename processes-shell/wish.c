#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
int main(int argc, char* argv[])
{	
	while(1)
	{	
		char A[50];
		char* myargv[10];
                char* tok;
                int i = 0;
		char* str;
		int redirection = 0;
		int parallel = 0;
		for(i = 0; i<10; i++)                
		{
                  myargv[i] = NULL;                
		}                 

		printf("wish> ");
		fgets(A, 50, stdin);

		if(strcmp(A, "exit\n")!=0)
		{
			str = strdup(A);
			i = 0;
			while ((tok = strsep(&str, " ")) != NULL)
			 {     
				myargv[i] = strdup(tok); 
//				printf("%d = %s", i, myargv[i]);
				myargv[i][strcspn(myargv[i], "\n")] = 0;
				i++;
                     	}
			int rc = fork();
			if(rc == 0)
			{
				for(i = 0; myargv[i] != NULL; i++)
				{
					if(strcmp(myargv[i], ">")==0)
					{
						redirection = 1;
					}
					if(strcmp(myargv[i], "&")==0)
                                        {   
                                              parallel = 1;                                         
					}
				}
				
				if(strcmp(myargv[0], "ls")==0)
				{
					printf("In ls");
					myargv[0] = strdup("/bin/ls");
                                        execv(myargv[0], myargv);
				}
				else if(strcmp(myargv[0], "cd")==0)
				{
					printf("%s\n", getcwd(str, 100));      
					chdir(myargv[1]);  
					printf("%s\n", getcwd(str, 100));
				}
				else if ( (strcmp(myargv[0], "cat")==0) && (redirection == 1) )
				{
					printf("redirection 1");
					close(STDOUT_FILENO);
					open("./p4.out", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU); 
					myargv[2] = NULL; 
					execvp(myargv[0], myargv);
				}
				else if ((strcmp(myargv[0], "hello")==0) && ( redirection == 1)) 
                                {
                                        close(STDOUT_FILENO);   
                                        open("./p4.out", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
					myargv[0] = strdup("./hello");
					myargv[1] = NULL;
                                        execvp(myargv[0], myargv);  

				}
				 if(parallel == 1)
				{
					
					int rc2 = fork();
					printf("In parallel");
					//myargv[0] = strdup("./hello");
					myargv[1] = NULL;
					
					
					if(rc2 == 0)
					{
						printf("child");
						execv(myargv[0], myargv);
					}
					else
					{

						rc2 = (int) wait(NULL);
						printf("parent");
						execv(myargv[0], myargv);	
					}
				}
				else  
				{
                                      execv(myargv[0], myargv);                                 
				}

				exit(0);	
			}			
			else
			{
				rc = (int) wait(NULL);
			}
		
		}
		else if(strcmp(A, "exit\n")==0)
		{
			break; 
			exit(0);
			//exit(0);
		}
	}
		
exit(0);
	
}

