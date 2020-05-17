#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

Function *g_function[256];
Node *g_code[100];

Node *expr();
void program();
void gen_function();

#endif
