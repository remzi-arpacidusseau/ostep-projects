#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"
#include "utils.h"

bool exec_cd(Token *path) {
    if (path == NULL || path->val == NULL || path->tok_type != ident_tok) return false;
    char *p = malloc(sizeof(1 + path->len));
    strncpy(p, path->val, path->len);
    p[path->len] = '\0';
    if (chdir(p) == -1) return false;
    return true;
}

void exec_path(PathNode *new_path, PathNode *path) {
    for (int i = 0; i < path->n_paths; ++i) {
        free(path->paths[i]->val);
        // free(path->paths[i]);    // TODO: fix this
    }

    path->n_paths = new_path->n_paths;
    path->paths = malloc(path->n_paths * sizeof(Token *));
    for (int i = 0; i < path->n_paths; ++i) {
        path->paths[i] = malloc(sizeof(Token));
        path->paths[i]->len = new_path->paths[i]->len;
        path->paths[i]->tok_type = ident_tok;
        path->paths[i]->val = malloc(path->paths[i]->len);
        strncpy(path->paths[i]->val, new_path->paths[i]->val, path->paths[i]->len);
    }
}

void exec_exec(ExecNode *exec_node, PathNode *path) {
    for (int i = 0; i < path->n_paths; ++i) {
        // try to find executable in current path
        char *p = malloc(path->paths[i]->len + 1 + exec_node->len_cmd + 1);
        if (p == NULL) error();
        p = strncpy(p, path->paths[i]->val, path->paths[i]->len);
        p[path->paths[i]->len] = '\0';
        strcat(p, "/");
        p[path->paths[i]->len + 1] = '\0';
        strncat(p, exec_node->cmd->val, exec_node->len_cmd); // TODO: check if null-terminated
        if (access(p, X_OK) != 0) continue;

        // prepare command and arguments for execv
        char **cmd_args = malloc((2 + exec_node->n_args) * sizeof(char *));
        if (cmd_args == NULL) error();
        cmd_args[0] = tok_to_str(exec_node->cmd);
        for (int j = 1; j < 1 + exec_node->n_args; ++j) {
            cmd_args[j] = tok_to_str(exec_node->args[j - 1]);
        }
        cmd_args[1 + exec_node->n_args] = NULL;

        // set up output for redirection
        FILE *fout;
        if (exec_node->out == NULL) {
            fout = stdout;
        } else {
            char *file = tok_to_str(exec_node->out);
            fout = fopen(file, "w");
            if (fout == NULL) error();
            free(file);
        }

        int pipefd[2];
        if (pipe(pipefd) == -1) error();
        pid_t pid = fork();
        if (pid == 0) {  // child
            // write to pipe
            if (dup2(pipefd[1], STDOUT_FILENO) == -1) error();
            if (close(pipefd[0]) == -1 || close(pipefd[1]) == -1) error();

            // execute command
            execv(p, cmd_args);
            return;
        } else if (pid > 0) {  // parent
            // read from pipe
            if (close(pipefd[1]) == -1) error();
            char *line = NULL;
            size_t len;
            FILE *fin = fdopen(pipefd[0], "r");
            if (fin == NULL) error();
            while (getline(&line, &len, fin) > 0) {
                fprintf(fout, "%s", line);
            }
            if (fclose(fin)) error();
            if (fout != stdout && fclose(fout)) error();

            // wait for child process to complete
            int wstatus;
            if (waitpid(pid, &wstatus, 0) == -1) exit(1);
            if (!WIFEXITED(wstatus)) exit(WEXITSTATUS(wstatus));
            return;
        } else {  // fork failed
            error();
        }
    }
    // executable not found in path
    warn();
}

void exec_command(CommandNode *cmd_node, PathNode *path) {
    if (!cmd_node) {
        warn();
        return;
    }

    switch (cmd_node->node_type) {
        case empty_t:
            break;
        case exit_t:
            exit(0);
        case cd_t:
            if (!exec_cd(cmd_node->cd_node->path)) {
                warn();
            }
            break;
        case path_t:
            exec_path(cmd_node->path_node, path);
            break;
        case exec_t:
            exec_exec(cmd_node->exec_node, path);
            break;
        default:
            error();
    }
}


void exec_parallel(ParallelNode *parallel_node, PathNode *path) {
    if (parallel_node->right) {
        pid_t pid = fork();
        if (!pid) {  // child
            exec_command(parallel_node->left, path);
            exit(0);
        } else {  // parent
            exec_parallel(parallel_node->right, path);

            int wstatus;
            if (waitpid(pid, &wstatus, 0) == -1) exit(1);
            // if (!WIFEXITED(wstatus)) exit(WEXITSTATUS(wstatus));
        }
    } else {
        exec_command(parallel_node->left, path);
    }
}
