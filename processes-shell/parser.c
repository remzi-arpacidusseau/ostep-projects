#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"

bool parse_exit(size_t len, Token **toks) {
    return len == 1 && toks[0]->tok_type == exit_tok;
}

CdNode *parse_cd(size_t len, Token **toks) {
    CdNode *cd_node = NULL;

    char *tok = malloc(len);
    if (len >= 2 &&
        toks[0]->tok_type == ident_tok &&
        toks[0]->len == 2 &&
        !strncmp(toks[0]->val, "cd", toks[0]->len)
    ) {
        cd_node = malloc(sizeof(CdNode));
        cd_node->path = toks[1];
    }

    return cd_node;
}

PathNode *parse_path(size_t len, Token **toks) {
    PathNode *path_node = NULL;

    if (len >= 1 &&
        toks[0]->tok_type == ident_tok &&
        toks[0]->len == 4 &&
        !strncmp(toks[0]->val, "path", toks[0]->len)
    ) {
        path_node = malloc(sizeof(PathNode));
        path_node->n_paths = 0;
        path_node->paths = malloc((len - 1) * sizeof(char *));

        while (1 + path_node->n_paths < len && toks[path_node->n_paths + 1]->tok_type == ident_tok) {
            (path_node->paths)[path_node->n_paths] = toks[path_node->n_paths + 1];
            ++(path_node->n_paths);
        }
    }

    return path_node;
}

ExecNode *parse_exec(size_t len, Token **toks) {
    ExecNode *exec_node = NULL;

    if (len >= 1 &&
        toks[0]->tok_type == ident_tok &&
        toks[0]->len > 0
    ) {
        // initialize command
        exec_node = malloc(sizeof(ExecNode));
        exec_node->len_cmd = toks[0]->len;
        exec_node->cmd = toks[0];
        exec_node->n_args = 0;
        exec_node->args = malloc((len - 1) * sizeof(Token *));

        // parse arguments
        while (1 + exec_node->n_args < len && toks[exec_node->n_args + 1]->tok_type == ident_tok) {
            (exec_node->args)[exec_node->n_args] = toks[exec_node->n_args + 1];
            ++(exec_node->n_args);
        }

        // parse output redirection
        if (exec_node->n_args == len - 3) {
            if (
                toks[exec_node->n_args + 1]->tok_type == rangle_tok &&
                toks[exec_node->n_args + 2]->tok_type == ident_tok
            ) {
                exec_node->out = toks[exec_node->n_args + 2];
            } else {
                // free(exec_node);  // TODO: free recursively
                return NULL;
            }
        } else if (exec_node->n_args != len - 1) {
            // free(exec_node);
            return NULL;
        }
    }

    return exec_node;
}

Node *parse(size_t len, Token **toks) {
    Node *node = malloc(sizeof(Node));
    if (!node) error();

    CdNode *cd_node;
    PathNode *path_node;
    ExecNode *exec_node;
    if (len == 0) {
        node->node_type = empty_t;
    } else if (parse_exit(len, toks)) {
        node->node_type = exit_t;
        return node;
    } else if (cd_node = parse_cd(len, toks)) {
        node->node_type = cd_t;
        node->cd_node = cd_node;
    } else if (path_node = parse_path(len, toks)) {
        node->node_type = path_t;
        node->path_node = path_node;
    } else if (exec_node = parse_exec(len, toks)) {
        node->node_type = exec_t;
        node->exec_node = exec_node;
    } else {
        free(node);
        return NULL;
    }
}
