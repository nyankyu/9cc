#include <ctype.h>
#include <string.h>
#include "error.h"
#include "token.h"

Token *current_token;

bool consume(char *op) {
  if (current_token->kind != TK_RESERVED ||
      strncmp(current_token->str, op, current_token->len) != 0)
    return (false);
  current_token = current_token->next;
  return (true);
}

void expect(char *op) {
  if (current_token->kind != TK_RESERVED ||
      strncmp(current_token->str, op, current_token->len) != 0)
    error("'%s'ではありません。", op);
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

Token *new_token(TokenKind kind, char *str, int len) {
  Token *token = calloc(1, sizeof(Token));
  if (token == NULL)
    error_str("callc()が失敗しました。");
  token->kind = kind;
  token->str = str;
  token->len = len;
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

    if ('a' <= *p && *p <= 'z') {
      new_token(TK_IDENT, p, 1);
      continue;
    }

    if (strncmp(p, "==", 2) == 0 ||
        strncmp(p, "!=", 2) == 0 ||
        strncmp(p, "<=", 2) == 0 ||
        strncmp(p, ">=", 2) == 0) {
      new_token(TK_RESERVED, p, 2);
      p += 2;
      continue;
    }

    if (*p == '+' ||
        *p == '-' ||
        *p == '*' ||
        *p == '/' ||
        *p == '<' ||
        *p == '>' ||
        *p == '(' ||
        *p == ')') {
      new_token(TK_RESERVED, p, 1);
      p++;
      continue;
    }

    if (isdigit(*p)) {
      new_num_token(&p);
      continue;
    }

    error("トークナイズできません。");
  }

  new_token(TK_EOF, p, 1);
  current_token = void_head.next;
}

