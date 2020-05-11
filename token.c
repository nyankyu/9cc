#include <ctype.h>
#include <string.h>
#include "error.h"
#include "token.h"

Token *current_token;

int is_alnum(char c) {
  return ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') ||
         (c == '_');
}

bool consume_kind(TokenKind kind) {
  if (current_token->kind != kind)
    return (false);
  current_token = current_token->next;
  return (true);
}

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
  printf("# kind:%d, str:%s, len:%d\n", kind, str, len);
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
  printf("# kind:%d, val:%d\n", TK_NUM, token->val);
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

    if (memcmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
      new_token(TK_RETURN, p, 6);
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

