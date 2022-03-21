#ifndef WISH_EXECUTOR_H
#define WISH_EXECUTOR_H

#include <stdbool.h>

#include "types.h"

bool exec_cd(Token *path);

int execute(Node *node);

#endif
