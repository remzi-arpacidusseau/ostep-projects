#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char sep[2] = " \n";
char error_message[30] = "An error has occurred\n";


void print_error_and_exit() {
    fprintf(stderr, error_message, strlen(error_message));
    exit(1);
}


void run_exit(char *args) {
    if (args != NULL && *args != '\0') print_error_and_exit();
    exit(0);
}


void run_cd(char *args) {
    char *path = strsep(&args, sep);
    if (path == NULL || (args != NULL && *args != '\0')) print_error_and_exit();
    if (chdir(path) == -1) print_error_and_exit();
}


int main(int argc, char *argv[]) {
    bool interactive = true;
    FILE *fin;
    if (argc > 2) {
        fprintf(stderr, "too many arguments\n");
        return 1;
    } else if (argc == 2) {
        interactive = false;
        fin = fopen(argv[1], "r");
        if (fin == NULL) {
            fprintf(stderr, "file not found\n");
            return 1;
        }
    } else fin = stdin;

    // TODO: initialize PATH=/bin

    char *line = NULL;
    size_t len;
    int num_lines = -1;
    do {
        if (interactive) printf("wish> ");
        num_lines = getline(&line, &len, fin);
        // TODO:
        // * execute built-in commands: cd (1 arg), path (any args, overwrites path)
        // * look up executables in path with access
        // * execute commands with fork, execv, wait/waitpid
        // * implement redirection, parallel commands, etc.
        char *command = strsep(&line, sep);
        if (strcmp(command, "exit") == 0) run_exit(line);
        else if (strcmp(command, "cd") == 0) run_cd(line);
        else print_error_and_exit();
    } while (num_lines != -1);
}
