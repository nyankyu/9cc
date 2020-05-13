#include <ctype.h>
#include <string.h>
#include "error.h"
#include "token.h"
#include "util.h"

Token *current_token;

bool consume(char *op) {
  if (strlen(op) != current_token->len)
    return (false);
  if (current_token->kind != TK_RESERVED ||
      strncmp(current_token->str, op, current_token->len) != 0)
    return (false);
  current_token = current_token->next;
  return (true);
}

Token *consume_ident() {
  if (current_token->kind != TK_IDENT)
    return (NULL);
  Token *token = current_token;
  current_token = current_token->next;
  return (token);
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
  dump_token(token);
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
  dump_token(token);
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

    // TODO 関数化
    if (is_keyword(p, "if", 2)) {
      new_token(TK_RESERVED, p, 2);
      p += 2;
      continue;
    }

    if (is_keyword(p, "else", 4)) {
      new_token(TK_RESERVED, p, 4);
      p += 4;
      continue;
    }

    if (is_keyword(p, "while", 5)) {
      new_token(TK_RESERVED, p, 5);
      p += 5;
      continue;
    }

    if (is_keyword(p, "for", 3)) {
      new_token(TK_RESERVED, p, 3);
      p += 3;
      continue;
    }

    if (is_keyword(p, "return", 6)) {
      new_token(TK_RESERVED, p, 6);
      p += 6;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      char *top = p;
      int len = 0;
      while ('a' <= *p && *p <= 'z')
        p++,len++;
      new_token(TK_IDENT, top, len);
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
        *p == ')' ||
        *p == '=' ||
        *p == ';') {
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

