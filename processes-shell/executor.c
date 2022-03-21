#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void exec_path(PathNode *new_path, PathNode *old_path) {
    old_path->n_paths = new_path->n_paths;
    old_path->paths = new_path->paths;
}

void exec_exec(ExecNode *exec_node, PathNode *path) {
    print_token(exec_node->cmd, true);
    for (int i = 0; i < exec_node->n_args; ++i) {
        print_token(exec_node->args[i], true);
    }
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
