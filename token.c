#include <ctype.h>
#include "error.h"
#include "token.h"

Token *current_token;

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

Token *new_op_token(TokenKind kind, char *str) {
  Token *token = calloc(1, sizeof(Token));
  if (token == NULL)
    error_str("callc()が失敗しました。");
  token->kind = kind;
  token->str = str;
  current_token->next = token;
  current_token = token;
  return token;
}

Token *new_num_token(char **str) {
  Token *token = calloc(1, sizeof(Token));
  if (token == NULL)
    error_str("callc()が失敗しました。");
  token->kind = TK_NUM;
  token->val = strtol(*str, str, 10);
  current_token->next = token;
  current_token = token;
  return token;
}

void tokenize(char *p) {
  Token void_head;
  current_token = &void_head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      new_op_token(TK_RESERVED, p);
      p++;
      continue;
    }

    if (isdigit(*p)) {
      new_num_token(&p);
      continue;
    }

    error("トークナイズできません。");
  }

  new_op_token(TK_EOF, p);
  current_token = void_head.next;
}

