#include <stdbool.h>

typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
};

Token *current_token;
