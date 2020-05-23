#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "token.h"
#include "ast.h"
#include "util.h"

LVar *g_locals;

LVar *find_lvar(Token *token) {
  for (LVar *var = g_locals; var; var = var->next)
    if (var->len == token->len && memcmp(token->str, var->name, var->len) == 0)
      return var;
  return NULL;
}

Node *param() {
  Node head = {};
  Node *cur = &head;
  cur = cur->next = expr();
  while (!consume(")")) {
    expect(",");
    cur = cur->next = expr();
  }
  return head.next;
}

Node *primary() {
  if (consume("(")) {
      Node *node = expr();
      expect(")");
      return (node);
  }

  Token *token = consume_ident();
  if (!token) {
    return new_node_num(expect_number());
  }

  if (consume("(")) {
    Node *param_list = NULL;
    if (!consume(")")) {
      param_list = param();
    }
    return new_call(token, param_list);
  }

  LVar *lvar = find_lvar(token);
  if (lvar == NULL) {
    lvar = calloc(1, sizeof(LVar));
    lvar->next = g_locals;
    lvar->name = token->str;
    lvar->len = token->len;
    lvar->offset = g_locals ? g_locals->offset + 8 : 8;
    g_locals = lvar;
  }

  return new_node_ident(lvar->offset);
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  if (consume("&"))
    return new_node(ND_ADDR, unary(), NULL);
  if (consume("*"))
    return new_node(ND_DEREF, unary(), NULL);
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

  if (consume("return")) {
    node = new_node(ND_RETURN, expr(), NULL);
  } else if (consume("if")) {
    consume("(");
    Node *if_expr = expr();
    expect(")");
    node = new_if(if_expr, stmt());

    if (consume("else")) {
      node = add_else(node, stmt());
    }
    return node;
  } else if (consume("while")) {
    consume("(");
    Node *cnd = expr();
    expect(")");
    node = new_while(cnd, stmt());
    return node;
  } else if (consume("for")) {
    consume("(");
    Node *init = expr();
    expect(";");
    Node *cnd = expr();
    expect(";");
    Node *step = expr();
    expect(")");
    node = new_for(init, cnd, step, stmt());
    return node;
  } else if (consume("{")) {
    node = new_node(ND_BLOCK, NULL, NULL);
    Node head = {};
    Node *cur = &head;
    while (!consume("}")) {
      cur = cur->next = stmt();
    }
    node->body = head.next;
    return node;
  } else {
    node = expr();
  }
  expect(";");
  return node;
}

size_t set_args() {
  size_t args_size = 0;
  expect("(");
  while (!consume(")")) {
    Token *token = consume_ident();
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->next = g_locals;
    lvar->name = token->str;
    lvar->len = token->len;
    lvar->offset = g_locals ? g_locals->offset + 8 : 8;
    g_locals = lvar;
    args_size++;
    consume(",");
  }
  return args_size;
}

Function *function() {
  Function *func = calloc(1, sizeof(Function));

  Token *token = consume_ident();
  func->name = token->str;
  func->len = token->len;

  g_locals = NULL;
  func->args_size = set_args();

  expect("{");
  Node *node = new_node(ND_BLOCK, NULL, NULL);
  Node head = {};
  Node *cur = &head;
  while (!consume("}")) {
    cur = cur->next = stmt();
  }
  node->body = head.next;
  func->block = node;

  return func;
}


void program() {
  int i = 0;
  while (!at_eof()) {
    g_function[i++] = function();
  }
  g_function[i] = NULL;
}

