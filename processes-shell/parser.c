#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LEN 4096

enum NodeType { exit_t, cd_t };

typedef struct Token {
    size_t len;
    char *val;
} Token;

void print_token(Token *tok, bool newline) {
    for (int j = 0; j < tok->len; ++j) {
        putchar(tok->val[j]);
    }
    if (newline) {
        putchar('\n');
    }
}

typedef struct ExitNode {} ExitNode;

typedef struct CdNode {
    Token *path;
} CdNode;

typedef struct Node {
    int node_type;
    union {
        ExitNode *exit_node;
        CdNode *cd_node;
    };
} Node;

void warn() {
    printf("error\n");
}

void error() {
    printf("error\n");
    exit(1);
}

void consume_whitespace(char **line) {
    size_t pos = strspn(*line, " \n");
    *line += pos;
}

size_t get_whitespace(char *line) {
    return strspn(line, " \n");
}

size_t get_token(char *line, char *seps, char *tok) {
    size_t pos = get_whitespace(line);
    line += pos;
    pos += strcspn(line, seps);
    strncpy(tok, line, pos);
    return pos;
}

bool parse_end(char *line) {
    size_t pos = get_whitespace(line);
    return *(line + pos) == '\0';
}

int parse_args(size_t len, char *line, char **args) {
    if (!line) return -1;

    int count = 0;
    char *tok;
    while (!strcmp((tok = strsep(&line, " \n")), "") != 0) {
        args[count] = tok;
        ++count;
    }
    return count;
}

ExitNode *parse_exit(size_t len, Token **toks) {
    ExitNode *exit_node = NULL;
    char *tok = malloc(len);
    if (len == 1 && !strncmp(toks[0]->val, "exit", toks[0]->len)) {
        exit_node = malloc(sizeof(ExitNode));
        if (!exit_node) error();
        exit_node = &((ExitNode) {});
    }

    return exit_node;
}

CdNode *parse_cd(size_t len, Token **toks) {
    CdNode *cd_node = NULL;

    char *tok = malloc(len);
    if (len >= 2 && !strncmp(toks[0]->val, "cd", toks[0]->len)) {
        cd_node = malloc(sizeof(CdNode));
        cd_node->path = toks[1];
    }

    return cd_node;
}

Node *parse(size_t len, Token **toks) {
    Node *node = malloc(sizeof(Node));
    if (!node) error();

    ExitNode *exit_node;
    CdNode *cd_node;
    if (exit_node = parse_exit(len, toks)) {
        node->node_type = exit_t;
        node->exit_node = exit_node;
        return node;
    } else if (cd_node = parse_cd(len, toks)) {
        node->node_type = cd_t;
        node->cd_node = cd_node;
    } else {
        free(node);
        return NULL;
    }
}

bool exec_cd(Token *path) {
    if (path->val == NULL) return false;
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
        default:
            error();
    }
}

int tokenize(size_t len, char *line, Token **toks) {
    int count = 0;
    size_t pos = 0;
    line += strspn(line, " \n");
    while (strcmp(line, "")) {
        toks[count] = malloc(sizeof(Token));
        size_t tok_size;
        if (*line == '>') {
            tok_size = 1;
        } else {
            tok_size = strcspn(line, " \n>");
        }
        toks[count]->len = tok_size;
        toks[count]->val = line;
        line += tok_size;
        line += strspn(line, " \n");
        ++count;
    }
    return count;
}

int main(int argc, char **argv) {
    size_t len, _dummy;
    char *line = malloc(MAX_LINE_LEN);
    FILE *fin = stdin;
    Node *node;
    Token **toks = malloc(MAX_LINE_LEN * sizeof(Token *));
    while ((len = getline(&line, &_dummy, fin)) != -1) {
        int n_toks = tokenize(len, line, toks);
        if (node = parse(n_toks, toks)) {
            execute(node);
        }
        else warn();
    }
}
