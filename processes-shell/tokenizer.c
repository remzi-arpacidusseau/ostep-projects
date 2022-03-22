#ifndef WISH_TOKENIZER_H
#define WISH_TOKENIZER_H

#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

int tokenize(size_t len, char *line, Token **toks) {
    int count = 0;
    size_t pos = 0;
    line += strspn(line, " \n");
    while (strcmp(line, "")) {
        toks[count] = malloc(sizeof(Token));

        size_t tok_size;
        int tok_type;
        if (*line == '>') {
            tok_size = 1;
            tok_type = rangle_tok;
        } else {
            tok_size = strcspn(line, " \n>");
            tok_type = ident_tok;
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
