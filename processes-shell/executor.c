#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "executor.h"
#include "utils.h"

bool exec_cd(Token *path) {
    if (path == NULL || path->val == NULL || path->tok_type != str_t) return false;
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
        path->paths[i]->tok_type = str_t;
        path->paths[i]->val = malloc(path->paths[i]->len);
        strncpy(path->paths[i]->val, new_path->paths[i]->val, path->paths[i]->len);
    }
}

void exec_exec(ExecNode *exec_node, PathNode *path) {
    for (int i = 0; i < path->n_paths; ++i) {
        char *p = malloc(path->paths[i]->len + 1 + exec_node->len_cmd + 1);
        if (p == NULL) error();
        p = strncpy(p, path->paths[i]->val, path->paths[i]->len);
        p[path->paths[i]->len] = '\0';
        strcat(p, "/");
        p[path->paths[i]->len + 1] = '\0';
        strncat(p, exec_node->cmd->val, exec_node->len_cmd); // TODO: check if null-terminated
        if (access(p, X_OK) != 0) continue;

        FILE *fout = stdout;
        char **cmd_args = malloc((2 + exec_node->n_args) * sizeof(char *));
        if (cmd_args == NULL) error();
        cmd_args[0] = tok_to_str(exec_node->cmd);
        for (int j = 1; j < 1 + exec_node->n_args; ++j) {
            cmd_args[j] = tok_to_str(exec_node->args[j - 1]);
        }
        cmd_args[1 + exec_node->n_args] = NULL;

        pid_t pid = fork();
        if (pid == 0) {  // child
            execv(p, cmd_args);
            return;
        } else if (pid > 0) {  // parent
            int wstatus;
            if (waitpid(pid, &wstatus, 0) == -1) exit(1);
            if (!WIFEXITED(wstatus)) exit(WEXITSTATUS(wstatus));
            return;
        } else error();
    }
    warn();
}

int execute(Node *node, PathNode *path) {
    if (!node) error();
    switch (node->node_type) {
        case exit_t:
            exit(0);
        case cd_t:
            if (!exec_cd(node->cd_node->path)) {
                warn();
            }
            break;
        case path_t:
            exec_path(node->path_node, path);
            break;
        case exec_t:
            exec_exec(node->exec_node, path);
            break;
        default:
            error();
    }
}
