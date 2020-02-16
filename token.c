#include "error.h"
#include "token.h"

bool consume(char op) {
  if (current_token->kind != TK_RESERVED ||
    current_token->str[0] != op)
    return (false);
  current_token = current_token->next;
  return (true);
}

void expect(char op) {
  if (current_token->kind != TK_RESERVED ||
    current_token->str[0] != op)
    error("'%c'ではありません。", op);
  current_token = current_token->next;
}

int expect_number() {
  if (current_token->kind != TK_NUM)
    error("数値ではありません。");
  int val = current_token->val;
  current_token = current_token->next;
  return (val);
}

bool at_eof() {
  return (current_token->kind == TK_EOF);
}

Token *new_token(TokenKind kind, char *str) {
  Token *token = calloc(1, sizeof(Token));
  if (token == NULL)
    error_str("callc()が失敗しました。");
  token->kind = kind;
  token->str = str;
  current_token->next = token;
  return token;
}
