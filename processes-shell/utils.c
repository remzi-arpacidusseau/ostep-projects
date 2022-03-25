#include <string.h>

#include "utils.h"

void print_token(Token *tok, bool newline) {
    for (int j = 0; j < tok->len; ++j) {
        putchar(tok->val[j]);
    }
    if (newline) {
        putchar('\n');
    }
}

void warn() {
    fprintf(stderr, "An error has occurred\n");
}

void error() {
    fprintf(stderr, "error\n");
    exit(1);
}

char *tok_to_str(Token *tok) {
    if (tok->tok_type != ident_tok) return NULL;
    char *str = malloc(1 + tok->len);
    strncpy(str, tok->val, tok->len);
    str[tok->len] = '\0';
    return str;
}
