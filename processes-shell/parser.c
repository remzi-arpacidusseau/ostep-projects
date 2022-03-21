#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "utils.h"

ExitNode *parse_exit(size_t len, Token **toks) {
    ExitNode *exit_node = NULL;
    char *tok = malloc(len);
    if (len == 1 &&
        toks[0]->tok_type == str_t &&
        toks[0]->len == 4 &&
        !strncmp(toks[0]->val, "exit", toks[0]->len)
    ) {
        exit_node = malloc(sizeof(ExitNode));
        if (!exit_node) error();
        exit_node = &((ExitNode) {});
    }

    return exit_node;
}

CdNode *parse_cd(size_t len, Token **toks) {
    CdNode *cd_node = NULL;

    char *tok = malloc(len);
    if (len >= 2 &&
        toks[0]->tok_type == str_t &&
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

    if (len >= 2 &&
        toks[0]->tok_type == str_t &&
        toks[0]->len == 4 &&
        !strncmp(toks[0]->val, "path", toks[0]->len)
    ) {
        path_node = malloc(sizeof(PathNode));
        path_node->n_paths = 0;
        path_node->paths = malloc((len - 1) * sizeof(char *));

        while (1 + path_node->n_paths < len && toks[path_node->n_paths + 1]->tok_type == str_t) {
            (path_node->paths)[path_node->n_paths] = toks[path_node->n_paths + 1];
            ++(path_node->n_paths);
        }
    }

    return path_node;
}

ExecNode *parse_exec(size_t len, Token **toks) {
    ExecNode *exec_node = NULL;

    if (len >= 1 &&
        toks[0]->tok_type == str_t &&
        toks[0]->len > 0
    ) {
        exec_node = malloc(sizeof(ExecNode));
        exec_node->len_cmd = toks[0]->len;
        exec_node->cmd = toks[0];
        exec_node->n_args = 0;
        exec_node->args = malloc((len - 1) * sizeof(char *));

        while (1 + exec_node->n_args < len && toks[exec_node->n_args + 1]->tok_type == str_t) {
            (exec_node->args)[exec_node->n_args] = toks[exec_node->n_args + 1];
            ++(exec_node->n_args);
        }
    }

    return exec_node;
}

Node *parse(size_t len, Token **toks) {
    Node *node = malloc(sizeof(Node));
    if (!node) error();

    ExitNode *exit_node;
    CdNode *cd_node;
    PathNode *path_node;
    if (exit_node = parse_exit(len, toks)) {
        node->node_type = exit_t;
        node->exit_node = exit_node;
        return node;
    } else if (cd_node = parse_cd(len, toks)) {
        node->node_type = cd_t;
        node->cd_node = cd_node;
    } else if (path_node = parse_path(len, toks)) {
        node->node_type = path_t;
        node->path_node = path_node;
    } else {
        free(node);
        return NULL;
    }
}
