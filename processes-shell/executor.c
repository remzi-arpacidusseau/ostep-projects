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

bool exec_path(PathNode *new_path, PathNode *old_path) {
    for (int i = 0; i < new_path->n_paths; ++i) {
        print_token((new_path->paths)[i], true);
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
            if (!exec_path(node->path_node, path)) {
                warn();
            }
            break;
        default:
            error();
    }
}
