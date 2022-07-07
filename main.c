#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    
    printf("%p\n", main);
    printf("%p\n", malloc(100));
}