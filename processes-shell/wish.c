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
    // TODO: initialize path variable

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
