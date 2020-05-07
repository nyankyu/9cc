#include "parser.h"
#include "token.h"
#include "ast.h"

Node *primary() {
  if (consume("(")) {
      Node *node = expr();
      expect(")");
      return (node);
  }

  return new_node_num(expect_number());
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    else if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return (node);
  }
}

Node *add() {
  Node *node = mul();

  for (;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    else if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return (node);
  }
}

Node *relational() {
  Node *node = add();

  for (;;) {
    if (consume("<"))
      node = new_node(ND_LESS, node, add());
    else if (consume(">"))
      node = new_node(ND_GREAT, node, add());
    else if (consume("<="))
      node = new_node(ND_LESS_EQUAL, node, add());
    else if (consume(">="))
      node = new_node(ND_GREAT_EQUAL, node, add());
    else
      return (node);
  }
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQUAL, node, relational());
    else if (consume("!="))
      node = new_node(ND_NOT_EQUAL, node, relational());
    else
      return (node);
  }
}

Node *expr() {
  return (equality());
}

