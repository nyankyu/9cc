#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "token.h"

Node *new_call(Token *token, Node *param) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_CALL;
  node->ident = token->str;
  node->len = token->len;
  node->param = param;
  return node;
}

Node *new_for(Node *init, Node *cnd, Node *step, Node *then) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_FOR;
  node->init = init;
  node->cnd = cnd;
  node->step = step;
  node->then = then;
  return node;
}

Node *new_while(Node *cnd, Node *then) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_WHILE;
  node->cnd = cnd;
  node->then = then;
  return node;
}

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

Node *new_node_ident(LVar *lvar) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_LVAR;
  node->offset = lvar->offset;
  node->type = lvar->type;
  return node;
}

