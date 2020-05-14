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
  ND_IF,          // if
  ND_WHILE,       // while
  ND_FOR,         // for
  ND_BLOCK,       // {}
} NodeKind;

typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;

  int val;
  int offset;     // offset from RBP

  // "if", "for", "while" statement
  Node *init;
  Node *cnd;
  Node *step;
  Node *then;
  Node *els;

  // block
  Node *body;
  Node *next;
};

Node *new_for(Node *init, Node *cnd, Node *step, Node *then);
Node *new_while(Node *cnd, Node *then);
Node *new_if(Node *cnd, Node *then);
Node *add_else(Node *node, Node *els);
Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_ident(int offset);

#endif

