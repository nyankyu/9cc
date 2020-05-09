#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

Node *g_code[100];

Node *expr();
void program();

#endif
