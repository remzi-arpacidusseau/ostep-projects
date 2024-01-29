#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
    for(int i =1; i < argc; i++){
        FILE *fp = NULL;
        fp = fopen(argv[i], "r");
        if(NULL == fp){
            printf("wcat: cannot open file\n");
            exit(1);
        }
        const size_t buff_size = 128;
        char buffer[buff_size];
        while(NULL != fgets(buffer, buff_size, fp)){
            printf("%s", buffer);
        }
        if(0 !=fclose(fp)){
            exit(1);
        }

    }
    
    return 0;
}
