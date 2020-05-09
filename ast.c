#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "token.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *new_node_ident(Token *token) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_LVAR;
  node->offset = (token->str[0] - 'a' + 1) * 8;
  return node;
}

void dump(Node *node, int depth) {
  printf("#");
  for (int i = 0; i < depth; i++)
    printf("  ");
  printf("kind: %d, val: %d, offset: %d\n", node->kind, node->val, node->offset);
  if (node->lhs != NULL)
    dump(node->lhs, depth+1);
  if (node->rhs != NULL)
    dump(node->rhs, depth+1);
}
