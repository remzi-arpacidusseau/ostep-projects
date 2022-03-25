#ifndef WISH_TOKENIZER_H
#define WISH_TOKENIZER_H

#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

int tokenize(size_t len, char *line, Token **toks) {
    int count = 0;
    line += strspn(line, " \n");
    while (strcmp(line, "")) {
        toks[count] = malloc(sizeof(Token));

        size_t tok_size;
        int tok_type;
        if (*line == '>') {
            tok_size = 1;
            tok_type = rangle_tok;
        } else if (*line == '&') {
            tok_size = 1;
            tok_type = ampand_tok;
        } else {
            tok_size = strcspn(line, " \n>&");
            if (tok_size == 4 && !strncmp(line, "exit", 4)) {
                tok_type = exit_tok;
            } else if (tok_size == 2 && !strncmp(line, "cd", 2)) {
                tok_type = cd_tok;
            } else if (tok_size == 4 && !strncmp(line, "path", 4)) {
                tok_type = path_tok;
            } else {
                tok_type = ident_tok;
            }
        }
        toks[count]->len = tok_size;
        toks[count]->val = line;
        toks[count]->tok_type = tok_type;

        line += tok_size;
        line += strspn(line, " \n");
        ++count;
    }
    return count;
}

#endif
