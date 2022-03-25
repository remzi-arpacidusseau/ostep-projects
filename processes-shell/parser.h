#ifndef WISH_PARSER_H
#define WISH_PARSER_H

#include <stdbool.h>

#include "types.h"

bool parse_exit(size_t, Token **);

CdNode *parse_cd(size_t, Token **);

PathNode *parse_path(size_t, Token **);

ExecNode *parse_exec(size_t, Token **);

CommandNode *parse_command(size_t, Token **);

ParallelNode *parse_parallel(size_t, Token **);

#endif
