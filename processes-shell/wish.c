#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef struct StringList {
    int size;
    char **strs;
} StringList;


char sep[2] = " \n";
char error_message[30] = "An error has occurred\n";


void print_error() {
    fprintf(stderr, error_message, strlen(error_message));
}


StringList *parse_args(char *buffer) {
    StringList *args = malloc(sizeof(StringList));
    args->size = 0;
    args->strs = NULL;
    if (buffer == NULL || *buffer == '\0' || *buffer == '\n') return args;

    args->size = 1;
    args->strs = malloc(sizeof(char *));
    if (args->strs == NULL) print_error();

    int i = 0;
    while (buffer != NULL && *buffer != '\0') {
        char *arg = strsep(&buffer, sep);
        if (i >=  args->size) {
            args->size *= 2;
            args->strs = realloc(args->strs, args->size * sizeof(char *));
            if (args->strs == NULL) print_error();
        }
        args->strs[i] = malloc(strlen(arg));
        if (args->strs[i] == NULL) print_error();
        strcpy(args->strs[i], arg);
        ++i;
    }
    return args;
}


void run_exit(char *args) {
    if (args != NULL && *args != '\0') print_error();
    exit(0);
}


void run_cd(char *args) {
    char *path = strsep(&args, sep);
    if (path == NULL || (args != NULL && *args != '\0')) {
        print_error();
        return;
    }
    if (chdir(path) == -1) print_error();
}


StringList *run_path(char *buffer) {
    return parse_args(buffer);
}


void run_exec(char *cmd, char *args, int num_paths, StringList *paths) {
    for (int i = 0; i < num_paths; ++i) {
        char *path = malloc(strlen(paths->strs[i]) + 1 + strlen(cmd));
        if (path == NULL) print_error();
        path = strcpy(path, paths->strs[i]);
        strcat(path, "/");
        strcat(path, cmd);
        if (access(path, X_OK) != 0) continue;

        pid_t pid = fork();
        if (pid == 0) {  // child
            StringList *parsed_args = parse_args(args);
            char **cmd_args = malloc((2 + parsed_args->size) * sizeof(char *));

            if (cmd_args == NULL) print_error();
            cmd_args[0] = cmd;
            for (int i = 1; i < 1 + parsed_args->size; ++i) cmd_args[i] = parsed_args->strs[i - 1];
            cmd_args[1 + parsed_args->size] = NULL;
            execv(path, cmd_args);
            return;
        } else if (pid > 0) {  // parent
            int wstatus;
            if (waitpid(pid, &wstatus, 0) == -1) exit(1);
            if (!WIFEXITED(wstatus) || WEXITSTATUS(wstatus) != 0) exit(WEXITSTATUS(wstatus));
            return;
        } else print_error();
    }
    print_error();
}


int main(int argc, char *argv[]) {
    StringList *paths = run_path("/bin");

    bool interactive = true;
    FILE *fin;
    if (argc > 2) {
        print_error();
        return 1;
    } else if (argc == 2) {
        interactive = false;
        fin = fopen(argv[1], "r");
        if (fin == NULL) {
            print_error();
            return 1;
        }
    } else fin = stdin;

    char *line = NULL;
    size_t len;
    if (interactive) printf("wish> ");
    while (getline(&line, &len, fin) != -1) {
        // TODO:
        // * look up executables in path with access
        // * execute commands with fork, execv, wait/waitpid
        // * implement redirection, parallel commands, etc.
        char *cmd = strsep(&line, sep);
        if (strcmp(cmd, "exit") == 0) run_exit(line);
        else if (strcmp(cmd, "cd") == 0) run_cd(line);
        else if (strcmp(cmd, "path") == 0) paths = run_path(line);
        else run_exec(cmd, line, paths->size, paths);
        if (interactive) printf("wish> ");
    }
}
