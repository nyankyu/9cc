#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "ast.h"
#include "util.h"

// list of local variable
typedef struct LVar LVar;
struct LVar {
  LVar *next;
  char *name;
  int len;
  int offset;
};

LVar *locals = NULL;

LVar *find_lvar(Token *token) {
  for (LVar *var = locals; var; var = var->next)
    if (var->len == token->len && memcmp(token->str, var->name, var->len) == 0)
      return var;
  return NULL;
}

Node *primary() {
  if (consume("(")) {
      Node *node = expr();
      expect(")");
      return (node);
  }

  Token *token = consume_ident();
  if (token) {
    LVar *lvar = find_lvar(token);
    if (lvar == NULL) {
      lvar = calloc(1, sizeof(LVar));
      lvar->next = locals;
      lvar->name = token->str;
      lvar->len = token->len;
      lvar->offset = locals ? locals->offset + 8 : 8;
      locals = lvar;
    }

    return new_node_ident(lvar->offset);
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

Node *assign() {
  Node *node = equality();
  if (consume("="))
    node = new_node(ND_ASSIGN, node, assign());
  return (node);
}

Node *expr() {
  return (assign());
}

Node *stmt() {
  Node *node;

  if (consume_kind(TK_RETURN)) {
    node = new_node(ND_RETURN, expr(), NULL);
  } else if (consume_kind(TK_IF)) {
    puts("# is if");
    consume("(");
    puts("# is (");
    Node *if_expr = expr();
    puts("# is expr");
    expect(")");
    puts("# is )");
    node = new_node(ND_IF, if_expr, stmt());
    puts("# is stmt");
    return node;
  } else {
    node = expr();
  }
  expect(";");
  return node;
}


void program() {
  int i = 0;
  while (!at_eof()) {
    g_code[i++] = stmt();
    dump_node(g_code[i-1], 0);
  }
  g_code[i] = NULL;
}

