#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>

typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_EOF
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
void tokenize(char *p);

#endif

