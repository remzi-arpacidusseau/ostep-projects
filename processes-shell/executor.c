#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "executor.h"
#include "utils.h"

bool exec_cd(Token *path) {
    if (path == NULL || path->val == NULL || path->tok_type != str_t) return false;
    char *p = malloc(sizeof(path->len));
    strncpy(p, path->val, path->len);
    if (chdir(p) == -1) return false;
    return true;
}

int execute(Node *node) {
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
            for (int i = 0; i < node->path_node->n_paths; ++i) {
                print_token((node->path_node->paths)[i], true);
            }
            break;
        default:
            error();
    }
}
