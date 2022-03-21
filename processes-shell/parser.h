#ifndef WISH_PARSER_H
#define WISH_PARSER_H

#include "types.h"

ExitNode *parse_exit(size_t, Token **);

CdNode *parse_cd(size_t, Token **);

PathNode *parse_path(size_t, Token **);

ExecNode *parse_exec(size_t, Token **);

Node *parse(size_t, Token **);

#endif
