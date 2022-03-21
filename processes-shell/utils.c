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
    printf("error\n");
}

void error() {
    printf("error\n");
    exit(1);
}

char *tok_to_str(Token *tok) {
    if (tok->tok_type != str_t) return NULL;
    char *str = malloc(1 + tok->len);
    strncpy(str, tok->val, tok->len);
    str[tok->len] = '\0';
    return str;
}
