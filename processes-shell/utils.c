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