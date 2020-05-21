#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "stdio.h"

int is_alnum(char c) {
  return ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         ('0' <= c && c <= '9') ||
         (c == '_');
}

void dump_node(Node *node, int depth) {
  printf("#");
  for (int i = 0; i < depth; i++)
    printf("  ");
  printf("kind: %d, val: %d, offset: %d\n", node->kind, node->val, node->offset);
  if (node->lhs != NULL)
    dump_node(node->lhs, depth+1);
  if (node->rhs != NULL)
    dump_node(node->rhs, depth+1);
}

void dump_token(Token *token) {
  if (token->kind == TK_NUM)
    printf("# kind:%d, val:%d\n", TK_NUM, token->val);
  else {
    char buff[256] = {0};
    strncpy(buff, token->str, token->len);
    printf("# kind:%d, str:%s, len:%ld\n", token->kind, buff, token->len);
  }
}

char *strndup(char *str, size_t len) {
  char *cpy = malloc(sizeof(char) * len + 1);
  strncpy(cpy, str, len);
  cpy[len] = '\0';
  return cpy;
}
