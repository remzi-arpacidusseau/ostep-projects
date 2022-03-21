#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "parser.h"
#include "executor.h"
#include "tokenizer.h"
#include "types.h"
#include "utils.h"

#define MAX_LINE_LEN 4096

int main(int argc, char **argv) {
    // initialize path
    char *bin = malloc(5);
    strcpy(bin, "/bin");
    Token *p = &((Token) { .tok_type = str_t, .len = 4, .val = bin });
    PathNode path = { .n_paths = 1, .paths = &p };

    // parse arguments
    bool interactive;
    FILE *fin;
    if (argc > 2) {
        error();
    } else if (argc == 2) {
        interactive = false;
        fin = fopen(argv[1], "r");
        if (fin == NULL) error();
    } else {
        interactive = true;
        fin = stdin;
    }

    // start REPL
    size_t len, _dummy;
    char *line = malloc(MAX_LINE_LEN);
    Node *node;
    Token **toks = malloc(MAX_LINE_LEN * sizeof(Token *));
    len = 0;
    if (interactive) printf("wish> ");
    while ((len = getline(&line, &_dummy, fin)) != -1) {
        int n_toks = tokenize(len, line, toks);
        if (node = parse(n_toks, toks)) {
            execute(node, &path);
        }
        else warn();
        if (interactive) printf("wish> ");
    }
}
