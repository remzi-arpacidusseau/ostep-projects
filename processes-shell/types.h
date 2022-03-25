#ifndef WISH_TYPES_H
#define WISH_TYPES_H

#include <sys/types.h>

enum NodeType { empty_t, exit_t, cd_t, path_t, exec_t };

enum TokType { rangle_tok, ampand_tok, exit_tok, cd_tok, path_tok, ident_tok };

typedef struct Token {
    int tok_type;
    size_t len;
    char *val;
} Token;

typedef struct CdNode {
    Token *path;
} CdNode;

typedef struct PathNode {
    int n_paths;
    Token **paths;
} PathNode;

typedef struct ExecNode {
    int len_cmd;
    Token *cmd;
    int n_args;
    Token **args;
    Token *out;
} ExecNode;

typedef struct CommandNode {
    int node_type;
    union {
        CdNode *cd_node;
        PathNode *path_node;
        ExecNode *exec_node;
    };
} CommandNode;

typedef struct ParallelNode ParallelNode;
typedef struct ParallelNode {
    CommandNode *left;
    ParallelNode *right;
} ParallelNode;

#endif
