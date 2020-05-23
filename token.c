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

char *keyword[] = {
  "if", "else", "while", "for", "swhitch", "case", "default", "return",
  "continue", "break", "sizeof", "do",
  "int", "long", "short", "char", "float", "double", "void",
  "signed", "unsigned", "const",
  "struct", "union", "enum", "extern", "static", "typedef"
};

char *symbol[] = {
  "==", "!=", "<=", ">=", "->",
  "+", "-", "*", "/", "%", "&",
  "<", ">", "(", ")", "[", "]", "{", "}",
  "=", ",", ";", ":", "."
};

size_t is_reserved_word(char *p) {
  for (size_t i = 0; i < sizeof(keyword) / sizeof(*keyword); i++) {
    char *word = keyword[i];
    size_t len = strlen(word);
    if (strncmp(p, word, len) == 0 && !is_alnum(p[len]))
      return len;
  }
  for (size_t i = 0; i < sizeof(symbol) / sizeof(*symbol); i++) {
    char *word = symbol[i];
    size_t len = strlen(word);
    if (strncmp(p, word, len) == 0)
      return len;
  }
  return 0;
}

void tokenize(char *p) {
  Token void_head;
  current_token = &void_head;

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    size_t len = is_reserved_word(p);
    if (len) {
      new_token(TK_RESERVED, p, len);
      p += len;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      char *top = p;
      int len = 0;
      while (is_alnum(*p))
        p++,len++;
      new_token(TK_IDENT, top, len);
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

