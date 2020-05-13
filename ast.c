#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "token.h"

Node *add_else(Node *node, Node *els) {
  node->els = els;
  return node;
}

Node *new_if(Node *cnd, Node *then) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_IF;
  node->cnd = cnd;
  node->then = then;
  return node;
}

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

Node *new_node_ident(int offset) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_LVAR;
  node->offset = offset;
  return node;
}

