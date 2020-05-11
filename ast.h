#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
  ND_ADD,         // +
  ND_SUB,         // -
  ND_MUL,         // *
  ND_DIV,         // /
  ND_NUM,         // integer
  ND_EQUAL,       // ==
  ND_NOT_EQUAL,   // !=
  ND_GREAT,       // >
  ND_LESS,        // <
  ND_GREAT_EQUAL, // >=
  ND_LESS_EQUAL,  // <=
  ND_ASSIGN,      // =
  ND_LVAR,        // local variable
  ND_RETURN,      // return
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
  int offset;     // offset from RBP
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_ident(int offset);
void dump(Node *node, int depth);

#endif

