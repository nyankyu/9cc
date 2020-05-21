#ifndef UTIL_H
#define UTIL_H

#include "token.h"
#include "ast.h"

int is_alnum(char c);
void dump_node(Node *node, int depth);
void dump_token(Token *token);
char *strndup(char *str, size_t len);

#endif
